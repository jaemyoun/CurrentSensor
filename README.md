# CurrentSensor

CurrentSensor is for a measurement tool for HDD power consumption.

## Description

## CurrentSensor core

### Revision 1

CurrentSensor revision 1 is first type to measure consumed power of a disk, and able to choice measuring power each 12 V, 5 V, 3 V. Values measured the power are sent through I2C. We recommend using Arduino UNO R3 to received I2C signal.

Schematics : [Link](currentsensor-core/R1/hardware/currentsensor-core-R1-schematics.pdf)

Gerber files : [Link](currentsensor-core/R1/hardware/currentsensor-core-R1-Gerber.7z)

NC Drill files : [Link](currentsensor-core/R1/hardware/currentsensor-core-R1-NCDrill.7z)

<img src="https://raw.githubusercontent.com/jaemyoun/CurrentSensor/master/res/current_sensor_r1.JPG" width="400px"/>

### Revision 2

CurrentSensor revision 2 changes to measure 3 kinds of power together. This revision is for the 3.5 inch Hard Disk because it uses both 12 V and 5 V power.

Schematics : [Link](currentsensor-core/R2/hardware/currentsensor-core-R2-schematics.pdf)

Gerber files : [Link](currentsensor-core/R2/hardware/currentsensor-core-r2-Gerber.7z)

NC Drill files : [Link](currentsensor-core/R2/hardware/currentsensor-core-r2-NCDrill.7z)

<img src="https://raw.githubusercontent.com/jaemyoun/CurrentSensor/master/res/current_sensor_r2.JPG" width="400"/>

### Revision 3

CurrentSensor revision 3 changes to link a few CurrentSensor R2s through I2C bus. If you would like to measure consumed power of a few disks at one time, revision 3 is very useful.

Schematics : [Link](currentsensor-core/R3/hardware/currentsensor-core-R3-schematics.pdf)

Gerber files : [Link](currentsensor-core/R3/hardware/currentsensor-core-R3-Gerber.7z)

NC Drill files : [Link](currentsensor-core/R3/hardware/currentsensor-core-R3-NCDrill.7z)

<img src="https://raw.githubusercontent.com/jaemyoun/CurrentSensor/master/res/current_sensor_R3_front.JPG" width="400px"/>

<img src="https://raw.githubusercontent.com/jaemyoun/CurrentSensor/master/res/current_sensor_R3_back.JPG" width="400px"/>

<img src="https://raw.githubusercontent.com/jaemyoun/CurrentSensor/master/res/current_sensor_R3_5ea.JPG" width="400px"/>

## Controller Shield

Controller Shield for Arduino allows you to be able to control the CurrentSensor R3, and includes 2 switches and a 16x2 charactor LCD. 

Schematics : [Link](controller-shield/hardware/controller-shield-schematics.pdf)

Gerber files : [Link](controller-shield/hardware/controller-shield-Gerber.7z)

NC Drill files : [Link](controller-shield/hardware/controller-shield-NCDrill.7z)

<img src="https://raw.githubusercontent.com/jaemyoun/CurrentSensor/master/res/ctrl4cs.png" width="400px"/>
