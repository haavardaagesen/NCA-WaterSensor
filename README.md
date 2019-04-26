# NCA Water Sensor Project

The NCA Water Sensor Project aims to build an IoT water sensor that reports from humanitarian water projects around the world. In this repository you will find the Arduino code for a Arduino MKR GSM 1400 and a php-file for the MySql-connection. We have tried to use as few components and as little code as possible, to make the project as flexible as possible for others to utilize.

Although we at NCA do not have the capacity to follow up contributions, we strongly encourage you to utilize and build upon our previous work. 

## Bill of Materials


| ID | Part name                | Quantity|
|----|--------------------------|---------|
| A1 | Arduino MKR GSM 1400     | 1       |
| L1 | LED                      | 1       |
| S1 | 2" Water meter           | 1       |
| B1 | Power bank 5000mAh       | 1		  |
| B2 | Battery min 2000mAh 3.7V | 1		  |


## Dependencies

* MKRGSM.h
* OneWire.h

## Assembly

* Connect the water meter to digital pin 0 on the Arduino and the indicator LED to digital pin 5.
* Insert SIM-card into the Arduino.
* Connect a power source to the power bank.
* Connect the Arduino to a battery and the power bank.
* Modify and load the code to the Arduino.

## License
This project is released under a CC BY-SA 4.0 License.





