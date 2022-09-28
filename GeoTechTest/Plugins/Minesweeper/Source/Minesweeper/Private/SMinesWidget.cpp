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
		MyOnClickedAction.Execute(MinesLocation);
	}
	else
	{
		FText temp1 = FText::FromString(TEXT("Invalid MyOnClickedAction"));
		FMessageDialog::Open(EAppMsgType::Ok, temp1);
	}

	return FReply::Handled();
}