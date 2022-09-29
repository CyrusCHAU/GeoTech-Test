// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Minesweeper.h"

/** Declare a delegate for OnClicked */
DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnClickedSignature, FIntPoint);

/**
 * 
 */
class MINESWEEPER_API SMinesWidget : public SCompoundWidget
{
public:

#pragma region Slate Argument

	SLATE_BEGIN_ARGS(SMinesWidget) 
		/*: _OnClicked()*/
	{}
	
	/** Expose OnClicked to Slate Event. */
	SLATE_EVENT(FOnClickedSignature, OnClicked)

	/** The mines location */ 
	SLATE_ARGUMENT(FIntPoint, MinesLocation);

	SLATE_END_ARGS()

#pragma endregion

	/** My mines button*/
	TSharedPtr<SButton> MyButton;

	/** My mines label*/
	TSharedPtr<STextBlock> MyLabel;

	/** My mines location*/
	FIntPoint MinesLocation;



	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:

	/** This is private function. Called when my button is fire OnClick. */
	FReply OnButtonClicked();

public:

	/** Public delegate that other needs to bind.*/
	FOnClickedSignature MyOnClickedAction;

	/** Show how many mines did it surround. */
	void DisplayEmptyStyle();
	//void DisplayStyle();

	void CheckEmptySpacesMain(FIntPoint InStartLocation);

	int CheckGridCore(FIntPoint InLocation);
};
