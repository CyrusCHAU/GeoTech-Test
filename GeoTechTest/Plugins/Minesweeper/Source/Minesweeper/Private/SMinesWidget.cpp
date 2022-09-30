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
			.Text(FText::FromString(TEXT("[  ]")))
			.Justification(ETextJustify::Center)
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

	// show empty text
	MyLabel->SetText(FText::FromString( TEXT("^.^")));
}

void SMinesWidget::DisplayNumberStyle(int InNumber)
{
	// Change text color
	//FLinearColor tempColor = (InNumber == 1) ? FLinearColor(0, 0, 1) : (InNumber == 2) ? FLinearColor(0, 1, .3) : FLinearColor(1, 0, 0);
	MyButton->SetColorAndOpacity(FLinearColor(1, 1, 0));

	// Show number
	MyLabel->SetText(FText::FromString(FString::FromInt(InNumber)));
}

void SMinesWidget::DisplayMinesStyle()
{
	// Change text color
	MyButton->SetColorAndOpacity(FLinearColor(1, 0, 0));

	// Show mines text
	MyLabel->SetText(FText::FromString(TEXT("X")));
}

void SMinesWidget::CheckEmptySpacesMain(FIntPoint InStartLocation)
{
	CheckGridCore(InStartLocation);
}

int SMinesWidget::CheckGridCore(FIntPoint InLocation)
{
	return 0;
}