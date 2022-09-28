// Fill out your copyright notice in the Description page of Project Settings.


#include "SMinesWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMinesWidget::Construct(const FArguments& InArgs)
{
	MyOnClickedAction = InArgs._OnClicked;
	MinesLocation = InArgs._MinesLocation;
	
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
	//OnClicked.Broadcast(MinesLocation);

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

//FOnClickedSignature SMinesWidget::OnClicked(SMinesWidget* InWidget)
//{
//	return FOnClickedSignature();
//}
