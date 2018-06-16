
/// древнее зло на новый лад
/// в процессе %0,1

/// + MATH.H http://kazus.ru/forums/attachment.php?attachmentid=124138&d=1515955294
/// + https://github.com/xboxfanj/math-neon
/// + gcc\math.h
/// + собсвенное мнение

#ifndef MATH_s
#define MATH_s

#ifdef PI
#undef  PI
#endif
#include <stdint.h>
#define PI     3.1415927410125732421875f
//3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679



#define SQRT2  1.4142135623730950f


#define fabs abs



union float_raw
{
    struct
    {
        uint32_t massa  :23;
        uint32_t order  :8;
        uint32_t sign   :1;
    };
    uint32_t    u_raw;
    float       f_raw;
};


/// drop a fraction below zero
/// отбросить дробное значение ниже нуля
float ceil_f(float value)
{
#ifdef __CM7_REV
    float rep;
    if (value > 0.0f) asm volatile ("vrintm.f32 %0,%1" : "=t"(rep) : "t"(value));
    else asm volatile ("vrintp.f32 %0,%1" : "=t"(rep) : "t"(value));
    return rep;
#else
    union float_raw Ftemp;
    Ftemp.f_raw = value;
    int_fast8_t ord = (int_fast8_t)Ftemp.order - 127;
    if (ord < 0) return 0.0f;
    else
    {
        ord = 23 - ord;
        if(ord > 0)
        {
            Ftemp.u_raw = ((Ftemp.u_raw >>ord)<<ord);
        };
    };
    return Ftemp.f_raw;
#endif
}

/// return fractional remainder of division
/// вернуть дробный остаток деления
float fmod_f(float value, float divider)
{
   if (divider!=0.0f)
   {
       return( value - ( ceil_f(value / divider) * divider));
   } else return 0.0f;
};


//#define LN2 0.6931471805599453
#define LN2_INV 1.4426950408889634073

const float __expf_rng[2] = {
	1.442695041f,
	0.693147180f,
};

const float __expf_lut[8] = {
	0.9999999916728642f,		//p0
	0.04165989275009526f, 	//p4
	0.5000006143673624f, 	//p2
	0.0014122663401803872f, 	//p6
	1.000000059694879f, 		//p1
	0.008336936973260111f, 	//p5
	0.16666570253074878f, 	//p3
	0.00019578093328483123f,	//p7
};

/// Raising the number E to the power of a value
/// Возведение числа E в степень value
float exp_f(float value)
{
    union float_raw Ftemp;
    if ( value > 88.7228317f) {Ftemp.u_raw = 0; Ftemp.order = 0xFF; return Ftemp.f_raw;};
	float tmp1, tmp2, tmp3, tmp4, tmp5;
	int8_t ord;


	ord = (int8_t) (value * __expf_rng[0]);
	value = value - ((float) ord) * __expf_rng[1];
	tmp1 = (__expf_lut[4] * value) + (__expf_lut[0]);
	tmp2 = (__expf_lut[6] * value) + (__expf_lut[2]);
	tmp3 = (__expf_lut[5] * value) + (__expf_lut[1]);
	tmp4 = (__expf_lut[7] * value) + (__expf_lut[3]);
	tmp5 = value * value;
	tmp1 = tmp1 + tmp2 * tmp5;
	tmp3 = tmp3 + tmp4 * tmp5;
	tmp5 = tmp5 * tmp5;
	Ftemp.f_raw = tmp1 + tmp3 * tmp5;
	Ftemp.order += ord;
	return Ftemp.f_raw;
}





/************************************************************/



const float __log10f_lut[9] = {
	-0.99697286229624f, 		//p0
	-1.07301643912502f, 		//p4
	-2.46980061535534f, 		//p2
	-0.07176870463131f, 		//p6
	2.247870219989470f, 		//p1
	0.366547581117400f, 		//p5
	1.991005185100089f, 		//p3
	0.006135635201050f,         //p7
	0.3010299957f,              //rng
};

const float __log_e_f_lut[9] = {
	-2.295614848256274, 	//p0
	-2.470711633419806, 	//p4
	-5.686926051100417, 	//p2
	-0.165253547131978, 	//p6
	+5.175912446351073, 	//p1
	+0.844006986174912, 	//p5
	+4.584458825456749, 	//p3
	+0.014127821926000,		//p7
	0.693147180f,           //rng
};

/// Logarithm value for base 2,10,e
/// Логарифм value по основанию 2,10,e
float log_f(float value, int_fast8_t base_2_10_e)
{
	float tmp1, tmp2, tmp3, tmp4, tmp5;
	int8_t ord, ord1;
	union float_raw Ftemp;
	const float *_lut;
	if (base_2_10_e != 10) _lut = __log_e_f_lut; else _lut = __log10f_lut;

	Ftemp.f_raw = value;
	ord = Ftemp.order; ord1 = ord;
	ord -= 127; ord1 -= ord;
	Ftemp.order = ord1;

	tmp5= Ftemp.f_raw * Ftemp.f_raw;
	tmp1 = (_lut[4] * Ftemp.f_raw) + (_lut[0]);
	tmp2 = (_lut[6] * Ftemp.f_raw) + (_lut[2]);
	tmp3 = (_lut[5] * Ftemp.f_raw) + (_lut[1]);
	tmp4 = (_lut[7] * Ftemp.f_raw) + (_lut[3]);
	tmp1 = tmp1 + tmp2 * tmp5;
	tmp3 = tmp3 + tmp4 * tmp5;
	tmp5 = tmp5 * tmp5;
	Ftemp.f_raw = tmp1 + tmp3 * tmp5;
	Ftemp.f_raw += ((float) ord) * _lut[8];
	if (base_2_10_e == 2) Ftemp.f_raw *= 1.442695040888965f;
	return Ftemp.f_raw;
}



/// returns fractional remainder
/// возвращает дробный остаток
float mod_f(float value)
{
   return(value - ceil_f(value));
}


/// exponentiation value^degree
/// Возведение в степень value^degree
float pow_f(float value,float degree)
{
    if(0 > value && fmod_f(degree, 1) == 0)
    {
        if(fmod_f(degree, 2) == 0)
            return (exp_f(log_f( -value, 'e') * degree));
        else return (-exp_f(log_f( -value, 'e') * degree));
    }else if(0 > value && fmod_f( degree, 1) != 0)
    {
      return 0.0f;
    }else
    {
        if(value != 0 || 0 >= degree)
            return (exp_f(log_f(value, 'e') * degree));
    }
}


/// Extraction of the square root value
/// Извлечение квадратного корня value
float sqrt_f(float value)
{
#ifdef __CM7_REV
    float rep;
    asm volatile ("vsqrt.f32 %0,%1" : "=t"(rep) : "t"(value));
    return rep;
#else

/// polynomial or magic_VRSQRTE
#define magic_VRSQRTE

 #ifdef magic_VRSQRTE
	float tmp1, tmp2;
	int32_t mas;
	union float_raw Ftemp;
	//fast invsqrt approx
	Ftemp.f_raw = value;
	Ftemp.u_raw = 0x5F3759DF - (Ftemp.u_raw >> 1);      //VRSQRTE
	tmp2 = value * Ftemp.f_raw;
	tmp1 = (3.0f - tmp2 * Ftemp.f_raw) * 0.5f;          //VRSQRTS
	Ftemp.f_raw = Ftemp.f_raw * tmp1;
	tmp2 = value * Ftemp.f_raw;
	tmp1 = (3.0f - tmp2 * Ftemp.f_raw) * 0.5f;
    Ftemp.f_raw = Ftemp.f_raw * tmp1;

	//fast inverse approx
	value = Ftemp.f_raw;
	mas = 0x3F800000 - (Ftemp.u_raw & 0x7F800000);
	Ftemp.u_raw = Ftemp.u_raw + mas;
	Ftemp.f_raw = 1.41176471f - 0.47058824f * Ftemp.f_raw;
	Ftemp.u_raw = Ftemp.u_raw + mas;
	tmp1 = 2.0f - Ftemp.f_raw * value;
	Ftemp.f_raw = Ftemp.f_raw * tmp1;
	tmp1 = 2.0f - Ftemp.f_raw * value;
	Ftemp.f_raw = Ftemp.f_raw * tmp1;

	return Ftemp.f_raw;
 #else
	return (pow_f(value, 0.5f ));
 #endif

#endif
}

float fact_f(float value)
{
    float ret = 1.0f;
    for (float i=1.0f; i <= value; i++)
        ret *= i;
    return ret;
}

/// error 0,0002384%
/// sin input is radian, output 1.0: -1.0.
float sin_f(float value_rad)
{
    float rep, rep_z, sig, ret, fac, fac_i;
    rep = value_rad; rep_z = value_rad; ret = value_rad;
    sig = 1.0f; fac = 1.0f; fac_i = 2.0f;
    do
    {
        rep_z = rep;
        sig *= -1.0f;
        ret *= value_rad * value_rad;
        fac *= fac_i++; fac *= fac_i++;
        rep += sig * (ret / fac);
    }while (rep_z != rep);
    return rep;
}




////////////////////////////// Trig Functions //////////////////////////////
#ifdef PI_DIV_BY_TWO_INV
#undef PI_DIV_BY_TWO_INV
#endif
#define PI_DIV_BY_TWO_INV 0.63661977236758134
#ifdef PI_DIV_BY_TWO
#undef PI_DIV_BY_TWO
#endif
#define PI_DIV_BY_TWO   1.5707963267948966  //pi/2
#ifdef TWOBYPI
#undef TWOBYPI
#endif
#define TWOBYPI          0.6366197723675813   //2/pi

////////////////////////////////////////////////////////////////////////////
//   float cos(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the cosine value of the angle x, which is in radian
// Date : 9/20/2001
//
float sin_f2(float value_rad)
{
   value_rad -= PI / 2.0f;
   float rep, tmp1, tmp2 = 1.0;
   uint8_t quad, nix;
   float frac;
   float dat[5] = {                    //by the series definition for cosine
      -0.49999999456337096f,            // sum ( ( (-1)^n * value_rad^2n )/(2n)! )
       0.04166663896921267f,
      -0.00138883894522527f,
       0.00002476138231734f,
      -0.00000026070414770f,
   };
   if (value_rad < 0.0f) value_rad = -value_rad;
   quad = (uint8_t)(value_rad * (2.0f / PI));
   frac = (value_rad * (2.0f / PI)) - quad;
   quad = quad % 4;
   if (quad == 0 || quad == 2) tmp1 = frac * (PI / 2.0f);
   else if (quad == 1) tmp1 = (1.0f - frac) * (PI / 2.0f);
   else tmp1 = (frac - 1.0f) * (PI / 2.0f);
   rep = 1.0f;
   tmp1 = tmp1 * tmp1;
   for (nix = 0; nix <= 4; nix++)
   {
      tmp2 = tmp2 * tmp1; rep = rep + dat[nix] * tmp2;
   }
   if (quad == 2 || quad == 1)
      rep = -rep;
   return (rep);
}



/*
////////////////////////////////////////////////////////////////////////////
//   float sin(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the sine value of the angle x, which is in radian
// Date : 9/20/2001
//
float sin(float x)
{
   return cos(x - PI_DIV_BY_TWO);
}



////////////////////////////////////////////////////////////////////////////
//   float tan(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the tangent value of the angle x, which is in radian
// Date : 9/20/2001
//
float tan(float x)
{
   float c, s;

   c = cos(x);
   if (c == 0.0)
      return (1.0e+36);

   s = sin(x);
   return(s/c);
}


float const pas[3] = {0.49559947, -4.6145309, 5.6036290};
float const qas[3] = {1.0000000,  -5.5484666, 5.6036290};

float ASIN_COS(float x, uint8_t n)
{
   float y, res, r, y2;
   int_fast8_t s;
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



////////////////////////////////////////////////////////////////////////////
//   float asin(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the arcsine value of the value x.
// Date : N/A
//
float asin(float x)
{
   float r;

   r = ASIN_COS(x, 0);
   return(r);
}


////////////////////////////////////////////////////////////////////////////
//   float acos(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the arccosine value of the value x.
// Date : N/A
//
float acos(float x)
{
   float r;

   r = ASIN_COS(x, 1);
   return(r);
}


float const pat[4] = {0.17630401, 5.6710795, 22.376096, 19.818457};
float const qat[4] = {1.0000000,  11.368190, 28.982246, 19.818457};

////////////////////////////////////////////////////////////////////////////
//   float atan(float x)
////////////////////////////////////////////////////////////////////////////
// Description : returns the arctangent value of the value x.
// Date : N/A
//
float atan(float x)
{
   float y, res, r;
   int_fast8_t s, flag;

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


/////////////////////////////////////////////////////////////////////////////
//   float atan2(float y, float x)
/////////////////////////////////////////////////////////////////////////////
// Description :computes the principal value of arc tangent of y/x, using the
// signs of both the arguments to determine the quadrant of the return value
// Returns : returns the arc tangent of y/x.
// Date : N/A
//

float atan2(float y,float x)
{
   float z;
   int_fast8_t sign;
   uint8_t quad;
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



//////////////////// Hyperbolic functions ////////////////////

////////////////////////////////////////////////////////////////////////////
//   float cosh(float x)
////////////////////////////////////////////////////////////////////////////
// Description : Computes the hyperbolic cosine value of x
// Returns : returns the hyperbolic cosine value of x
// Date : N/A
//

float cosh(float x)
{
   return ((exp(x)+exp(-x))*0.5);
}


////////////////////////////////////////////////////////////////////////////
//   float sinh(float x)
////////////////////////////////////////////////////////////////////////////
// Description : Computes the hyperbolic sine value of x
// Returns : returns the hyperbolic sine value of x
// Date : N/A
//

float sinh(float x)
{

   return ((exp(x) - exp(-x))*0.5);
}


////////////////////////////////////////////////////////////////////////////
//   float tanh(float x)
////////////////////////////////////////////////////////////////////////////
// Description : Computes the hyperbolic tangent value of x
// Returns : returns the hyperbolic tangent value of x
// Date : N/A
//

float tanh(float x)
{
   return(sinh(x)/cosh(x));
}


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
float frexp(float x, int8_t *exp)
{
   float res;
   int_fast8_t sign = 0;
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



//////////////////////////////////////////////////////////////////////////////
//   float ldexp(float x, signed int *exp)
//////////////////////////////////////////////////////////////////////////////
// Description : multiplies a floating point number by an integral power of 2.
// Returns : returns the value of x times 2 raised to the power exp.
// Date : N/A
//

float ldexp(float value, int8_t exp)
{
   return (value * pow(2,exp));
}
*/

#endif
