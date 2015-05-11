# CurrentSensor

CurrentSensor is for a measurement tool for HDD power consumption.

## Description

## CurrentSensor core

### Revision 1

CurrentSensor revision 1 is first type to measure consumed power of a disk, and able to choice measuring power each 12 V, 5 V, 3 V. Values measured the power are sent through I2C. We recommend using Arduino UNO R3 to received I2C signal.

### Revision 2

CurrentSensor revision 2 changes to measure 3 kinds of power together. This revision is for the 3.5 inch Hard Disk because it uses both 12 V and 5 V power.

### Revision 3

CurrentSensor revision 3 changes to link a few CurrentSensor R2s through I2C bus. If you would like to measure consumed power of a few disks at one time, revision 3 is very useful.

## Controller Shield

Controller Shield for Arduino allows you to be able to control the CurrentSensor R3, and includes 2 switches and a 16x2 charactor LCD. 

