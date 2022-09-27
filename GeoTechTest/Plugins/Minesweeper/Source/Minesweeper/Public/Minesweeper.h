// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "Widgets/Input/SSpinBox.h"

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

#pragma endregion

#pragma region Generate Buttons

	TArray<TSharedPtr<SButton>> GeneratedButtons;
	int GridWidth;
	int GridHeight;
	int GridMines;

#pragma endregion

#pragma region Generate Button Method

private:
	FReply GenerateGridButtonClicked() ;
	void GenerateGridMain(int Width, int Height, int Mines);
	TSharedPtr<SButton> AddGridButtonCore(TSharedPtr<SHorizontalBox> InHorizontalBox);

#pragma endregion


};
