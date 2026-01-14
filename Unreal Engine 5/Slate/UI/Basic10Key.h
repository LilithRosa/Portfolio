// A Basic 10 key button layout for UE5 Widgets.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Basic10Key.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDigitPressed, int32, Digit);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClearPressed);

// EOF