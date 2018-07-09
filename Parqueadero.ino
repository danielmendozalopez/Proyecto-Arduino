/* The MIT License (MIT)

Copyright (c) <2013> <DMA-SOURCE>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


*/
// Modified by DMA-SOURCE: Authors: Daniel Mendoza & Maldonado Lobsang
// Website: www.dma-source.com
// Based on:
// Arduino 7 segment display example software
// http://www.hacktronics.com/Tutorials/arduino-and-7-segment-led.html
// License: http://www.opensource.org/licenses/mit-license.php

// Define the LED digit patters, from 0 - 9
// Note that these patterns are for common cathode displays
// For common anode displays, change the 1's to 0's and 0's to 1's
// 1 = LED on, 0 = LED off, in this order:
                               
   #include <math.h>
   byte seven_seg_digits[10][7] = 
      {  { 1,1,1,1,1,1,0 },  // = 0
         { 0,1,1,0,0,0,0 },  // = 1
         { 1,1,0,1,1,0,1 },  // = 2
         { 1,1,1,1,0,0,1 },  // = 3
         { 0,1,1,0,0,1,1 },  // = 4
         { 1,0,1,1,0,1,1 },  // = 5
         { 1,0,1,1,1,1,1 },  // = 6
         { 1,1,1,0,0,0,0 },  // = 7
         { 1,1,1,1,1,1,1 },  // = 8
         { 1,1,1,0,0,1,1 }   // = 9
      };
   byte num[1];
   byte autoNormal = A0;
   byte autoEspecial = A1;
   byte ca_acum = A2;
   byte autoSalen = A3;
   byte punto= A4;
   byte ledfullempty=A5;
   byte readAutoNormal = 0;
   byte readAutoEspecial = 0;
   byte readCaAcum=0;
   byte readSalen=0;
   byte autoNormalState = 0;
   byte lastReadAutoNormal = 0;
   byte totalAuto = 0;
   float valorNormal=0;
   float valorEspecial=0;
   float totalDinero=0;
   byte bandera=0;
   
   void pushButtonNormal(byte autoNormal)
     {  readAutoNormal = digitalRead(autoNormal);
        if(readAutoNormal==1)
         {  totalAuto++;delay(150);valorNormal++;}
     }
   void pushButtonEspecial(byte autoEspecial)
     {  readAutoEspecial = digitalRead(autoEspecial);
        if(readAutoEspecial==1)
         { totalAuto++;delay(150); valorEspecial+=0.5;}
     }
   void pushButtonAcumEspecial(byte ca_acum)
     { readCaAcum = digitalRead(ca_acum);
       if(readCaAcum==0)
          { writeDisplay(totalAuto);delay(150);digitalWrite(punto,LOW); }
       else 
          { clearDisplay();delay(300);  digitalWrite(punto,HIGH);
            totalDinero = valorNormal+valorEspecial;writeDisplay(round(totalDinero));delay(600);clearDisplay();delay(300);
          }
     }
   void pushButtonSalen(byte autoSalen)
     { readSalen = digitalRead(autoSalen);
       if(readSalen == 1)
         { if(totalAuto >0)
            {totalAuto--;}} 
     } 
   void setup() 
    { pinMode(0, OUTPUT);   //Patilla 7 del display 1
      pinMode(1, OUTPUT);   //Patilla 6 del display 1
      pinMode(2, OUTPUT);   //Patilla 4 del display 1
      pinMode(3, OUTPUT);   //Patilla 2 del display 1
      pinMode(4, OUTPUT);   //Patilla 1 del display 1
      pinMode(5, OUTPUT);   //Patilla 9 del display 1
      pinMode(6, OUTPUT);   //Patilla 10 del display 1
      pinMode(7, OUTPUT);   //Patilla 7 del display 2
      pinMode(8, OUTPUT);   //Patilla 6 del display 2
      pinMode(9, OUTPUT);   //Patilla 4 del display 2
      pinMode(10, OUTPUT);  //Patilla 2 del display 2
      pinMode(11, OUTPUT);  //Patilla 1 del display 2
      pinMode(12, OUTPUT);  //Patilla 9 del display 2
      pinMode(13, OUTPUT);  //Patilla 10 del display 2
      pinMode(autoNormal,INPUT);
      pinMode(punto,OUTPUT);
      pinMode(ledfullempty,OUTPUT);
    }
   void sevenSegWrite(byte digit)
    { byte pin = 0;
      for (byte segCount = 0; segCount < 7; ++segCount)
        { digitalWrite(pin, seven_seg_digits[digit][segCount]); ++pin;}
    }
   void numberWrite_D1(byte digit)
    {  for (byte segCount = 0; segCount < 7; ++segCount) 
        { digitalWrite(segCount,seven_seg_digits[digit][segCount]);}
    }
   void numberWrite_D2(byte digit) 
    { for (byte segCount = 7; segCount < 14; segCount++) 
       { digitalWrite(segCount,seven_seg_digits[digit-1][segCount]); }
    }
   void nSplit(byte y) 
    { byte temp = 1;
       if (0 == y)
         { } 
       else 
         { while (y != 0) 
            { byte last =  y % 10;
              num[temp] = last;
              y = (y - last) / 10;
              temp--;
            }
         }
    }
   void writeDisplay(byte number)
    { if(number < 9)
        { numberWrite_D1(number); numberWrite_D2(0); }
      else
       { for(int i=0;i<2;i++)
          { num[i] = 0;} 
         nSplit(number);
         numberWrite_D1(num[1]);
         numberWrite_D2(num[0]); 
       }
    }
   void clearDisplay()
    { for (byte segCount = 0; segCount < 14; ++segCount)
        { digitalWrite(segCount,LOW);}
    }
   void loop()
    { pushButtonAcumEspecial(ca_acum);
      pushButtonSalen(autoSalen);
      if(totalAuto<25)
       { digitalWrite(ledfullempty,HIGH);
         pushButtonNormal(autoNormal);
         pushButtonEspecial(autoEspecial);
         delay(150);
       }
       else{digitalWrite(ledfullempty,LOW);}
    }
 
