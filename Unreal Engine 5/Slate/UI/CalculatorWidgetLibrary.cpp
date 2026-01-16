// Calculator Widget Functions.

#include "CalculatorWidgetFunctions.h"

double UCalculatorWidgetFunctions::GetResult(double A, double B, uint8 OpCode)
{
     switch(OpCode)
     {
          case 1: return A + B;
          case 2: return A - B;
          case 3: return A * B;
          case 4: return A / B;

          default: return A + B;
     }
}

// EOF