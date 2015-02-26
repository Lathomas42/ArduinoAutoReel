#include <AutoDriver.h>
#include <CmdMessenger.h>

#define CS_PIN 10
#define RESET_PIN 6
#define BUSY_PIN 5
#define DEBUG 1

class AutoReel: public AutoDriver {
  private:
    int max_speed;
    int acc;
    int dec;
    int k;
    int ms;
    boolean low_speed;
    int _bp;
    
  public:
    AutoReel(int CSPin, int resetPin, int busyPin)
    : AutoDriver(CSPin, resetPin, busyPin),
      max_speed(100),
      acc(100),
      dec(100),
      k(8),
      ms(7),
      low_speed(false),
      _bp(busyPin)
      {}
    void configure () {
      #ifdef DEBUG
        Serial.println("~configure");
        Serial.print("~max_speed "); Serial.println(max_speed, DEC);
        Serial.print("~acc "); Serial.println(acc, DEC);
        Serial.print("~dec "); Serial.println(dec, DEC);
        Serial.print("~k "); Serial.println(k, DEC);
        Serial.print("~ms "); Serial.println(ms, DEC);
        Serial.print("~low_speed "); Serial.println(low_speed, DEC);
      #endif
      resetDev();
      configSyncPin(_bp, 0);
      configStepMode(ms);
      setMaxSpeed(max_speed);
      setFullSpeed(100000);
      setLoSpdOpt(low_speed);
      setAcc(acc);
      setDec(dec);
      setSlewRate(SR_530V_us);
      setOCThreshold(OC_6000mA);
      setPWMFreq(PWM_DIV_2, PWM_MUL_2);
      setOCShutdown(OC_SD_DISABLE);
      setVoltageComp(VS_COMP_DISABLE);
      setSwitchMode(SW_USER);
      setOscMode(INT_16MHZ_OSCOUT_16MHZ);
      setAccKVAL(k);
      setDecKVAL(k);
      setRunKVAL(k);
      setHoldKVAL(k);
    };
    
    
    
    
};


AutoReel reel = AutoReel(CS_PIN, RESET_PIN, BUSY_PIN);
CmdMessenger cmdMessenger = CmdMessenger(Serial);

enum
{
  //Commands
  kConfigure,     //0
  kStop,              //1
  kRelease,         //2
  kVelocity,        //3
  kAcceleration, //4
  kDeceleration, //5
  kAccK,             //6
  kDecK,             //7
  kRunK,            //8
  kHoldK,           //9
  kms,                //10
  kLowSpeed,     //11
  kRotate,           //12
  kMove,            //13
  kCheckStatus, //14
};

void attachCommandCallbacks()
{
  cmdMessenger.attach(kConfigure, config);
  cmdMessenger.attach(kStop, stop_cmd);
  cmdMessenger.attach(kRelease, release);
  cmdMessenger.attach(kVelocity, velocity);
  cmdMessenger.attach(kAcceleration, acceleration);
  cmdMessenger.attach(kDeceleration, deceleration);
  cmdMessenger.attach(kAccK, accKVal);
  cmdMessenger.attach(kDecK, decKVal);
  cmdMessenger.attach(kRunK, runKVal);
  cmdMessenger.attach(kHoldK, holdKVal);
  cmdMessenger.attach(kms, microStep);
  cmdMessenger.attach(kLowSpeed, low_speed);
  cmdMessenger.attach(kRotate, rotate);
  cmdMessenger.attach(kMove, moveSteps);
  cmdMessenger.attach(kCheckStatus, check);
};

void config(){
  reel.configure();
};

void stop_cmd(){
  //pass 0 for soft 1 for hard
  int16_t soft = cmdMessenger.readBinArg<uint16_t>();
  if (soft == 0){
    reel.softStop();
  }
  else{
    reel.hardStop();
  }
};

void release(){
  reel.softHiZ();
}

void velocity(){
  float vel = cmdMessenger.readBinArg<float>();
  reel.setMaxSpeed(vel);
}

void acceleration(){
  float acc = cmdMessenger.readBinArg<float>();
  reel.setAcc(acc);
}

void deceleration(){
  float dec = cmdMessenger.readBinArg<float>();
  reel.setDec(dec);
}

void accKVal(){
  int16_t k = cmdMessenger.readBinArg<int16_t>();
  reel.setAccKVAL(k);
}

void decKVal(){
  int16_t k = cmdMessenger.readBinArg<int16_t>();
  reel.setDecKVAL(k);
}

void runKVal(){
  int16_t k = cmdMessenger.readBinArg<int16_t>();
  reel.setRunKVAL(k);
}

void holdKVal(){
  int16_t k = cmdMessenger.readBinArg<int16_t>();
  reel.setHoldKVAL(k);
}

void microStep(){
  // input can be 1, 2, 4, 8, .... , 128 microsteps per full step
  int16_t steps = cmdMessenger.readBinArg<int16_t>();
  reel.configStepMode(steps);
}

void low_speed(){
  boolean low = cmdMessenger.readBoolArg();
  reel.setLoSpdOpt(low);
}

void rotate(){
  //byte direction, float steps_per_sec
  int16_t dir = cmdMessenger.readBinArg<int16_t>();
  float sps = cmdMessenger.readBinArg<float>();
  Serial.print(dir);
  Serial.print(',');
  Serial.println(sps);
  reel.run(dir, sps);
}

void moveSteps(){
  //byte direction, long number of steps
  int16_t dir = cmdMessenger.readBinArg<int16_t>();
  Serial.println(dir);
  int16_t steps = cmdMessenger.readBinArg<int16_t>();
  Serial.println(steps);
  reel.move(dir, steps);
}

void check(){
  //returns 1 or 0 
  int a = cmdMessenger.sendBinCmd(kCheckStatus, reel.getStatus());
  Serial.println(a);
}

void setup(){
  Serial.begin(9600);
  reel.configure();
  cmdMessenger.printLfCr();
  attachCommandCallbacks();
}

void loop(){
  cmdMessenger.feedinSerialData();
}
