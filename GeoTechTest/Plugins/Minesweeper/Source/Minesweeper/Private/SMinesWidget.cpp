// Fill out your copyright notice in the Description page of Project Settings.


#include "SMinesWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesWidget::Construct(const FArguments& InArgs)
{
	// Let all Args set into this var
	MyOnClickedAction = InArgs._OnClicked;
	MinesLocation = InArgs._MinesLocation;
	
	// Create Custom Widget
	ChildSlot
	[
		SAssignNew(MyButton, SButton)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.OnClicked_Raw(this, &SMinesWidget::OnButtonClicked)
		[
			SAssignNew(MyLabel, STextBlock)
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SMinesWidget::OnButtonClicked()
{
	// Execute a delegate
	if (MyOnClickedAction.IsBound())
	{
		MyOnClickedAction.Execute(MinesLocation, this);
	}
	else
	{
		FText temp1 = FText::FromString(TEXT("Invalid MyOnClickedAction"));
		FMessageDialog::Open(EAppMsgType::Ok, temp1);
	}

	return FReply::Handled();
}

void SMinesWidget::DisplayEmptyStyle()
{
	// Change text color
	MyButton->SetColorAndOpacity(FLinearColor(0, 1, 0));

	// Button Background Color
	MyButton->SetBorderBackgroundColor(FLinearColor(0.55, 0.55, 0.55));

	// show empty text
	//MyLabel->SetText(FText::FromString( TEXT("^.^")));
}

void SMinesWidget::DisplayNumberStyle(int InNumber)
{
	// Change text color
	FLinearColor tempColor = (InNumber == 1) ? FLinearColor(0.06, 0.06, 1) : (InNumber == 2) ? FLinearColor(0, 1, .3) : FLinearColor(1, 0, 0);
	MyButton->SetColorAndOpacity(tempColor); //FLinearColor(1, 1, 0)

	// Button Background Color
	MyButton->SetBorderBackgroundColor(FLinearColor(0.26, 0.26, 0.26));

	// Show number
	MyLabel->SetText(FText::FromString(FString::FromInt(InNumber)));
}

void SMinesWidget::DisplayMinesStyle()
{
	// Change text color
	MyButton->SetColorAndOpacity(FLinearColor(1, 0, 0));

	// Button Background Color
	MyButton->SetBorderBackgroundColor(FLinearColor(0.27, 0.08, 0.08));

	// Show mines text
	MyLabel->SetText(FText::FromString(TEXT("X")));
}
