"""
Goal: Create python library to talk to ArduinoAutoreel using CmdMessenger
"""

import serial
import time
import numpy
import json

ByteMap = {'dSPIN':{'FWD': {'hex': '0x01','int': 1},
                    'REV': {'hex': '0x01','int': 0}},
           'STEP_FS':{0: {'hex': '0x00', 'int':0},
                      2: {'hex': '0x01','int': 1},
                      8: {'hex': '0x02','int': 2},
                      8: {'hex': '0x03', 'int':3},
                      16: {'hex': '0x04', 'int':4},
                      32: {'hex': '0x05', 'int':5},
                      64: {'hex': '0x06', 'int':6},
                      128: {'hex': '0x07', 'int':7}}}
             
class AutoReel(object):
    def __init__(self,port):
        self.con = serial.Serial(port, 9600)
    
    def config(self):
        self.con.write('0;')
    
    def stop(hard=False):
        self.con.write('1, {};'.format(int(hard)))
    
    def release():
        self.con.write('2;')
    
    def set_velocity(vel):
        self.con.write('3, {};'.format(vel))
    
    def set_acceleration(acc):
        self.con.write('4, {};'.format(acc))
        
    def set_deceleration(dec):
        self.con.write('5, {};'.format(dec))
        
    def set_accKVAL(k):
        self.con.write('6, {};'.format(k))
        
    def set_decKVAL(k):
        self.con.write('7, {};'.format(k))
        
    def set_runKVAL(k):
        self.con.write('8, {};'.format(k))
        
    def set_holdKVAL(k):
        self.con.write('9, {};'.format(k))
        
    def microstep(n_steps):
        """
        sets stepping mode to Microstepping with n_steps
        per full step (n_steps must be power of 2)
        """
        self.con.write('10, {};'.format(ByteMap['STEP_FS'][n_steps]['int']))
        
    def lowSpeed(low=True):
        self.con.write('11, {};'.format(int(low)))
    
    def rotate(speed, dir='FWD'):
        """
        input either FWD or REV for dir
        """
        self.con.write('12, {}, {};'.format(ByteMap['dSPIN'][dir]['int'], speed))
        
    def moveSteps(steps, dir='FWD'):
        self.con.write('13, {}, {};'.format(ByteMap['dSPIN'][dir]['int'], steps))
        
    def check():
        self.con.write('14;')
        