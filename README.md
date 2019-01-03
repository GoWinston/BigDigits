# The Big Board

This is an Arduino powered system to allow the construction of giant seven segment displays, up to 50V 0.5A per segment.

Two PCB designs are included, one with four digits on one board and another with a single digit per board.  

The hardware and library support four channels. 

<hr>

Boards are availible at OshPark <br>
4 Channel: https://oshpark.com/shared_projects/9ZOlH1Lw <br>
Digikey parts list [here](https://github.com/GoWinston/BigDigits/blob/master/PCB's/1%20Digit%20PCB%20Materials%20Digikey.csv) <br>
<br>
1 Channel: https://oshpark.com/shared_projects/VTMd3b0f <br>
Digikey parts list [here](https://github.com/GoWinston/BigDigits/blob/master/PCB's/4%20Digit%20PCB%20Materials%20Digikey.csv) <br>

<hr>

Connect to the Arduino with a standard ethernet cable and the SparkFun RJ45 Breakout <br>
https://www.sparkfun.com/products/716 <br>
<br>
```
Ethernet | Arduino | Assignment
    1         13      Clock - SCK - SHCP
    2         11      Data - MOSI - DS
    3         9       Digit D  X---  Thousands
    4         6       Digit A  ---X  Units
    5         8       Digit C  -X--  Hundreds
    6         7       Digit B  --X-  Tens
    7         +5      VCC
    8         GND     GND
```
<hr>

The power for the big displays goes directly into the PCBs and is seperate from the logic power on the Arduino which travels over the ethernet cable<br>
<hr>

![4 channel board](https://github.com/GoWinston/BigDigits/blob/master/PCB's/Auction_Multi_RevA_Final/Multi-Render.jpeg "4 channel board")

![1 channel board](https://github.com/GoWinston/BigDigits/blob/master/PCB's/Auction_Single_RevB_Final/Single-Render.jpeg "1 channel board")
