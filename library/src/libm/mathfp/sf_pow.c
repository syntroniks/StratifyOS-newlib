
/* @(#)z_powf.c 1.0 98/08/13 */
#include <float.h>
#include "fdlibm.h"
#include "zmath.h"

float powf (float x, float y)
{
  float d, k, t, r = 1.0;
  int n, sign, exponent_is_even_int = 0;
  __int32_t px;

  GET_FLOAT_WORD (px, x);

  k = modff (y, &d);

  if (k == 0.0f)
    {
      /* Exponent y is an integer. */
      if (modff (ldexpf (y, -1), &t))
        {
          /* y is odd. */
          exponent_is_even_int = 0;
        }
      else
        {
          /* y is even. */
          exponent_is_even_int = 1;
        }
    }

  if (x == 0.0f)
    {
      if (y <= 0.0f)
        errno = EDOM;
    }
  else if ((t = y * log (fabsf (x))) >= BIGX) 
    {
      errno = ERANGE;
      if (px & 0x80000000) 
        {
          /* x is negative. */
          if (k) 
            {
              /* y is not an integer. */
              errno = EDOM;
              x = 0.0f;
            }
          else if (exponent_is_even_int)
            x = z_infinity_f.f;
          else
            x = -z_infinity_f.f;
        }
    else
      {
        x = z_infinity_f.f;
      }
    }
  else if (t < SMALLX)
    {
      errno = ERANGE;
      x = 0.0f;
    }
  else 
    {
      if ( !k && fabsf (d) <= 32767 ) 
        {
          n = (int) d;

          if ((sign = (n < 0)))
            n = -n;

          while ( n > 0 ) 
            {
              if ((unsigned int) n % 2) 
                r *= x;
              x *= x;
              n = (unsigned int) n / 2;
            }

          if (sign)
            r = 1.0f / r;

          return r;
        }
      else 
        {
          if ( px & 0x80000000 ) 
            {
              /* x is negative. */
              if (k) 
                {
                  /* y is not an integer. */
                  errno = EDOM;
                  return 0.0f;
                }
            }

          x = exp (t);

          if (!exponent_is_even_int) 
            { 
              if (px & 0x80000000)
                {
                  /* y is an odd integer, and x is negative,
                     so the result is negative. */
                  GET_FLOAT_WORD (px, x);
                  px |= 0x80000000;
                  SET_FLOAT_WORD (x, px);
                }
            }
        }
    }

  return x;
}

#ifdef _DOUBLE_IS_32BITS

double pow (double x, double y)
{
  return (double) powf ((float) x, (float) y);
}

#endif /* defined(_DOUBLE_IS_32BITS) */ 
