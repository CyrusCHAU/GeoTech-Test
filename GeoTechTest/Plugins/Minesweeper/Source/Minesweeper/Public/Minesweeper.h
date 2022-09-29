// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Input/SSpinBox.h"
#include "SMinesWidget.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"

class FToolBarBuilder;
class FMenuBuilder;

class FMinesweeperModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();


private:
	TSharedPtr<class FUICommandList> PluginCommands;

#pragma region Slate

	TSharedPtr<SSpinBox<int>> Input_WidthSpinBox;
	TSharedPtr<SSpinBox<int>> Input_HeightSpinBox;
	TSharedPtr<SSpinBox<int>> Input_NumberOfMinesSpinBox;
	TSharedPtr<SButton> Input_GenerateGridButton;
	TSharedPtr<STextBlock> Input_GenerateGridLabel;
	TSharedPtr<SVerticalBox> GridVerticalBoxRoot;

	/** 
	* Key: Button Pointer.
	* Value: Location, start from 0. 
	*/
	//TMap<FIntPoint, TSharedPtr<SMinesWidget>> GeneratedButtonIDs;
	int ButtonSize_Width = 48;
	int ButtonSize_Height = 48;

#pragma endregion

#pragma region Mines & Grid

	int GridWidth;
	int GridHeight;
	int GridMines;

	/** The mines location. */
	TArray<FIntPoint> MinesMap;

	/** The discovered location. */
	TArray<FIntPoint> OpenedMap;

	/** The pre-calculate map that already shows the each grid how many mines they surround. */
	TMap<FIntPoint, int> NumberOfMinesSurroundMap;

#pragma endregion

#pragma region Generate Mines / Grid Button Method

private:

	/** Called when user clicked "Generate Button". */
	FReply OnGenerateGridButtonClicked();

	/** Main action to generate a new grid. */
	void GenerateGridMain(int InWidth, int InHeight, int InMines);

	/** Add a grid button. For internal use only. */
	TSharedPtr<SButton> AddGridButtonCore(TSharedPtr<SHorizontalBox> InHorizontalBox, FIntPoint InLocation);
	//TSharedPtr<SMinesWidget> temp1(); // This ok

	/** Empty all grid & buttons. */
	void ClearAllButtons();

	/** Called when user click a grid button during finding a mine. */
	FReply OnMineButtonClicked(FIntPoint InLocation);
	FReply OnMineButtonClicked_BACKUP(TSharedPtr<SButton> InButton);

	/** Generate a new Mines Map, which returns a FVector2D. */
	TArray<FIntPoint> GenerateMinesMapMain(int InWidth, int InHeight, int InMines);

	/** Action for a lose game. */
	void LoseGameMain();
	
#pragma endregion


};
