// Basic Calculator App Widget Function Library.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "CalculatorWidgetFunctions.generated.h"

UCLASS()
class CALCULATOR_APP_API UCalculatorWidgetFunctions : UBlueprintFunctionLibrary
{
     GENERATED_BODY()

public:

     double GetResult(double A, double B, uint8 OpCode);
};


// EOF