// A Basic 10 key button layout for UE5 Widgets.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Basic10Key.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDigitPressed, int32, Digit);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClearPressed);

UCLASS()
class UBasic10Key : UUserWidget
{
     GENERATED_BODY()

public:

     UBasic10Key(const FObjectInitializer& ObjectInitializer);

     virtual TSharedRef<SWidget> RebuildWidget () override;

     UFUNCTION(BlueprintAssignable, Category= "Keypad")
     FOnDigitPressed DigitPressed;

     UFUNCTION(BlueprintAssignable, Category= "Keypad")
     FOnClearPressed ClearPressed;

protected:

     void AddDigitButton(
           const TSharedRef<class SUniformGridPanel>& Grid,
           int32 Digit,
           int32 Row,
           int32 Col
     );

     FReply OnClearClicked();
};

// EOF