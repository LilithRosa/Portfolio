// Distance Conversion Header file 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DistanceConversions.generated.h"

UENUM(BlueprintType)
enum class EDistanceUnit
{
     Nanometer,
     Micrometer,
     Millimeter,
     Centimeter,
     Decimeter,
     Meter,
     Decimeter,
     Hectometer,
     Kilometer,

     Inch,
     Foot,
     Yard,
     Mile
};


UCLASS()
class EXAMPLEPROJECT_API UDistanceConversions : public UBlueprintFunctionLibrary
{
     GENERATED_BODY()

public:

     UFUNCTION(BlueprintPure, Category="Math|Conversions")
     static double Convert();
};

// EOF