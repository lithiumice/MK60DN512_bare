#include "headfile.h"
#include "main.h"

//////////////////////////////////////////////////////////
void push_and_pull(float *buff, int len, float newdata)
{
  for (int i = len - 1; i > 0; i--)
  {
    *(buff + i) = *(buff + i - 1);
  }
  *buff = newdata;
}

void push_int_pull(int *buff, int len, int newdata)
{
  for (int i = len - 1; i > 0; i--)
  {
    *(buff + i) = *(buff + i - 1);
  }
  *buff = newdata;
}

int clip(int x, int low, int up)
{
  return x > up ? up : x < low ? low
                               : x;
}

float fclip(float x, float low, float up)
{
  return x > up ? up : x < low ? low
                               : x;
}

void LPF_1_db(float hz, float time, float in, float *out)
{
  *out += (1 / (1 + 1 / (hz * 6.28f * time))) * (in - *out);
}

float my_sqrt(float number)
{
  long i;
  float x, y;
  const float f = 1.5F;
  x = number * 0.5F;
  y = number;
  i = *(long *)&y;
  i = 0x5f3759df - (i >> 1);

  y = *(float *)&i;
  y = y * (f - (x * y * y));
  y = y * (f - (x * y * y));
  return number * y;
}