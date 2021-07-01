from m5stack import *
from m5ui import *
from uiflow import *
import imu
import time
import unit

setScreenColor(0x111111)
neopixel0 = unit.get(unit.NEOPIXEL, unit.PORTA, 100)




imu0 = imu.IMU()
line0 = M5Line(M5Line.PLINE, 158, 0, 158, 150, 0x00ff00)
line1 = M5Line(M5Line.PLINE, 158, 150, 2, 239, 0x0000ff)
line2 = M5Line(M5Line.PLINE, 158, 150, 317, 238, 0xff0000)
label0 = M5TextBox(130, 0, "Y", lcd.FONT_Default, 0x00ff00, rotate=0)
label1 = M5TextBox(7, 190, "Z", lcd.FONT_Default, 0x0000ff, rotate=0)
label2 = M5TextBox(302, 190, "X", lcd.FONT_Default, 0xff0000, rotate=0)
circle0 = M5Circle(160, 153, 15, 0xFFFFFF, 0xFFFFFF)


while True:
  line0.setSize(158, 0, 158, 150)
  line1.setSize(158, 150, 2, 239)
  line2.setSize(158, 150, 317, 238)
  if (imu0.gyro[0]) >= 200:
    circle0.setPosition(284, 219)
    neopixel0.setColorAll(0xff0000)
    wait(0.5)
  elif (imu0.gyro[0]) <= -200:
    circle0.setPosition(53, 80)
    neopixel0.setColorAll(0xff0000)
    wait(0.5)
  elif (imu0.gyro[1]) >= 200:
    circle0.setPosition(160, 23)
    neopixel0.setColorAll(0x006600)
    wait(0.5)
  elif (imu0.gyro[1]) <= -200:
    circle0.setPosition(160, 214)
    neopixel0.setColorAll(0x006600)
    wait(0.5)
  elif (imu0.gyro[2]) >= 200:
    circle0.setPosition(31, 219)
    neopixel0.setColorAll(0x000099)
    wait(0.5)
  elif (imu0.gyro[2]) <= -200:
    circle0.setPosition(292, 83)
    neopixel0.setColorAll(0x000099)
    wait(0.5)
  else:
    circle0.setPosition(160, 153)
    neopixel0.setColorAll(0xffffff)
  wait_ms(2)