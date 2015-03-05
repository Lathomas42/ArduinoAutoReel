import cmdmessenger
import serial

cmds = [ 
  {'name': 'kConfigure'},
  {
    'name': 'kStop',
    'params': ['bi16'],
  },
  {'name': 'kRelease'},
  {
    'name': 'kVelocity',
    'params':['bf'],
  },
  {
    'name': 'kAcceleration',
    'params':['bf'],
  },
  {
    'name': 'kDeceleration',
    'params': ['bf'],
  },
  {
    'name': 'kAccK',
    'params': ['bi16'],
  },
  {
    'name': 'kDecK',
    'params': ['bi16'],
  },
  {
    'name': 'kRunK',
    'params': ['bi16'],
  },
  {
    'name': 'kHoldK',
    'params': ['bi16'],
  },
  {
    'name': 'kms',
    'params': ['bi16'],
  },
  {
    'name': 'kLowSpeed',
    'params': ['bBool'],
  },
  {
    'name': 'kRotate',
    'params': ['bi16', 'bf'],
  },
  {
    'name': 'kMove',
    'params': ['bi16', 'bi32'],
  },
  {'name': 'kCheckStatus'}]

class AutoReelMessenger():
    def __init__(self, port='COM4', rate=9600):
        self._port = port
        self._serial = serial.Serial(port, rate)
        self.messenger = cmdmessenger.Messenger(self._serial, cmds)
        print(cmds)
    
    def config(self):
        self.messenger.call('kConfigure')
    
    def stop(self, soft=0):
        """
        stops the motor, soft = 0 creates a soft stop
        """
        self.messenger.call('kConfigure', soft)
        
    def release(self):
        self.messenger.call('kRelease')
        
    def velocity(self, vel):
        """
        changes velocity of motor vel is a float arg
        """
        self.messenger.call('kVelocity',vel)
    
    def acceleration(self, acc):
        """
        changes acceleration of motor, acc is a float arg
        """
        self.messenger.call('kAcceleration', acc)
    
    def deceleration(self, dec):
        """
        sets teh deceleration of the motor, dec is a float arg
        """
        self.messenger.call('kDeceleration', dec)
        
    def accKVal(self, k):
        """
        sets the accKVAL to int k
        """
        self.messenger.call('kAccK', k)
        
    def decKVal(self, k):
        """
        sets the decKVAL to int k
        """
        self.messenger.call('kDecK', k)
        
    def runKVal(self, k):
        """
        sets the RunKVAL to int k
        """
        self.messenger.call('kRunK', k)
        
    def holdKVal(self, k):
        """
        sets the holdKVAL to int k
        """
        self.messenger.call('kHoldK', k)
        
    def microstep(self, n_steps):
        """
        microsteps the motor n_steps times (int)
        """
        self.messenger.call('kms', n_steps)
        
    def low_speed(self, low):
        """
        activates or deactivates low speed mode according to boolean low
        """
        self.messenger.call('kLowSpeed', low)
        
    def rotate(self, dir, sps):
        """
        rotates byte direction, float steps per second
        """
        self.messenger.call('kRotate', dir, sps)

    def moveSteps(self, dir, n_steps):
        """
        rotates byte direction n_steps amount of steps
        """
        self.messenger.call('kMove', dir, n_steps)
        
    def check(self):
        self.messenger.call('kCheckStatus')