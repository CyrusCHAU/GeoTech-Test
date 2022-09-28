// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Minesweeper.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedSignature, SMinesWidget*, InWidget);
//DECLARE_MULTICAST_DELEGATE_OneParam(FOnClickedSignature, FIntPoint, InLocation)

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnClickedSignature, FIntPoint, temp1);
//DECLARE_DELEGATE_RetVal(FReply, FImageOnClicked); //this is OK
DECLARE_DELEGATE_RetVal_OneParam(FReply, FOnClickedSignature, FIntPoint); //this is OK

/**
 * 
 */
class MINESWEEPER_API SMinesWidget : public SCompoundWidget
{
public:

	//typedef typename TSlateDelegates<SMinesWidget>::FOnGenerateRow FOnGenerateRow; M1

#pragma region Slate Argument

	SLATE_BEGIN_ARGS(SMinesWidget) 
		: _OnClicked()
	{}
	
	/** My owning FMinesweeperModule object. */
	//SLATE_ARGUMENT(FMinesweeperModule*, OwnRoot);
	//SLATE_ARGUMENT(TWeakObjectPtr<class FMinesweeperModule>, OwnRoot);

	//SLATE_EVENT(FOnGenerateRow, OnGenerateRow) M1
	SLATE_EVENT(FOnClickedSignature, OnClicked)

	/** The mines location */ 
	SLATE_ARGUMENT(FIntPoint, MinesLocation);

	SLATE_END_ARGS()

#pragma endregion

	//TWeakObjectPtr<FMinesweeperModule> OwnRoot;

	/** My mines button*/
	TSharedPtr<SButton> MyButton;

	/** My mines label*/
	TSharedPtr<STextBlock> MyLabel;

	/** My mines location*/
	FIntPoint MinesLocation;



	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	FReply OnButtonClicked();

public:
	/*UDELEGATE()
	FOnClickedSignature OnClicked; */

	FOnClickedSignature MyOnClickedAction;
};
