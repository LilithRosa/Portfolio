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

     UFUNCTION(BlueprintAssignable, Category= "Keypad")
     FOnAddPressed AddPressed;

     UFUNCTION(BlueprintAssignable, Category= "Keypad")
     FOnSubPressed SubPressed;

     UFUNCTION(BlueprintAssignable, Category= "Keypad")
     FOnMulPressed MulPressed;

     UFUNCTION(BlueprintAssignable, Category= "Keypad")
     FOnDivPressed DivPressed;

     UFUNCTION(BlueprintAssignable, Category= "Keypad")
     FOnEqPressed EqPressed;

     UFUNCTION(BlueprintAssignable, Category= "Keypad")
     FOnDecressed DecPressed;

protected:

     void AddDigitButton(
           const TSharedRef<class SUniformGridPanel>& Grid,
           int32 Digit,
           int32 Row,
           int32 Col
     );

     FReply OnClearClicked();

     FReply OnAddClicked();
     FReply OnSubClicked();
     FReply OnMulClicked();
     FReply OnDivClicked();

     FReply OnEqClicked();
     FReply OnDecClicked();
};

// EOF