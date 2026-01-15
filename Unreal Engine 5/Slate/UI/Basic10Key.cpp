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

     AddDigitButton(Grid, 4, 1, 0);
     AddDigitButton(Grid, 5, 1, 1);
     AddDigitButton(Grid, 6, 1, 2);

     AddDigitButton(Grid, 1, 2, 0);
     AddDigitButton(Grid, 2, 2, 1);
     AddDigitButton(Grid, 3, 2, 2);

     AddDigitButton(Grid, 0, 3, 1);

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

// EOF