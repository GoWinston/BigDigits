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

#include "Arduino.h"
#include "BigBoard.h"

BigBoard::BigBoard (byte DigitA, byte DigitB, byte DigitC, byte DigitD, boolean clearZero) {
  _DigitA = DigitA;
  _DigitB = DigitB;
  _DigitC = DigitC;
  _DigitD = DigitD;
  _clearZero = clearZero;
}

void BigBoard::begin () {

  SPI.begin();

  // Latch initialization
  pinMode(_DigitA, OUTPUT); // ---X  Units
  pinMode(_DigitB, OUTPUT); // --X-  Tens
  pinMode(_DigitC, OUTPUT); // -X--  Hundreds
  pinMode(_DigitD, OUTPUT); // X---  Thousands
  // Latches Set High
  digitalWrite(_DigitA, HIGH);
  digitalWrite(_DigitB, HIGH);
  digitalWrite(_DigitC, HIGH);
  digitalWrite(_DigitD, HIGH);
}




/*****   CORE FUNCTION which actually puts stuff on the board   *****/

void BigBoard::displayChar ( byte where, byte what) {
    SPI.beginTransaction(SPISettings(25000000, MSBFIRST, SPI_MODE0));
    switch (where) {
      case 0: // ---X
        digitalWrite(_DigitA, LOW);
        _DataA = font(what);
        if (_DpA) {
          bitSet(_toTransfer, 0);
        }
        SPI.transfer(_toTransfer);
        digitalWrite(_DigitA, HIGH);
        break;
      case 1: // --X-
        digitalWrite(_DigitB, LOW);
        _DataB = font(what);
        if (_DpB) {
          bitSet(_toTransfer, 0);
        }
        SPI.transfer(_toTransfer);
        digitalWrite(_DigitB, HIGH);
        break;
      case 2: // -X--
        digitalWrite(_DigitC, LOW);
        _DataC = font(what);
        if (_DpC) {
          bitSet(_toTransfer, 0);
        }
        SPI.transfer(_toTransfer);
        digitalWrite(_DigitC, HIGH);
        break;
      case 3: // X---
        digitalWrite(_DigitD, LOW);
        _DataD = font(what);
        if (_DpD) {
          bitSet(_toTransfer, 0);
        }
        SPI.transfer(_toTransfer);
        digitalWrite(_DigitD, HIGH);
        break;
    }
    SPI.endTransaction();
}



/*****   Functions for displaying numbers   *****/

void BigBoard::displayNum (int number) {
  byte payloadA;
  byte payloadB;
  byte payloadC;
  byte payloadD;
  if (number > 9999 || number < -999) {
    displayWord ("-err");
  }
  else {
    int thousands = abs(number) / 1000;
    if (number < 0 ) {
      payloadD = 45;
    }
    else if ( number > 999) {
      payloadD = thousands;
    }
    else if (_clearZero && number < 1000) {
      payloadD = 36;
    }
    else {
      payloadD = 0;
    }
    int hundreds = (abs(number) - (thousands * 1000)) / 100;
    if ( abs(number) > 99) {
      payloadC = hundreds;
    }
    else if (abs(number) < 100) {
      if (_clearZero) {
        payloadC = 36;
      }
      else {
        payloadC = 0;
      }
    }
    int tens = (abs(number) - (thousands * 1000) - (hundreds * 100)) / 10;
    if ( abs(number) > 9) {
      payloadB = tens;
    }
    else if (abs(number) < 10) {
      if (_clearZero) {
        payloadB = 36;
      }
      else {
        payloadB = 0;
      }
    }
    int units = abs (number % 10);
    payloadA = units;

    displayChar (3, payloadD);
    displayChar (2, payloadC);
    displayChar (1, payloadB);
    displayChar (0, payloadA);
    
    _prevNum = number;
  }
}


void BigBoard::spinNum (int number) {
  byte payloadA;
  byte payloadB;
  byte payloadC;
  byte payloadD;
  byte Logic; // D C B A
  boolean SpinF;
  if (number > 9999 || number < -999) {
    displayWord ("-err");
    return; //nothing more to do
  }
  int thousands = abs(number) / 1000;
  if (number < 0 ) {
    payloadD = 45;  // negative sign
  }
  else if ( number > 999) {
    payloadD = thousands;
  }
  else if (_clearZero && number < 1000) {
    payloadD = 36;
  }
  else {
    payloadD = 0;
  }
  int hundreds = (abs(number) - (thousands * 1000)) / 100;
  if ( abs(number) > 99) {
    payloadC = hundreds;
  }
  else if (abs(number) < 100) {
    if (_clearZero) {
      payloadC = 36;
    }
    else {
      payloadC = 0;
    }
  }
  int tens = (abs(number) - (thousands * 1000) - (hundreds * 100)) / 10;
  if ( abs(number) > 9) {
    payloadB = tens;
  }
  else if (abs(number) < 10) {
    if (_clearZero) {
      payloadB = 36;
    }
    else {
      payloadB = 0;
    }
  }
  int units = abs (number % 10);
  payloadA = units;

  if (thousands - (abs(_prevNum) / 1000) != 0) {
    Logic = B1111;
    if ( thousands - (abs(_prevNum) / 1000) >= 0) {
      SpinF = true;
    }
    else {
      SpinF = false;
    }
  }
  else if (hundreds - ((abs(_prevNum) - (thousands * 1000)) / 100) != 0) {
    Logic = B0111;
    if (hundreds - ((abs(_prevNum) - (thousands * 1000)) / 100) >= 0) {
      SpinF = true;
    }
    else {
      SpinF = false;
    }
  }
  else if (tens - ((abs(_prevNum) - (thousands * 1000) - (hundreds * 100)) / 10) != 0) {
    Logic = B0011;
    if (tens - ((abs(_prevNum) - (thousands * 1000) - (hundreds * 100)) / 10)  >= 0) {
      SpinF = true;
    }
    else {
      SpinF = false;
    }
  }
  else if (units - (abs(_prevNum) % 10) != 0) {
    Logic = B0001;
    if (units - (abs(_prevNum) % 10) >= 0) {
      SpinF = true;
    }
    else {
      SpinF = false;
    }
  }
  else {
    refresh ();
    return; //nothing more to do
  }
  if (SpinF) {
    for (int y = 21; y > 0; y = y - 3) {  // Spin Annimation for used spaces
      spinF (bitRead(Logic, 3), bitRead(Logic, 2), bitRead(Logic, 1), bitRead(Logic, 0), 1, y);
    }
  }
  else {
    for (int y = 21; y > 0; y = y - 3) {  // Spin Annimation for used spaces
      spinB (bitRead(Logic, 3), bitRead(Logic, 2), bitRead(Logic, 1), bitRead(Logic, 0), 1, y);
    }
  }
  displayChar (3, payloadD);
  displayChar (2, payloadC);
  displayChar (1, payloadB);
  displayChar (0, payloadA);
  _prevNum = number;
}



/*****   Functions for displaying strings   *****/

void BigBoard::displayWord (String TheWord) {
  int len = TheWord.length();
  if (len > 4 || len < 0) {
    displayWord ("-err");    ////////////////////// should be TheWord instead of recalling the function?
    return;
  }
  for (int y = 4; y > len ; y--) {
    displayChar (y, 36); // BLANK
  }
  for (int x = len; x > 0; x--) {
    char aa = TheWord.charAt(abs(x - len));
    byte A = byte (aa);
    displayChar (x - 1, A);
  }
}

void BigBoard::spinWord (String TheWord) {
  byte Logic; // D C B A
  byte len = TheWord.length();
  if (len > 4 || len < 0) {
    displayWord ("-err");
    return;
  }
  switch (len) {  // D C B A
    case 0:
      Logic = B0000;
      break;
    case 1:
      Logic = B0001;
      break;
    case 2:
      Logic = B0011;
      break;
    case 3:
      Logic = B0111;
      break;
    case 4:
      Logic = B1111;
      break;
  }
  for (int z = 0; z < 4 ; z++) {  // BLANK unused spaces
    if (!bitRead(Logic, z)) {
      displayChar (z, 36);
    }
  }
  for (int y = 21; y > 0; y = y - 3) {  // Spin Annimation for used spaces
    spinF (bitRead(Logic, 3), bitRead(Logic, 2), bitRead(Logic, 1), bitRead(Logic, 0), 1, y);
  }
  for (int x = 0; x < 4 ; x++) {  // Display values of used spaces
    if (bitRead(Logic, x)) {
      char aa = TheWord.charAt((len - 1) - x);
      byte A = byte (aa);
      displayChar (x, A);
    }
  }
}



/*****   Functions for Decimal Point manipulation   *****/

void BigBoard::setDP (byte where, boolean what) {
  switch (where) {
    case 0:
      _DpA = what;
      displayChar (where, _DataA);
      break;
    case 1:
      _DpB = what;
      displayChar (where, _DataB);
      break;
    case 2:
      _DpC = what;
      displayChar (where, _DataC);
      break;
    case 3:
      _DpD = what;
      displayChar (where, _DataD);
      break;
  }
  
}


/*****   Functions for visual effect   *****/

void BigBoard::flash (int reps, int timeDelay) {
  byte holdA = _DataA;
  byte holdB = _DataB;
  byte holdC = _DataC;
  byte holdD = _DataD;
  for (int x = 0; x < reps; x++) {
    delay (timeDelay);
    if (holdA != 36) {
      displayChar (0, 36);
    }
    if (holdB != 36) {
      displayChar (1, 36);
    }
    if (holdC != 36) {
      displayChar (2, 36);
    }
    if (holdD != 36) {
      displayChar (3, 36);
    }
    delay (timeDelay);
    if (holdA != 36) {
      displayChar (0, holdA);
    }
    if (holdB != 36) {
      displayChar (1, holdB);
    }
    if (holdC != 36) {
      displayChar (2, holdC);
    }
    if (holdD != 36) {
      displayChar (3, holdD);
    }
  }
}

void BigBoard::spinF (boolean D, boolean C, boolean B, boolean A, int reps, int rate) {
  for (int y = 0; y < reps; y++) {
    for (int x = 20; x < 26; x++) {
      if (A) {
        displayChar ( 0, x);
      }
      if (B) {
        displayChar ( 1, x);
      }
      if (C) {
        displayChar ( 2, x);
      }
      if (D) {
        displayChar ( 3, x);
      }
      delay (rate);
    }
  }
}

void BigBoard::spinB (boolean D, boolean C, boolean B, boolean A, int reps, int rate) {
  for (int y = 0; y < reps; y++) {
    for (int x = 25; x > 19; x--) {
      if (A) {
        displayChar ( 0, x);
      }
      if (B) {
        displayChar ( 1, x);
      }
      if (C) {
        displayChar ( 2, x);
      }
      if (D) {
        displayChar ( 3, x);
      }
      delay (rate);
    }
  }
}

/*****   Utility Functions   *****/

void BigBoard::refresh () {
  displayChar (0, _DataA);
  displayChar (1, _DataB);
  displayChar (2, _DataC);
  displayChar (3, _DataD);
}

void BigBoard::clear () {
  displayChar (0, 36);
  displayChar (1, 36);
  displayChar (2, 36);
  displayChar (3, 36);
  _prevNum = 0;
}

void BigBoard::reset () {
  _DpA = false;
  _DpB = false;
  _DpC = false;
  _DpD = false;
  displayChar (0, 36);
  displayChar (1, 36);
  displayChar (2, 36);
  displayChar (3, 36);
  _prevNum = 0;
}

/*****   The font, which also does some ASCII translation   *****/

byte BigBoard::font (byte what) {    // MSB Format
  switch (what) {
    case 0: // 0
      _toTransfer = 126;
      return 0;
      break;
    case 1: // 1
      _toTransfer = 12;
      return 1;
      break;
    case 2: // 2
      _toTransfer = 182;
      return 2;
      break;
    case 3: // 3
      _toTransfer = 158;
      return 3;
      break;
    case 4: // 4
      _toTransfer = 204;
      return 4;
      break;
    case 5: // 5
      _toTransfer = 218;
      return 5;
      break;
    case 6: // 6
      _toTransfer = 250;
      return 6;
      break;
    case 7: // 7
      _toTransfer = 14;
      return 7;
      break;
    case 8: // 8
      _toTransfer = 254;
      return 8;
      break;
    case 9: // 9
      _toTransfer = 222;
      return 9;
      break;

    // Special Characters

    case 20: // (Segment A)
      _toTransfer = 2;
      return 20;
      break;
    case 21: // (Segment B)
      _toTransfer = 4;
      return 21;
      break;
    case 22: // (Segment C)
      _toTransfer = 8;
      return 22;
      break;
    case 23: // (Segment D)
      _toTransfer = 16;
      return 23;
      break;
    case 24: // (Segment E)
      _toTransfer = 32;
      return 24;
      break;
    case 25: // (Segment F)
      _toTransfer = 64;
      return 25;
      break;
    case 26: // (Segment G)
      _toTransfer = 128;
      return 26;
      break;

    case 36: // (clear)
      _toTransfer = 0;
      return 36;
      break;

    case 45: // -
      _toTransfer = 128;
      return 45;
      break;

    // BIN Values for ASCII Characters

    case 48: // 0
      _toTransfer = 126;
      return 0;
      break;
    case 49: // 1
      _toTransfer = 12;
      return 1;
      break;
    case 50: // 2
      _toTransfer = 182;
      return 2;
      break;
    case 51: // 3
      _toTransfer = 158;
      return 3;
      break;
    case 52: // 4
      _toTransfer = 204;
      return 4;
      break;
    case 53: // 5
      _toTransfer = 218;
      return 5;
      break;
    case 54: // 6
      _toTransfer = 250;
      return 6;
      break;
    case 55: // 7
      _toTransfer = 14;
      return 7;
      break;
    case 56: // 8
      _toTransfer = 254;
      return 8;
      break;
    case 57: // 9
      _toTransfer = 222;
      return 9;
      break;

    case 65: // A
      _toTransfer = 238;
      return 65;
      break;
    case 66: // B
      _toTransfer = 248;
      return 66;
      break;
    case 67: // C
      _toTransfer = 114;
      return 67;
      break;
    case 68: // D
      _toTransfer = 188;
      return 68;
      break;
    case 69: // E
      _toTransfer = 242;
      return 69;
      break;
    case 70: // F
      _toTransfer = 226;
      return 70;
      break;
    case 71: // G
      _toTransfer = 122;
      return 71;
      break;
    case 72: // H
      _toTransfer = 232;
      return 72;
      break;
    case 73: // I
      _toTransfer = 96;
      return 73;
      break;
    case 74: // J
      _toTransfer = 60;
      return 74;
      break;
    case 75: // K
      _toTransfer = 234;
      return 75;
      break;
    case 76: // L
      _toTransfer = 112;
      return 76;
      break;
    case 77: // M
      _toTransfer = 42;
      return 77;
      break;
    case 78: // N
      _toTransfer = 110;
      return 78;
      break;
    case 79: // O
      _toTransfer = 126;
      return 79;
      break;
    case 80: // P
      _toTransfer = 230;
      return 80;
      break;
    case 81: // Q
      _toTransfer = 206;
      return 81;
      break;
    case 82: // R
      _toTransfer = 102;
      return 82;
      break;
    case 83: // S
      _toTransfer = 218;
      return 83;
      break;
    case 84: // T
      _toTransfer = 240;
      return 84;
      break;
    case 85: // U
      _toTransfer = 124;
      return 85;
      break;
    case 86: // V
      _toTransfer = 92;
      return 86;
      break;
    case 87: // W
      _toTransfer = 84;
      return 87;
      break;
    case 88: // X
      _toTransfer = 108;
      return 88;
      break;
    case 89: // Y
      _toTransfer = 220;
      return 89;
      break;
    case 90: // Z
      _toTransfer = 150;
      return 90;
      break;

    case 95: // _
      _toTransfer = 16;
      return 95;
      break;

    case 97: // A
      _toTransfer = 238;
      return 97;
      break;
    case 98: // B
      _toTransfer = 248;
      return 98;
      break;
    case 99: // C
      _toTransfer = 114;
      return 99;
      break;
    case 100: // D
      _toTransfer = 188;
      return 100;
      break;
    case 101: // E
      _toTransfer = 242;
      return 101;
      break;
    case 102: // F
      _toTransfer = 226;
      return 102;
      break;
    case 103: // G
      _toTransfer = 122;
      return 103;
      break;
    case 104: // H
      _toTransfer = 232;
      return 104;
      break;
    case 105: // I
      _toTransfer = 96;
      return 105;
      break;
    case 106: // J
      _toTransfer = 60;
      return 106;
      break;
    case 107: // K
      _toTransfer = 234;
      return 107;
      break;
    case 108: // L
      _toTransfer = 112;
      return 108;
      break;
    case 109: // M
      _toTransfer = 42;
      return 109;
      break;
    case 110: // N
      _toTransfer = 110;
      return 110;
      break;
    case 111: // O
      _toTransfer = 126;
      return 111;
      break;
    case 112: // P
      _toTransfer = 230;
      return 112;
      break;
    case 113: // Q
      _toTransfer = 206;
      return 113;
      break;
    case 114: // R
      _toTransfer = 102;
      return 114;
      break;
    case 115: // S
      _toTransfer = 218;
      return 115;
      break;
    case 116: // T
      _toTransfer = 240;
      return 116;
      break;
    case 117: // U
      _toTransfer = 124;
      return 117;
      break;
    case 118: // V
      _toTransfer = 92;
      return 118;
      break;
    case 119: // W
      _toTransfer = 84;
      return 119;
      break;
    case 120: // X
      _toTransfer = 108;
      return 120;
      break;
    case 121: // Y
      _toTransfer = 220;
      return 121;
      break;
    case 122: // Z
      _toTransfer = 150;
      return 122;
      break;
  }
}
