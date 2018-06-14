////////////////////////////////////////////////////////////////////////////
////        (C) Copyright 1996,2011 Custom Computer Services            ////
//// This source code may only be used by licensed users of the CCS C   ////
//// compiler.  This source code may only be distributed to other       ////
//// licensed users of the CCS C compiler.  No other use, reproduction  ////
//// or distribution is permitted without written permission.           ////
//// Derivative programs created using this software in object code     ////
//// form are not restricted in any way.                                ////
////////////////////////////////////////////////////////////////////////////
////                                                                    ////
//// History:                                                           ////
////  * 9/20/2001 :  Improvments are made to sin/cos code.              ////
////                 The code now is small, much faster,                ////
////                 and more accurate.                                 ////
////  * 2/21/2007 :  Compiler handles & operator differently and does   ////
////                 not return generic (int8 *) so type cast is done   ////
////  * 6/19/2010 :  Divisions by constants converted to multiplication ////
////                 by its inverse to improve computation speed        ////
////  * 1/21/2011 :  Constants used for double precision math updated   ////
////                 Updated routines include 64 bit versions of cos,   ////
////                 asin,atan,exp,log and dependant functions          ////
////  * 1/31/2011 :  Optimized current 32-bit float routines for cos    ////
////                 and log function                                   ////
////  * 6/13/2011 :  Fixed PCD overload CEIL_FLOOR functions for values ////
////                 greater then 10000000 and less then -10000000      ////
////                                                                    ////
////////////////////////////////////////////////////////////////////////////

#ifndef MATH_H
#define MATH_H

#ifdef PI
#undef  PI
#endif
#define PI     3.1415926535897932


#define SQRT2  1.4142135623730950

//float const ps[4] = {5.9304945, 21.125224, 8.9403076, 0.29730279};
//float const qs[4] = {1.0000000, 15.035723, 17.764134, 2.4934718};

///////////////////////////// Round Functions //////////////////////////////

float32 CEIL_FLOOR(float32 x, unsigned int8 n)
{
   float32 y, res;
   unsigned int16 l;
   int1 s;

   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y <= 32768.0)
      res = (float32)(unsigned int16)y;

 else if (y < 10000000.0)
   { 
      l = (unsigned int16)(y*0.000030517578125);
      y = 32768.0*(y*0.000030517578125 - (float32)l);
      res = 32768.0*(float32)l;
      res += (float32)(unsigned int16)y;
   }

 else
  res = y;

 y = y - (float32)(unsigned int16)y;

 if (s)
  res = -res;

 if (y != 0)
 {
  if (s == 1 && n == 0)
   res -= 1.0;

  if (s == 0 && n == 1)
   res += 1.0;
 }
 if (x == 0)
    res = 0;

 return (res);
}

// Overloaded Functions to take care for new Data types in PCD
// Overloaded function CEIL_FLOOR() for data type - Float48
#if defined(__PCD__)
float48 CEIL_FLOOR(float48 x, unsigned int8 n)
{
   float48 y, res;
   unsigned int32 l;
   int1 s;

   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y <= 32768.0)
      res = (float48)(unsigned int16)y;

   else if (y < 549755813888.0)
   {
      l = (unsigned int32)(y*0.000030517578125);
      y = 32768.0*(y*0.000030517578125 - (float48)l);
      res = 32768.0*(float48)l;
      res += (float48)(unsigned int16)y;
   }
   else
   {  
      res = y;
      y = 0.0;
   }
   
   if(y != 0)
      y = y - (float48)(unsigned int16)y;

   if (s)
      res = -res;

   if (y != 0)
   {
      if (s == 1 && n == 0)
         res -= 1.0;

      if (s == 0 && n == 1)
         res += 1.0;
   }
   if (x == 0)
      res = 0;

   return (res);
}


// Overloaded function CEIL_FLOOR() for data type - Float64
float64 CEIL_FLOOR(float64 x, unsigned int8 n)
{
   float64 y, res;
   int64 l;
   int1 s;

   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y <= 32768.0)
      res = (float64)(unsigned int16)y;
   else if (y < 4503599627370496.0)
   {
      l = (int64)(y*0.000030517578125);
      y = 32768.0*(y*0.000030517578125 - (float64)l);
      res = 32768.0*(float64)l;
      res += (float64)(unsigned int16)y;
   }
   else
   {
      res = y;
      y = 0.0;
   }
   
   if(y != 0)
      y = y - (float64)(unsigned int16)y;

   if (s)
      res = -res;

   if (y != 0)
   {
      if (s == 1 && n == 0)
         res -= 1.0;

      if (s == 0 && n == 1)
         res += 1.0;
   }
   
   if (x == 0)
      res = 0;

   return (res);
}
#endif

////////////////////////////////////////////////////////////////////////////
//   float floor(float x)
////////////////////////////////////////////////////////////////////////////
// Description : rounds down the number x.
// Date : N/A
//
float32 floor(float32 x)
{
   return CEIL_FLOOR(x,0);
}
// Following 2 functions are overloaded functions of floor() for PCD
// Overloaded function floor() for data type - Float48
#if defined(__PCD__)
float48 floor(float48 x)
{
   return CEIL_FLOOR(x, 0);
}

// Overloaded function floor() for data type - Float64
float64 floor(float64 x)
{
   return CEIL_FLOOR(x, 0);
}
#endif


////////////////////////////////////////////////////////////////////////////
//   float ceil(float x)
////////////////////////////////////////////////////////////////////////////
// Description : rounds up the number x.
// Date : N/A
//
float32 ceil(float32 x)
{
   return CEIL_FLOOR(x, 1);
}
// Following 2 functions are overloaded functions of ceil() for PCD
// Overloaded function ceil() for data type - Float48
#if defined(__PCD__)
float48 ceil(float48 x)
{
   return CEIL_FLOOR(x, 1);
}

// Overloaded function ceil() for data type - Float64
float64 ceil(float64 x)
{
   return CEIL_FLOOR(x, 1);
}
#endif

 ////////////////////////////////////////////////////////////////////////////
//   float fabs(float x)
////////////////////////////////////////////////////////////////////////////
// Description : Computes the absolute value of floating point number x
// Returns : returns the absolute value of x
// Date : N/A
//
#define fabs abs

////////////////////////////////////////////////////////////////////////////
//   float fmod(float x)
////////////////////////////////////////////////////////////////////////////
// Description : Computes the floating point remainder of x/y
// Returns : returns the value of x= i*y, for some integer i such that, if y
// is non zero, the result has the same isgn of x na dmagnitude less than the
// magnitude of y. If y is zero then a domain error occurs.
// Date : N/A
//

float fmod(float32 x,float32 y)
{
   float32 i;
   if (y!=0.0)
   {
      i=(x/y < 0.0)? ceil(x/y): floor(x/y);
      return(x-(i*y));
   }
   else
   {
   #ifdef _ERRNO
   {
      errno=EDOM;
   }
   #endif
   }
}
//Overloaded function for fmod() for PCD
// Overloaded function fmod() for data type - Float48
#if defined(__PCD__)
float48 fmod(float48 x,float48 y)
{
   float48 i;
   if (y!=0.0)
   {
      i=(x/y < 0.0)? ceil(x/y): floor(x/y);
      return(x-(i*y));
   }
   else
   {
   #ifdef _ERRNO
   {
      errno=EDOM;
   }
   #endif
   }
}
// Overloaded function fmod() for data type - Float64
float64 fmod(float64 x,float64 y)
{
   float64 i;
   if (y!=0.0)
   {
      i=(x/y < 0.0)? ceil(x/y): floor(x/y);
      return(x-(i*y));
   }
   else
   {
   #ifdef _ERRNO
   {
      errno=EDOM;
   }
   #endif
   }
}
#endif
//////////////////// Exponential and logarithmic functions ////////////////////
////////////////////////////////////////////////////////////////////////////
//   float exp(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the value (e^x)
// Date : N/A
//
#define LN2 0.6931471805599453
#define LN2_INV 1.4426950408889634073

float const pe[6] = {0.000207455774, 0.00127100575, 0.00965065093,
                     0.0554965651,  0.240227138,  0.693147172};


float32 exp(float32 x)
{
   float32 y, res, r;
   #if defined(__PCD__)
   int8 data1;
   #endif
   signed int8 n;
   int1 s;
   #ifdef _ERRNO
   if(x > 88.722838)
   {
      errno=ERANGE;
      return(0);
   }
   #endif
   n = (signed int16)(x*LN2_INV);
   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      n = -n;
      y = -y;
   }

   res = 0.0;
#if !defined(__PCD__)
   *((unsigned int8 *)(&res)) = n + 0x7F;
#endif

#if defined(__PCD__)  // Takes care of IEEE format for PCD
   data1 = n+0x7F;
   if(bit_test(data1,0))
   bit_set(*(((unsigned int8 *)(&res)+2)),7);
   rotate_right(&data1,1);
   bit_clear(data1,7);
   *(((unsigned int8 *)(&res)+3)) = data1;
#endif

   y = y*LN2_INV - (float32)n;

   r = pe[0]*y + pe[1];
   r = r*y + pe[2];
   r = r*y + pe[3];
   r = r*y + pe[4];
   r = r*y + pe[5];

   res = res*(1.0 + y*r);

   if (s)
      res = 1.0/res;
   return(res);
}


//Overloaded function for exp() for PCD
// Overloaded function exp() for data type - Float48
#if defined(__PCD__)
float48 exp(float48 x)
{
   float48 y, res, r;
   int8 data1;
   signed int8 n;
   int1 s;
   #ifdef _ERRNO
   if(x > 88.722838)
   {
      errno=ERANGE;
      return(0);
   }
   #endif
   n = (signed int16)(x*LN2_INV);
   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      n = -n;
      y = -y;
   }

   res = 0.0;

   data1 = n+0x7F;
   if(bit_test(data1,0))
   bit_set(*(((unsigned int8 *)(&res)+4)),7);
   rotate_right(&data1,1);
   bit_clear(data1,7);
   *(((unsigned int8 *)(&res)+5)) = data1;

   y = y*LN2_INV - (float48)n;

   r = pe[0]*y + pe[1];
   r = r*y + pe[2];
   r = r*y + pe[3];
   r = r*y + pe[4];
   r = r*y + pe[5];

   res = res*(1.0 + y*r);

   if (s)
      res = 1.0/res;
   return(res);
}

// Overloaded function exp() for data type - Float64
float64 const pe_64[12] ={9.30741400474913e-011,-4.28655416283316e-011,
                          8.71486547014137e-009,9.84458531538385e-008,
                          1.32588296983536e-006,1.52489283823016e-005,
                          0.000154037598423921,0.00133335487036216,
                          0.00961812936407326,0.0555041086222122,
                          0.240226506962827,0.693147180559823};

float64 exp(float64 x)
{
   float64 y, res, r;
   unsigned int16 data1, data2;
   unsigned int16 *p;
   signed int16 n;
   int1 s;
   #ifdef _ERRNO
   if(x > 709.7827128)
   {
      errno=ERANGE;
      return(0);
   }
   #endif
   y = x*LN2_INV;
   n = (signed int16)y;
   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      n = -n;
      y = -y;
   }

   res = 0.0;

#if !defined(__PCD__)
   *((unsigned int16 *)(&res)) = n + 0x7F;
#endif
   p= (((unsigned int16 *)(&res))+3);
   data1 = *p;
   data2 = *p;   
   data1 = n + 0x3FF;
   data1 = data1 <<4;
   if(bit_test(data2,15))
   bit_set(data1,15);
   data2 = data2 & 0x000F;
   data1 ^= data2;

   *(((unsigned int16 *)(&res)+3)) = data1;


   y = y*LN2_INV - (float64)n;

   r = pe_64[0]*y + pe_64[1];
   r = r*y + pe_64[2];
   r = r*y + pe_64[3];
   r = r*y + pe_64[4];
   r = r*y + pe_64[5];
   r = r*y + pe_64[6];
   r = r*y + pe_64[7];
   r = r*y + pe_64[8];
   r = r*y + pe_64[9];
   r = r*y + pe_64[10];
   r = r*y + pe_64[11];
   
   res = res*(1.0 + y*r);

   if (s)
      res = 1.0/res;
   return(res);
}
#ENDIF


/************************************************************/

float32 const pl[4] = {-1.080310025160955, 1.999999947089217};
float32 const ql[4] = {0.091284365719509, -0.873491916557671};

////////////////////////////////////////////////////////////////////////////
//   float log(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the the natural log of x
// Date : N/A
//
float32 log(float32 x)
{
   float32 y, res, r, y2;
   #if defined(__PCD__)
   unsigned int8  data1,data2;
   #endif
   signed int8 n;
   #ifdef _ERRNO
   if(x <0)
   {
      errno=EDOM;
   }
   if(x ==0)
   {
      errno=ERANGE;
      return(0);
   }
   #endif
   y = x;

   if (y != 1.0)
   {
#if !defined(__PCD__)
    *((unsigned int8 *)(&y)) = 0x7E; 
#endif

#if defined(__PCD__) // Takes care of IEEE format
   data2 = *(((unsigned int8 *)(&y))+3);
   *(((unsigned int8 *)(&y))+3) = 0x3F;
   data1 = *(((unsigned int8 *)(&y))+2);
   bit_clear(data1,7);
   *(((unsigned int8 *)(&y))+2) = data1;
   if(bit_test(data2,7))
     bit_set(*(((unsigned int8 *)(&y))+3),7);
#endif

      y = (y - 1.0)/(y + 1.0);

      y2=y*y;

      res = pl[0]*y2 + pl[1];

      r = ql[0]*y2 + ql[1];
      r = r*y2 + 1.0;

      res = y*res/r;
#if !defined(__PCD__)
      n = *((unsigned int8 *)(&x)) - 0x7E;
#endif
#if defined(__PCD__) 
    data1 = *(((unsigned int8 *)(&x)+3));
    rotate_left(&data1,1);
    data2 = *(((unsigned int8 *)(&x)+2));
    if(bit_test (data2,7))
      bit_set(data1,0);
    n = data1 - 0x7E;
#endif

      if (n<0) 
         r = -(float32)-n;
      else
         r = (float32)n;

      res += r*LN2;
   }

   else
      res = 0.0;

   return(res);
}

//Overloaded function for log() for PCD
// Overloaded function log() for data type - Float48
#if defined(__PCD__)
// Overloaded function log() for data type - float64
float64 const pl_64[4] = {-0.145694052150681, 1.365485300000370,
                          -3.120535668274329, 2.000000000000000};

float64 const ql_64[4] = {0.006047500465908, -0.208298281937234,
                          1.113943039156721, -1.893601167470470};

float48 log(float48 x)
{
   float48 y, res, r, y2;
   unsigned int8  data1,data2;
   signed int8 n;
#ifdef _ERRNO
   if(x <0)
   {
      errno=EDOM;
   }
   if(x ==0)
   {
      errno=ERANGE;
      return(0);
   }
#endif
   y = x;

   if (y != 1.0)
   {

#if !defined(__PCD__)
      *((unsigned int8 *)(&y)) = 0x7E; 
#endif
      data2 = *(((unsigned int8 *)(&y))+5);
      *(((unsigned int8 *)(&y))+5) = 0x3F;
      data1 = *(((unsigned int8 *)(&y))+4);
      bit_clear(data1,7);
      *(((unsigned int8 *)(&y))+4) = data1;

      if(bit_test(data2,7))
      bit_set(*(((unsigned int8 *)(&y))+4),7);
      y = (y - 1.0)/(y + 1.0);

      y2=y*y;

      res = pl_64[0]*y2 + pl_64[1];
      res = res*y2 + pl_64[2];
      res = res*y2 + pl_64[3];

      r = ql_64[0]*y2 + ql_64[1];
      r = r*y2 + ql_64[2];
      r = r*y2 + ql_64[3];
      r = r*y2 + 1.0; 
      res = y*res/r;

      data1 = *(((unsigned int8 *)(&x)+5));
      rotate_left(&data1,1);
      data2 = *(((unsigned int8 *)(&x)+4));
      if(bit_test (data2,7))
      bit_set(data1,0);
      
      n = data1 - 0x7E;

      if (n<0)
      r = -(float48)-n;
      else
      r = (float48)n;

      res += r*LN2;
   }

   else
   res = 0.0;

   return(res);
}

float64 log(float64 x)
{
   float64 y, res, r, y2;
   unsigned int16  data1,data2;
   unsigned int16 *p;
   signed int16 n;
   #ifdef _ERRNO
   if(x <0)
   {
      errno=EDOM;
   }
   if(x ==0)
   {
      errno=ERANGE;
      return(0);
   }
   #endif
   y = x;

   if (y != 1.0)
   {
   #if !defined(__PCD__)
       *((unsigned int8 *)(&y)) = 0x7E; 
   #endif
      p= (((unsigned int16 *)(&y))+3);
      data1 = *p;
      data2 = *p;
      data1 = 0x3FE;
      data1 = data1 <<4;
      if(bit_test (data2,15))
         bit_set(data1,15);
      data2 = data2 & 0x000F;
      data1 ^=data2;

      *p = data1;

      y = (y - 1.0)/(y + 1.0);

      y2=y*y;

      res = pl_64[0]*y2 + pl_64[1];
      res = res*y2 + pl_64[2];
      res = res*y2 + pl_64[3];

      r = ql_64[0]*y2 + ql_64[1];
      r = r*y2 + ql_64[2];
      r = r*y2 + ql_64[3];
      r = r*y2 + 1.0;

      res = y*res/r;
 
      p= (((unsigned int16 *)(&x))+3);
      data1 = *p;
      bit_clear(data1,15);
      data1 = data1 >>4;    
      n = data1 - 0x3FE;


      if (n<0)
         r = -(float64)-n;
      else
         r = (float64)n;

      res += r*LN2;
   }

   else
      res = 0.0;

   return(res);
}
#endif


#define LN10 2.3025850929940456
#define LN10_INV 0.4342944819032518276

////////////////////////////////////////////////////////////////////////////
//   float log10(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the the log base 10 of x
// Date : N/A
//
float32 log10(float32 x)
{
   float32 r;

   r = log(x);
   r = r*LN10_INV;
   return(r);
}

//Overloaded functions for log10() for PCD
// Overloaded function log10() for data type - Float48
#if defined(__PCD__)
float48 log10(float48 x)
{
   float48 r;

   r = log(x);
   r = r*LN10_INV;
   return(r);
}

// Overloaded function log10() for data type - Float64
float64 log10(float64 x)
{
   float64 r;

   r = log(x);
   r = r*LN10_INV;
   return(r);
}
#endif
////////////////////////////////////////////////////////////////////////////
//   float modf(float x)
////////////////////////////////////////////////////////////////////////////
// Description :breaks the argument value int integral and fractional parts,
// ach of which have the same sign as the argument.  It stores the integral part
// as a float in the object pointed to by the iptr
// Returns : returns the signed fractional part of value.
// Date : N/A
//

float32 modf(float32 value,float32 *iptr)
{
   *iptr=(value < 0.0)? ceil(value): floor(value);
   return(value - *iptr);
}
//Overloaded functions for modf() for PCD
// Overloaded function modf() for data type - Float48
#if defined(__PCD__)
float48 modf(float48 value,float48 *iptr)
{
   *iptr=(value < 0.0)? ceil(value): floor(value);
   return(value - *iptr);
}
// Overloaded function modf() for data type - Float64
float64 modf(float64 value,float64 *iptr)
{
   *iptr=(value < 0.0)? ceil(value): floor(value);
   return(value - *iptr);
}
#endif

////////////////////////////////////////////////////////////////////////////
//   float pwr(float x,float y)
////////////////////////////////////////////////////////////////////////////
// Description : returns the value (x^y)
// Date : N/A
// Note : 0 is returned when the function will generate an imaginary number
//
float32 pwr(float32 x,float32 y)
{
   if(0 > x && fmod(y, 1) == 0) {
      if(fmod(y, 2) == 0) {
         return (exp(log(-x) * y));
      } else {
         return (-exp(log(-x) * y));
      }
   } else if(0 > x && fmod(y, 1) != 0) {
      return 0;
   } else {
      if(x != 0 || 0 >= y) {
         return (exp(log(x) * y));
      }
   }
}
//Overloaded functions for pwr() for PCD
// Overloaded function pwr() for data type - Float48
#if defined(__PCD__)
float48 pwr(float48 x,float48 y)
{
   if(0 > x && fmod(y, 1) == 0) {
      if(fmod(y, 2) == 0) {
         return (exp(log(-x) * y));
      } else {
         return (-exp(log(-x) * y));
      }
   } else if(0 > x && fmod(y, 1) != 0) {
      return 0;
   } else {
      if(x != 0 || 0 >= y) {
         return (exp(log(x) * y));
      }
   }
}
// Overloaded function pwr() for data type - Float64
float64 pwr(float64 x,float64 y)
{
   if(0 > x && fmod(y, 1) == 0) {
      if(fmod(y, 2) == 0) {
         return (exp(log(-x) * y));
      } else {
         return (-exp(log(-x) * y));
      }
   } else if(0 > x && fmod(y, 1) != 0) {
      return 0;
   } else {
      if(x != 0 || 0 >= y) {
         return (exp(log(x) * y));
      }
   }
}
#endif

//////////////////// Power functions ////////////////////

////////////////////////////////////////////////////////////////////////////
//   float pow(float x,float y)
////////////////////////////////////////////////////////////////////////////
// Description : returns the value (x^y)
// Date : N/A
// Note : 0 is returned when the function will generate an imaginary number
//
float32 pow(float32 x,float32 y)
{
   if(0 > x && fmod(y, 1) == 0) {
      if(fmod(y, 2) == 0) {
         return (exp(log(-x) * y));
      } else {
         return (-exp(log(-x) * y));
      }
   } else if(0 > x && fmod(y, 1) != 0) {
      return 0;
   } else {
      if(x != 0 || 0 >= y) {
         return (exp(log(x) * y));
      }
   }
}
//Overloaded functions for pow() for PCD
// Overloaded function for pow() data type - Float48
#if defined(__PCD__)
float48 pow(float48 x,float48 y)
{
   if(0 > x && fmod(y, 1) == 0) {
      if(fmod(y, 2) == 0) {
         return (exp(log(-x) * y));
      } else {
         return (-exp(log(-x) * y));
      }
   } else if(0 > x && fmod(y, 1) != 0) {
      return 0;
   } else {
      if(x != 0 || 0 >= y) {
         return (exp(log(x) * y));
      }
   }
}

// Overloaded function pow() for data type - Float64
float64 pow(float64 x,float64 y)
{
   if(0 > x && fmod(y, 1) == 0) {
      if(fmod(y, 2) == 0) {
         return (exp(log(-x) * y));
      } else {
         return (-exp(log(-x) * y));
      }
   } else if(0 > x && fmod(y, 1) != 0) {
      return 0;
   } else {
      if(x != 0 || 0 >= y) {
         return (exp(log(x) * y));
      }
   }
}
#endif

////////////////////////////////////////////////////////////////////////////
//   float sqrt(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the square root of x
// Date : N/A
//
float32 sqrt(float32 x)
{
   float32 y, res;
   #if defined(__PCD__)
   unsigned int16 data1,data2;
   #endif
   BYTE *p;

   #ifdef _ERRNO
   if(x < 0)
   {
      errno=EDOM;
   }
   #endif

   if( x<=0.0)
      return(0.0);

   y=x;
   
   #if !defined(__PCD__)
    p=&y;
   (*p)=(BYTE)((((unsigned int16)(*p)) + 127) >> 1);
   #endif
   
   #if defined(__PCD__)
    p = (((unsigned int8 *)(&y))+3);
    data1 = *(((unsigned int8 *)(&y))+3);
    data2 = *(((unsigned int8 *)(&y))+2);
    rotate_left(&data1,1);    
    if(bit_test(data2,7))    
       bit_set(data1,0);    
    data1 = ((data1+127) >>1);
    bit_clear(data2,7);
    if(bit_test(data1,0))
       bit_set(data2,7);
    data1 = data1 >>1;
    *(((unsigned int8 *)(&y))+3) = data1;
    *(((unsigned int8 *)(&y))+2) = data2;
    
  #endif

   do {
      res=y;
      y+=(x/y);

      #if !defined(__PCD__)
     (*p)--;
   #endif
   
   #if defined(__PCD__)
    data1 = *(((unsigned int8 *)(&y))+3);
    data2 = *(((unsigned int8 *)(&y))+2);
    rotate_left(&data1,1);
    if(bit_test(data2,7))
       bit_set(data1,0);    
    data1--;
    bit_clear(data2,7);
    if(bit_test(data1,0))
       bit_set(data2,7);
    data1 = data1 >>1;
    *(((unsigned int8 *)(&y))+3) = data1;
    *(((unsigned int8 *)(&y))+2) = data2;
    
  #endif
   } while(res != y);

   return(res);
}
//Overloaded functions for sqrt() for PCD
// Overloaded function sqrt() for data type - Float48
#if defined(__PCD__)
float48 sqrt(float48 x)
{
   float48 y, res;
   unsigned int16 data1,data2;
   BYTE *p;

   #ifdef _ERRNO
   if(x < 0)
   {
      errno=EDOM;
   }
   #endif

   if( x<=0.0)
      return(0.0);

   y=x;
   
   #if !defined(__PCD__)
    p=&y;
   (*p)=(BYTE)((((unsigned int16)(*p)) + 127) >> 1);
   #endif
   
   #if defined(__PCD__)
    p = (((unsigned int8 *)(&y))+5);
    data1 = *(((unsigned int8 *)(&y))+5);
    data2 = *(((unsigned int8 *)(&y))+4);
    rotate_left(&data1,1);
    if(bit_test(data2,7))
       bit_set(data1,0);    
    data1 = ((data1+127) >>1);
    bit_clear(data2,7);
    if(bit_test(data1,0))
       bit_set(data2,7);
    data1 = data1 >>1;
    *(((unsigned int8 *)(&y))+5) = data1;
    *(((unsigned int8 *)(&y))+4) = data2;
    
  #endif

   do {
      res=y;
      y+=(x/y);

      #if !defined(__PCD__)
     (*p)--;
   #endif
   
    data1 = *(((unsigned int8 *)(&y))+5);
    data2 = *(((unsigned int8 *)(&y))+4);
    rotate_left(&data1,1);
    if(bit_test(data2,7))
       bit_set(data1,0);    
    data1--;
    bit_clear(data2,7);
    if(bit_test(data1,0))
       bit_set(data2,7);
    data1 = data1 >>1;
    *(((unsigned int8 *)(&y))+5) = data1;
    *(((unsigned int8 *)(&y))+4) = data2;
    
   } while(res != y);

   return(res);
}

// Overloaded function sqrt() for data type - Float64
float64 sqrt(float64 x)
{
   float64 y, res;
   unsigned int16 *p;
   unsigned int16 temp1,temp2;

   #ifdef _ERRNO
   if(x < 0)
   {
      errno=EDOM;
   }
   #endif

   if( x<=0.0)
      return(0.0);

   y=x;
   p= (((unsigned int16 *)(&y))+3);
   temp1 = *p;
   temp2 = *p;
   bit_clear(temp1,15);
   temp1 = (temp1>>4)+1023;
   temp1 = temp1 >> 1;
   temp1 = (temp1<<4) & 0xFFF0;
   if(bit_test(temp2,15))
      bit_set(temp1,15);
   temp2 = temp2 & 0x000F;
   temp1 ^= temp2;
   
   (*p) = temp1;
   
   do {
      res=y;
      y+=(x/y);
     temp1 = *p;
     temp2 = *p;
     bit_clear(temp1,15);
     temp1 = (temp1>>4);
     temp1--;
     temp1 = (temp1<<4) & 0xFFF0;
     if(bit_test(temp2,15))
        bit_set(temp1,15);
     temp2 = temp2 & 0x000F;
     temp1 ^= temp2;
     (*p) = temp1;

   } while(res != y);

   return(res);
}
#endif

////////////////////////////// Trig Functions //////////////////////////////
#ifdef PI_DIV_BY_TWO_INV
#undef PI_DIV_BY_TWO_INV
#endif
#define PI_DIV_BY_TWO_INV 0.63661977236758134
#ifdef PI_DIV_BY_TWO
#undef PI_DIV_BY_TWO
#endif
#define PI_DIV_BY_TWO   1.5707963267948966
#ifdef TWOBYPI
#undef TWOBYPI
#endif
#define TWOBYPI          0.6366197723675813

////////////////////////////////////////////////////////////////////////////
//   float cos(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the cosine value of the angle x, which is in radian
// Date : 9/20/2001
//
float32 cos(float32 x)
{
   float32 y, t, t2 = 1.0;
   unsigned int8 quad, i;
   float32 frac;
   float32 p[5] = {                    //by the series definition for cosine
      -0.49999999456337096,            // sum ( ( (-1)^n * x^2n )/(2n)! )
       0.04166663896921267,
      -0.00138883894522527,
       0.00002476138231734,
      -0.00000026070414770
      //-0.00000000001147,
      // 0.00000000000005
   };

   if (x < 0) x = -x;                  // absolute value of input

   quad = (unsigned int8)(x * PI_DIV_BY_TWO_INV);    // quadrant
   frac = (x * PI_DIV_BY_TWO_INV) - quad;  // fractional part of input
   quad = quad % 4;                    // quadrant (0 to 3)

   if (quad == 0 || quad == 2)
      t = frac * PI_DIV_BY_TWO;
   else if (quad == 1)
      t = (1-frac) * PI_DIV_BY_TWO;
   else // should be 3
      t = (frac-1) * PI_DIV_BY_TWO;

   y = 1.0;
   t = t * t;
   for (i = 0; i <= 4; i++)
   {
      t2 = t2 * t;
      y = y + p[i] * t2;
   }

   if (quad == 2 || quad == 1)
      y = -y;  // correct sign

   return (y);
}


//Overloaded functions for cos() for PCD
// Overloaded function cos() for data type - Float48
#if defined(__PCD__)
float48 cos(float48 x)
{
   float48 y, t, t2 = 1.0;
   unsigned int8 quad, i;
   float48 frac;
   float48 p[6] = {                    //by the series definition for cosine
      -0.5,                            // sum ( ( (-1)^n * x^2n )/(2n)! )
       0.04166666666667,
      -0.00138888888889,
       0.00002480158730,
      -0.00000027557319,
       0.00000000208767,
      //-0.00000000001147,
      // 0.00000000000005
   };

   if (x < 0) x = -x;                  // absolute value of input

   quad = (unsigned int8)(x * PI_DIV_BY_TWO_INV);    // quadrant
   frac = (x * PI_DIV_BY_TWO_INV) - quad;  // fractional part of input
   quad = quad % 4;                    // quadrant (0 to 3)

   if (quad == 0 || quad == 2)
      t = frac * PI_DIV_BY_TWO;
   else if (quad == 1)
      t = (1-frac) * PI_DIV_BY_TWO;
   else // should be 3
      t = (frac-1) * PI_DIV_BY_TWO;

   y = 0.999999999781;
   t = t * t;
   for (i = 0; i <= 5; i++)
   {
      t2 = t2 * t;
      y = y + p[i] * t2;
   }

   if (quad == 2 || quad == 1)
      y = -y;  // correct sign

   return (y);
}

// Overloaded function cos() for data type - Float64
float64 cos(float64 x)
{
   float64 y, t, t2 = 1.0;
   unsigned int8 quad, i;
   float64 frac;
   float64 p_64[9] = {               //by the series definition for cosine
     -0.49999999999998740,           // sum ( ( (-1)^n * x^2n )/(2n)! )             
      0.04166666666656518,
     -0.00138888888851691,
      0.00002480158658490,
     -0.00000027557239796,
      0.00000000208715031,
     -0.00000000001126577,
      0.00000000000000427,
      0.00000000000000372};

   if (x < 0) x = -x;                  // absolute value of input

   quad = (unsigned int8)(x * PI_DIV_BY_TWO_INV);    // quadrant
   frac = (x * PI_DIV_BY_TWO_INV) - quad;  // fractional part of input
   quad = quad % 4;                    // quadrant (0 to 3)

   if (quad == 0 || quad == 2)
      t = frac * PI_DIV_BY_TWO;
   else if (quad == 1)
      t = (1-frac) * PI_DIV_BY_TWO;
   else // should be 3
      t = (frac-1) * PI_DIV_BY_TWO;

   y = 1.0;
   t = t * t;
   for (i = 0; i <= 8; i++)
   {
      t2 = t2 * t;
      y = y + p_64[i] * t2;
   }

   if (quad == 2 || quad == 1)
      y = -y;  // correct sign

   return (y);
}

#endif

////////////////////////////////////////////////////////////////////////////
//   float sin(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the sine value of the angle x, which is in radian
// Date : 9/20/2001
//
float32 sin(float32 x)
{
   return cos(x - PI_DIV_BY_TWO);
}

//Overloaded functions for sin() for PCD
// Overloaded function sin() for data type - Float48
#if defined(__PCD__)
float48 sin(float48 x)
{
   return cos(x - PI_DIV_BY_TWO);
}

// Overloaded function sin() for data type - Float48
float64 sin(float64 x)
{
   return cos(x - PI_DIV_BY_TWO);
}
#endif

////////////////////////////////////////////////////////////////////////////
//   float tan(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the tangent value of the angle x, which is in radian
// Date : 9/20/2001
//
float32 tan(float32 x)
{
   float32 c, s;

   c = cos(x);
   if (c == 0.0)
      return (1.0e+36);

   s = sin(x);
   return(s/c);
}
//Overloaded functions for tan() for PCD
// Overloaded function tan() for data type - Float48
#if defined(__PCD__)
float48 tan(float48 x)
{
   float48 c, s;

   c = cos(x);
   if (c == 0.0)
      return (1.0e+36);

   s = sin(x);
   return(s/c);
}

// Overloaded function tan() for data type - Float48
float64 tan(float64 x)
{
   float64 c, s;

   c = cos(x);
   if (c == 0.0)
      return (1.0e+36);

   s = sin(x);
   return(s/c);
}
#endif

float32 const pas[3] = {0.49559947, -4.6145309, 5.6036290};
float32 const qas[3] = {1.0000000,  -5.5484666, 5.6036290};

float32 ASIN_COS(float32 x, unsigned int8 n)
{
   float32 y, res, r, y2;
   int1 s;
   #ifdef _ERRNO
   if(x <-1 || x > 1)
   {
      errno=EDOM;
   }
   #endif
   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y > 0.5)
   {
      y = sqrt((1.0 - y)*0.5);
      n += 2;
   }

   y2=y*y;

   res = pas[0]*y2 + pas[1];
   res = res*y2 + pas[2];

   r = qas[0]*y2 + qas[1];
   r = r*y2 + qas[2];

   res = y*res/r;

   if (n & 2)     // |x| > 0.5
      res = PI_DIV_BY_TWO - 2.0*res;
   if (s)
      res = -res;
   if (n & 1)           // take arccos
      res = PI_DIV_BY_TWO - res;

   return(res);
}


//Overloaded functions for ASIN_COS() for PCD
// Overloaded function ASIN_COS() for data type - Float48
#if defined(__PCD__)
float48 ASIN_COS(float48 x, unsigned int8 n)
{
   float48 y, res, r, y2;
   int1 s;
   #ifdef _ERRNO
   if(x <-1 || x > 1)
   {
      errno=EDOM;
   }
   #endif
   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y > 0.5)
   {
      y = sqrt((1.0 - y)*0.5);
      n += 2;
   }

   y2=y*y;

   res = pas[0]*y2 + pas[1];
   res = res*y2 + pas[2];

   r = qas[0]*y2 + qas[1];
   r = r*y2 + qas[2];

   res = y*res/r;

   if (n & 2)     // |x| > 0.5
      res = PI_DIV_BY_TWO - 2.0*res;
   if (s)
      res = -res;
   if (n & 1)           // take arccos
      res = PI_DIV_BY_TWO - res;

   return(res);
}

// Overloaded function ASIN_COS() for data type - Float64
double pas_64[5]={1.000000000000000,-2.069938587724477,1.421444580548907,
                 -0.360690137621842,0.024929451660228};

double qas_64[5]={-2.236605254391134,1.719212122946451,-0.524122954476133,
                   0.052808872045749,-0.000662536036570};
                   
float64 ASIN_COS(float64 x, unsigned int8 n)
{
   float64 y, res, r, y2;
   int1 s;
   #ifdef _ERRNO
   if(x <-1 || x > 1)
   {
      errno=EDOM;
   }
   #endif
   s = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y > 0.5)
   {
      y = sqrt((1.0 - y)*0.5);
      n += 2;
   }

   y2=y*y;

   res = pas_64[4]*y2 + pas_64[3];
   res = res*y2 + pas_64[2];
   res = res*y2 + pas_64[1];
   res = res*y2 + pas_64[0];

   r = qas_64[4]*y2 + qas_64[3];
   r = r*y2 + qas_64[2];
   r = r*y2 + qas_64[1];
   r = r*y2 + qas_64[0];
   r = r*y2 + 1.0;

   res = y*res/r;

   if (n & 2)     // |x| > 0.5
      res = PI_DIV_BY_TWO - 2.0*res;
   if (s)
      res = -res;
   if (n & 1)           // take arccos
      res = PI_DIV_BY_TWO - res;

   return(res);
}
#endif

////////////////////////////////////////////////////////////////////////////
//   float asin(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the arcsine value of the value x.
// Date : N/A
//
float32 asin(float32 x)
{
   float32 r;

   r = ASIN_COS(x, 0);
   return(r);
}
//Overloaded functions for asin() for PCD
// Overloaded function asin() for data type - Float48
#if defined(__PCD__)
float48 asin(float48 x)
{
   float48 r;

   r = ASIN_COS(x, 0);
   return(r);
}

// Overloaded function asin() for data type - Float64
float64 asin(float64 x)
{
   float64 r;

   r = ASIN_COS(x, 0);
   return(r);
}
#endif

////////////////////////////////////////////////////////////////////////////
//   float acos(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the arccosine value of the value x.
// Date : N/A
//
float32 acos(float32 x)
{
   float32 r;

   r = ASIN_COS(x, 1);
   return(r);
}
//Overloaded functions for acos() for PCD
// Overloaded function acos() for data type - Float48
#if defined(__PCD__)
float48 acos(float48 x)
{
   float48 r;

   r = ASIN_COS(x, 1);
   return(r);
}

// Overloaded function acos() for data type - Float64
float64 acos(float64 x)
{
   float64 r;

   r = ASIN_COS(x, 1);
   return(r);
}
#endif

float32 const pat[4] = {0.17630401, 5.6710795, 22.376096, 19.818457};
float32 const qat[4] = {1.0000000,  11.368190, 28.982246, 19.818457};

////////////////////////////////////////////////////////////////////////////
//   float atan(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the arctangent value of the value x.
// Date : N/A
//
float32 atan(float32 x)
{
   float32 y, res, r;
   int1 s, flag;

   s = 0;
   flag = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y > 1.0)
   {
      y = 1.0/y;
      flag = 1;
   }

   res = pat[0]*y*y + pat[1];
   res = res*y*y + pat[2];
   res = res*y*y + pat[3];

   r = qat[0]*y*y + qat[1];
   r = r*y*y + qat[2];
   r = r*y*y + qat[3];

   res = y*res/r;


   if (flag)                              // for |x| > 1
      res = PI_DIV_BY_TWO - res;
   if (s)
      res = -res;

   return(res);
}
//Overloaded functions for atan() for PCD
// Overloaded function atan() for data type - Float48
#if defined(__PCD__)
float48 atan(float48 x)
{
   float48 y, res, r;
   int1 s, flag;

   s = 0;
   flag = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y > 1.0)
   {
      y = 1.0/y;
      flag = 1;
   }

   res = pat[0]*y*y + pat[1];
   res = res*y*y + pat[2];
   res = res*y*y + pat[3];

   r = qat[0]*y*y + qat[1];
   r = r*y*y + qat[2];
   r = r*y*y + qat[3];

   res = y*res/r;


   if (flag)                              // for |x| > 1
      res = PI_DIV_BY_TWO - res;
   if (s)
      res = -res;

   return(res);
}

float64 pat_64[6]={0.999999999999999,2.249923645595566,1.771541617806449,
                   0.579678874003185,0.072162702162882,0.002281100141660};

float64 qat_64[6]={2.583256978928510,2.432627277466967,1.016760379885393,
                   0.182482977188688,0.011155377971690,0.000100013019160};
// Overloaded function atan() for data type - Float64
float64 atan(float64 x)
{
   float64 y,y2, res, r;
   int1 s, flag;

   s = 0;
   flag = 0;
   y = x;

   if (x < 0)
   {
      s = 1;
      y = -y;
   }

   if (y > 1.0)
   {
      y = 1.0/y;
      flag = 1;
   }
   y2 = y*y;
   res = pat_64[5]*y2 + pat_64[4];
   res = res*y2 + pat_64[3];
   res = res*y2 + pat_64[2];
   res = res*y2 + pat_64[1];
   res = res*y2 + pat_64[0];

   r = qat_64[5]*y2 + qat_64[4];
   r = r*y2 + qat_64[3];
   r = r*y2 + qat_64[2];
   r = r*y2 + qat_64[1];
   r = r*y2 + qat_64[0];
   r = r*y2 + 1.0;

   res = y*res/r;


   if (flag)                              // for |x| > 1
      res = PI_DIV_BY_TWO - res;
   if (s)
      res = -res;

   return(res);
}
#endif

/////////////////////////////////////////////////////////////////////////////
//   float atan2(float y, float x)
/////////////////////////////////////////////////////////////////////////////
// Description :computes the principal value of arc tangent of y/x, using the
// signs of both the arguments to determine the quadrant of the return value
// Returns : returns the arc tangent of y/x.
// Date : N/A
//

float32 atan2(float32 y,float32 x)
{
   float32 z;
   int1 sign;
   unsigned int8 quad;
   sign=0;
   quad=0; //quadrant
   quad=((y<=0.0)?((x<=0.0)?3:4):((x<0.0)?2:1));
   if(y<0.0)
   {
      sign=1;
      y=-y;
   }
   if(x<0.0)
   {
      x=-x;
   }
   if (x==0.0)
   {
      if(y==0.0)
      {
      #ifdef _ERRNO
      {
         errno=EDOM;
      }
      #endif
      }
      else
      {
         if(sign)
         {
         return (-(PI_DIV_BY_TWO));
         }
         else
         {
         return (PI_DIV_BY_TWO);
         }
      }
   }
   else
   {
      z=y/x;
      switch(quad)
      {
         case 1:
         {
            return atan(z);
            break;
         }
         case 2:
         {
//            return (atan(z)+PI_DIV_BY_TWO);  //2L3122
            return (PI-atan(z));
            break;
         }
         case 3:
         {
            return (atan(z)-PI);
            break;
         }
         case 4:
         {
            return (-atan(z));
            break;
         }
      }
   }
}

//Overloaded functions for atan2() for PCD
// Overloaded function atan2() for data type - Float48
#if defined(__PCD__)
float48 atan2(float48 y,float48 x)
{
   float48 z;
   int1 sign;
   unsigned int8 quad;
   sign=0;
   quad=0; //quadrant
   quad=((y<=0.0)?((x<=0.0)?3:4):((x<0.0)?2:1));
   if(y<0.0)
   {
      sign=1;
      y=-y;
   }
   if(x<0.0)
   {
      x=-x;
   }
   if (x==0.0)
   {
      if(y==0.0)
      {
      #ifdef _ERRNO
      {
         errno=EDOM;
      }
      #endif
      }
      else
      {
         if(sign)
         {
         return (-(PI_DIV_BY_TWO));
         }
         else
         {
         return (PI_DIV_BY_TWO);
         }
      }
   }
   else
   {
      z=y/x;
      switch(quad)
      {
         case 1:
         {
            return atan(z);
            break;
         }
         case 2:
         {
//            return (atan(z)+PI_DIV_BY_TWO);  //2L3122
            return (PI-atan(z));
            break;
         }
         case 3:
         {
            return (atan(z)-PI);
            break;
         }
         case 4:
         {
            return (-atan(z));
            break;
         }
      }
   }
}

// Overloaded function atan2() for data type - Float64
float64 atan2(float64 y,float64 x)
{
   float64 z;
   int1 sign;
   unsigned int8 quad;
   sign=0;
   quad=0; //quadrant
   quad=((y<=0.0)?((x<=0.0)?3:4):((x<0.0)?2:1));
   if(y<0.0)
   {
      sign=1;
      y=-y;
   }
   if(x<0.0)
   {
      x=-x;
   }
   if (x==0.0)
   {
      if(y==0.0)
      {
      #ifdef _ERRNO
      {
         errno=EDOM;
      }
      #endif
      }
      else
      {
         if(sign)
         {
         return (-(PI_DIV_BY_TWO));
         }
         else
         {
         return (PI_DIV_BY_TWO);
         }
      }
   }
   else
   {
      z=y/x;
      switch(quad)
      {
         case 1:
         {
            return atan(z);
            break;
         }
         case 2:
         {
//            return (atan(z)+PI_DIV_BY_TWO);  //2L3122
            return (PI-atan(z));
            break;
         }
         case 3:
         {
            return (atan(z)-PI);
            break;
         }
         case 4:
         {
            return (-atan(z));
            break;
         }
      }
   }
}
#endif

//////////////////// Hyperbolic functions ////////////////////

////////////////////////////////////////////////////////////////////////////
//   float cosh(float x)
////////////////////////////////////////////////////////////////////////////
// Description : Computes the hyperbolic cosine value of x
// Returns : returns the hyperbolic cosine value of x
// Date : N/A
//

float32 cosh(float32 x)
{
   return ((exp(x)+exp(-x))*0.5);
}
//Overloaded functions for cosh() for PCD
// Overloaded function cosh() for data type - Float48
#if defined(__PCD__)
float48 cosh(float48 x)
{
   return ((exp(x)+exp(-x))*0.5);
}

// Overloaded function cosh() for data type - Float64
float64 cosh(float64 x)
{
   return ((exp(x)+exp(-x))*0.5);
}
#endif

////////////////////////////////////////////////////////////////////////////
//   float sinh(float x)
////////////////////////////////////////////////////////////////////////////
// Description : Computes the hyperbolic sine value of x
// Returns : returns the hyperbolic sine value of x
// Date : N/A
//

float32 sinh(float32 x)
{

   return ((exp(x) - exp(-x))*0.5);
}
//Overloaded functions for sinh() for PCD
// Overloaded function sinh() for data type - Float48
#if defined(__PCD__)
float48 sinh(float48 x)
{

   return ((exp(x) - exp(-x))*0.5);
}

// Overloaded function sinh() for data type - Float48
float64 sinh(float64 x)
{

   return ((exp(x) - exp(-x))*0.5);
}
#endif

////////////////////////////////////////////////////////////////////////////
//   float tanh(float x)
////////////////////////////////////////////////////////////////////////////
// Description : Computes the hyperbolic tangent value of x
// Returns : returns the hyperbolic tangent value of x
// Date : N/A
//

float32 tanh(float32 x)
{
   return(sinh(x)/cosh(x));
}
//Overloaded functions for tanh() for PCD
// Overloaded function tanh() for data type - Float48
#if defined(__PCD__)
float48 tanh(float48 x)
{
   return(sinh(x)/cosh(x));
}

// Overloaded function tanh() for data type - Float64
float64 tanh(float64 x)
{
   return(sinh(x)/cosh(x));
}
#endif

////////////////////////////////////////////////////////////////////////////
//   float frexp(float x, signed int *exp)
////////////////////////////////////////////////////////////////////////////
// Description : breaks a floating point number into a normalized fraction and an integral
// power of 2. It stores the integer in the signed int object pointed to by exp.
// Returns : returns the value x, such that x is a double with magnitude in the interval
// [1/2,1) or zero, and value equals x times 2 raised to the power *exp.If value is zero,
// both parts of the result are zero.
// Date : N/A
//

#define LOG2 .30102999566398119521
#define LOG2_INV 3.32192809488736234787
float32 frexp(float32 x, signed int8 *exp)
{
   float32 res;
   int1 sign = 0;
   if(x == 0.0)
   {
      *exp=0;
      return (0.0);
   }
   if(x < 0.0)
   {
     x=-x;
     sign=1;
   }
   if (x > 1.0)
   {
      *exp=(ceil(log10(x)*LOG2_INV));
      res=x/(pow(2, *exp));
      if (res == 1)
      {
         *exp=*exp+1;
          res=.5;
      }
   }
   else
   {
      if(x < 0.5)
      {
         *exp=-1;
         res=x*2;
      }
      else
      {
         *exp=0;
          res=x;
      }
   }
   if(sign)
   {
      res=-res;
   }
   return res;
}

//Overloaded functions for frexp() for PCD
// Overloaded function frexp() for data type - Float48
#if defined(__PCD__)
float48 frexp(float48 x, signed int8 *exp)
{
   float48 res;
   int1 sign = 0;
   if(x == 0.0)
   {
      *exp=0;
      return (0.0);
   }
   if(x < 0.0)
   {
     x=-x;
     sign=1;
   }
   if (x > 1.0)
   {
      *exp=(ceil(log10(x)*LOG2_INV));
      res=x/(pow(2, *exp));
      if (res == 1)
      {
         *exp=*exp+1;
          res=.5;
      }
   }
   else
   {
      if(x < 0.5)
      {
         *exp=-1;
         res=x*2;
      }
      else
      {
         *exp=0;
          res=x;
      }
   }
   if(sign)
   {
      res=-res;
   }
   return res;
}

// Overloaded function frexp() for data type - Float64
float64 frexp(float64 x, signed int8 *exp)
{
   float64 res;
   int1 sign = 0;
   if(x == 0.0)
   {
      *exp=0;
      return (0.0);
   }
   if(x < 0.0)
   {
     x=-x;
     sign=1;
   }
   if (x > 1.0)
   {
      *exp=(ceil(log10(x)*LOG2_INV));
      res=x/(pow(2, *exp));
      if (res == 1)
      {
         *exp=*exp+1;
          res=.5;
      }
   }
   else
   {
      if(x < 0.5)
      {
         *exp=-1;
         res=x*2;
      }
      else
      {
         *exp=0;
          res=x;
      }
   }
   if(sign)
   {
      res=-res;
   }
   return res;
}
#endif

//////////////////////////////////////////////////////////////////////////////
//   float ldexp(float x, signed int *exp)
//////////////////////////////////////////////////////////////////////////////
// Description : multiplies a floating point number by an integral power of 2.
// Returns : returns the value of x times 2 raised to the power exp.
// Date : N/A
//

float32 ldexp(float32 value, signed int8 exp)
{
   return (value * pow(2,exp));
}
//Overloaded functions for ldexp() for PCD
// Overloaded function ldexp() for data type - Float48

#if defined(__PCD__)
float48 ldexp(float48 value, signed int8 exp)
{
   return (value * pow(2,exp));
}
// Overloaded function ldexp() for data type - Float64
float64 ldexp(float64 value, signed int8 exp)
{
   return (value * pow(2,exp));
}
#endif

#endif
