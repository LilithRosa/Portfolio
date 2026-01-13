// Distance Conversion Header file 

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DistanceConversions.generated.h"

UENUM(BlueprintType)
enum class EDistanceUnit : uint8
{
     Nanometer UMETA(DisplayName="Nanometer"),
     Micrometer UMETA(DisplayName="Micrometer"),
     Millimeter UMETA(DisplayName="Millimeter"),
     Centimeter UMETA(DisplayName="Centimeter"),
     Decimeter UMETA(DisplayName="Decimeter"),
     Meter UMETA(DisplayName="Meter"),
     Decimeter UMETA(DisplayName="Decameter"),
     Hectometer UMETA(DisplayName="Hectometer"),
     Kilometer UMETA(DisplayName="Kilometer"),

     Inch UMETA(DisplayName="Inch"),
     Foot UMETA(DisplayName="Foot"),
     Yard UMETA(DisplayName="Yard"),
     Mile UMETA(DisplayName="Mile")
};


UCLASS()
class EXAMPLEPROJECT_API UDistanceConversions : public UBlueprintFunctionLibrary
{
     GENERATED_BODY()

public:

     UFUNCTION(BlueprintPure, Category="Math|Conversions")
     static double Convert(double Value, EDistanceUnit From, EDistanceUnit To);

private:

     static double ToMeters(EDistanceUnit Unit);
};

// EOF