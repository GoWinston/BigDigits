/*
*  BigBoard Library
*  
*  A library for rolling your own giant seven-segment display with the BigBoard PCB.
*  Get your own board at http://example.com
*  
*  Author	Winston Cutshall
*           http://example.com
*  
*  Date		10/29/15
*  Version	0.9 Beta
*  
*  Copyright	© Winston Cutshall, 2015
*  License	BSD license, all text above must be included in any redistribution
*/

#ifndef BigBoard_h
#define BigBoard_h

#include "Arduino.h"
#include <SPI.h>


class BigBoard {

  public:

    BigBoard (byte DigitA, byte DigitB, byte DigitC, byte DigitD, boolean clearZero);
    void begin ();

    void displayChar (byte where, byte what);
    byte font (byte what);

    void displayNum (int number);
    void spinNum (int number);

    void displayWord (String TheWord);
    void spinWord (String TheWord);

    void setDP (byte where, boolean what);

    void flash (int reps, int timeDelay);
    void spinF (boolean D, boolean C, boolean B, boolean A, int reps, int rate);
    void spinB (boolean D, boolean C, boolean B, boolean A, int reps, int rate);

    void refresh ();
    void clear ();
    void reset ();

  private:

    //Setting: should leading zeros be omitted for displayed numbers?
    boolean _clearZero;

    //Identification of Arduino pins used for latches
    byte _DigitA; // ---X  Units
    byte _DigitB; // --X-  Tens
    byte _DigitC; // -X--  Hundreds
    byte _DigitD; // X---  Thousands

    //Holding Variables for Current Data Displayed
    byte _DataA;
    byte _DataB;
    byte _DataC;
    byte _DataD;

    //Holding Variables for Decimal Point Circuits
    boolean _DpA;
    boolean _DpB;
    boolean _DpC;
    boolean _DpD;

    //Holding Variable for SPI Payload
    byte _toTransfer;

    //Holding Variable for spinNum
    int _prevNum;
};

#endif
