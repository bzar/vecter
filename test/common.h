#ifndef VECTER_TEST_COMMON_H
#define VECTER_TEST_COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "vecter.h"


bool equalV2d(v2d a, v2d b)
{
  if(a.x == b.x && a.y == b.y)
  {
    return true;
  }
  else
  {
    printf("Vectors are not equal! (%i, %i) != (%i, %i)\n", a.x, a.y, b.x, b.y);
    return false;
  }
}

#endif
