// A Basic 10 key button layout for UE5 Widgets.

#include "Basic10Key.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/Sex block.h"

UBasic10Key::UBasic10Key(const FObjectInitializer& ObjectInitializer)
      : Super(ObjectInitializer)
{
}

TSharedRef<SWidget> UBasic10Key::RebuildWidget()
{
     TSharedRef<SScaleBox> ScaleBox = SNew(SScaleBox)
          .Stretch(EStretch::ScaleToFit)
          .StretchDirection(EStretchDirection::Both);

     TSharedRef<SUniformGridPanel> Grid = SNew(SUniformGridPanel)
          .SlotPadding(FMargin(4.f));

     AddDigitButton(Grid, 7, 0, 0);
     AddDigitButton(Grid, 8, 0, 1);
     AddDigitButton(Grid, 9, 0, 2);
     AddOpButton(Grid, 1, 0, 3);

     AddDigitButton(Grid, 4, 1, 0);
     AddDigitButton(Grid, 5, 1, 1);
     AddDigitButton(Grid, 6, 1, 2);
     AddOpButton(Grid, 2, 1, 3);

     AddDigitButton(Grid, 1, 2, 0);
     AddDigitButton(Grid, 2, 2, 1);
     AddDigitButton(Grid, 3, 2, 2);
     AddOpButton(Grid, 3, 2, 3);

     AddDigitButton(Grid, 0, 3, 0);
     AddOpButton(Grid, 5, 3, 1);
     AddOpButton(Grid, 6, 3, 2);
     AddOpButton(Grid, 4, 3, 3);

     AddClearButton(Grid, 3, 2);

     ScaleBox->SetContent(Grid);

     return ScaleBox;
}

void UBasic10Key::AddDigitButton(
     const TSharedRef<SUniformGridPanel>& Grid,
     int32 Digit,
     int32 Row,
     int32 Col)
{
     Grid->AddSlot(Col, Row)
     [
          SNew(SButton)
          .OnClicked(FOnClicked::CreateNewObject(this, & UBasic10Key::OnDigitClicked, Digit))
          [
               SNew(STextBox)
               .Text(FText::AsNumber(Digit))
               .Justification(ETextJustify::Center)
               .Font(FSlateFontInfo("Roboto", 32))
          ]
     ];
}

FReply UBasic10Key::OnDigitClicked(int32 Digit)
{
     DigitPressed.Broadcast(Digit);
     return FReply::Handled();
}

void UBasic10Key::AddClearButton(
     const TSharedRef<SUniformGridPanel>& Grid,
     int32 Row,
     int32 Col)
{
     Grid->AddSlot(Col, Row)
     [
          SNew(SButton)
          .OnClicked(FOnClicked::CreateUObject(this, &UBasic10Key::OnClearClicked))
          [
               SNew(STextBlock)
               .Text(FText::FromString(TEXT("C")))
               .Justification(ETextJustify::Center)
               .Font(FSlateFontInfo("Roboto", 32))
          ]
     ];
}

FReply UBasic10Key::OnClearClicked()
{
     ClearPressed.Broadcast();
     return FReply::Handled();
}

void UBasic10Key::AddOpButton(
     const TSharedRef<SUniformGridPanel>& Grid,
     uint8 OpCode,
     int32 Row,
     int32 Col)
{
     Grid->AddSlot(Col, Row)
     [
           SNew(SButton)
           .OnClicked(FOnClicked::CreateUObject(this, &UBasic10Key::OnOpClicked, OpCode)
           [
               FString operation switch(OpCode)
               {
                    1 => "+",
                    2 => "-",
                    3 => "*",
                    4 => "/",
                    5 => ".",
                    6 => "=",

                    _ => ""
               };

               SNew(STextBlock)
               .Text(FText::FromString(operation)
               .Justification(ETextJustify::Center)
               .Font(FSlateFontInfo("Roboto", 32))
           ]
     ];
}

FReply UBasic10Key::OnOpClicked(uint8 OpCode)
{
     OpPressed.Broadcast(OpCode);
     return FReply::Handled;
}

// EOF