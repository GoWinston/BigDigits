/*
*  BigBoard Library Demo Sketch
*  
*  Some code for rolling your own giant seven-segment display with the BigBoard PCB.
*  Get your own board at http://example.com
*  
*  Author		Winston Cutshall
*           http://example.com
*  
*  Date		10/29/15
*  Version	0.9 Beta
*  
*  Copyright	© Winston Cutshall, 2015
*  License	BSD license, all text above must be included in any redistribution
*/


/*
These are the typical pin connections for using the BigBoard

Ethernet | Arduino | Assignment
    1         13      Clock - SCK - SHCP
    2         11      Data - MOSI - DS
    3         9       Digit D  X---  Thousands
    4         6       Digit A  ---X  Units
    5         8       Digit C  -X--  Hundreds
    6         7       Digit B  --X-  Tens
    7         +5      VCC
    8         GND     GND
*/

// include these two libraries to use the BigBoard
#include <SPI.h>
#include <BigBoard.h>

/*
 * Create an instance of the BigBoard
 * Setup variables are as follows:
 * DigitA = pin number for the rightmost digit - the units - index zero
 * DigitB = pin number for the tens - index one
 * DigitC = pin number for the hundreds - index two
 * DigitD = pin number for the thousands - index three
 * clearZero = boolean setting to remove leading zeros from numbers
*/

BigBoard tally (6, 7, 8, 9, true);

void setup() {

  // Initialize the instance
  tally.begin();

}// END SETUP

void loop() {

  // ******** The most basic and difficult way to display something ************//
  // display any character (location, value)
  // locations are indexed from the right and start at zero: A=0, B=1, C=2, D=3
  tally.displayChar (0, 117);  // 117 is "u" in ASCII
  delay (1000);

  // ******** Methods to display text ************//
  // NOTE: 4 characters or less

  // instead of coding each letter, just us a string
  // HINT:  USE ALL CAPS (all lower case characters will be converted to capitals)
  tally.displayWord ("LOOK");
  delay (1000);

  // and then get fancy
  tally.spinWord ("GOOD");
  delay (1000);

  // clears the display, but the Decimal Point (DP) channels are unchanged.
  tally.clear();
  delay (1000);

  // ******** Methods to display numbers ************//
  // NOTE: range is -999 to 9999

  // you could just give it a number
  tally.displayNum (1234);
  delay (1000);

  // or your could get fancy and spin the numbers
  // this one will spin clockwise because you're increasing from the previous value
  tally.spinNum (5678);
  delay (1000);

  // this one spins counterclockwise because it is lower than the previous number
  tally.spinNum (0);
  delay (1000);

  // clear everything - the display and DP's will all be off
  tally.reset();
  delay (1000);

  // ******** Methods to do visual effects ************//

  // spins all selected digits clockwise (seg D, seg C, seg B, seg A, reps, speed - lower is faster)
  tally.spinF (true, true, true, true, 10, 25);

  // spins all selected digits counterclockwise (seg D, seg C, seg B, seg A, reps, speed - lower is faster)
  tally.spinB (true, true, true, true, 10, 25);

  // blinks the display, not DP's.  (reps, speed - lower is faster)
  tally.flash (5, 100);

  // ******** Methods to toggle the decimal point channels ************//

  //Toggle the DP status (location, value) || locations:  DpA = 0, DpB = 1, DpC = 2, DpD = 3

  tally.setDP(0, true);
  tally.setDP(1, true);
  tally.setDP(2, true);
  tally.setDP(3, true);
  delay (1000);
  tally.setDP(0, false);
  tally.setDP(1, false);
  tally.setDP(2, false);
  tally.setDP(3, false);

  // re-sends all display data and DP data.  
  // Not required here perse, but another function to be aware of.
  tally.refresh();
  delay (1000);

  // ******** lets put it all together and make something nice for the kids************//
  tally.clear();
  delay (1000);
  tally.displayWord ("____");
  tally.flash (5, 100);
  tally.spinWord ("AC11");
  delay (500);
  tally.flash (5, 100);
  tally.spinWord ("____");
  tally.flash (5, 100);
  tally.clear ();
  delay (1000);


}// END LOOP


/*
Here are the numbers to use to call a letter with the displayChar function:

Character   Number
    0       0 or 48
    1       1 or 49
    2       2 or 50
    3       3 or 51
    4       4 or 52
    5       5 or 53
    6       6 or 54
    7       7 or 55
    8       8 or 56
    9       9 or 57
Segment A   20
Segment B   21
Segment C   22
Segment D   23
Segment E   24
Segment F   25
Segment G   26
 (blank)    36
    -       45
    A       65 or 97
    B       66 or 98
    C       67 or 99
    D       68 or 100
    E       69 or 101
    F       70 or 102
    G       71 or 103
    H       72 or 104
    I       73 or 105
    J       74 or 106
    K       75 or 107
    L       76 or 108
    M       77 or 109
    N       78 or 110
    O       79 or 111
    P       80 or 112
    Q       81 or 113
    R       82 or 114
    S       83 or 115
    T       84 or 116
    U       85 or 117
    V       86 or 118
    W       87 or 119
    X       88 or 120
    Y       89 or 121
    Z       90 or 122
    _       95
*/
