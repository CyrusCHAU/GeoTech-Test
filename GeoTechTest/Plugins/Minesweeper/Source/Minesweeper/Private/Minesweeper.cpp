// Copyright Epic Games, Inc. All Rights Reserved.

#include "Minesweeper.h"
#include "MinesweeperStyle.h"
#include "MinesweeperCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"


static const FName MinesweeperTabName("Minesweeper");

#define LOCTEXT_NAMESPACE "FMinesweeperModule"

void FMinesweeperModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMinesweeperStyle::Initialize();
	FMinesweeperStyle::ReloadTextures();

	FMinesweeperCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMinesweeperCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FMinesweeperModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMinesweeperModule::RegisterMenus));
}

void FMinesweeperModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMinesweeperStyle::Shutdown();

	FMinesweeperCommands::Unregister();
}

void FMinesweeperModule::PluginButtonClicked()
{
	//// Put your "OnButtonClicked" stuff here
	//FText DialogText = FText::Format(
	//						LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
	//						FText::FromString(TEXT("FMinesweeperModule::PluginButtonClicked()")),
	//						FText::FromString(TEXT("Minesweeper.cpp"))
	//				   );
	//FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	// Init Create Window
	TSharedRef<SWindow> MinesweeperWindow = SNew(SWindow)
		.Title(FText::FromString(TEXT("Minesweeper Window")))
		.ClientSize(FVector2D(800, 400))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
			SNew(SVerticalBox)

			// 1st Row: Custom Title
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
				SNew(STextBlock).Text(FText::FromString(TEXT("Welcome to Minesweeper!!!")))
				]

			// 2nd Row: Input Field
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				[
				SNew(SHorizontalBox) 
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Width")))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(1)
					[
						SNew(SSpacer)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center).FillWidth(1)
					[
						// The Width SpinBox
						 SAssignNew(Input_WidthSpinBox, SSpinBox<int>).Value(5).MinFractionalDigits(0).MaxFractionalDigits(0).MinValue(1)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(1)
					[
						SNew(SSpacer)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(1)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Height")))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(SSpacer)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center).FillWidth(1)
					[
						// The Height SpinBox
						SAssignNew(Input_HeightSpinBox, SSpinBox<int>).Value(5).MinFractionalDigits(0).MaxFractionalDigits(0).MinValue(1)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(SSpacer)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(3)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Number Of Mines")))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(1)
					[
						SNew(SSpacer)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center).FillWidth(1)
					[
						// The Number of Mines SpinBox
						SAssignNew(Input_NumberOfMinesSpinBox, SSpinBox<int>).Value(2).MinFractionalDigits(0).MaxFractionalDigits(0).MinValue(1)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(1)
					[
						SNew(SSpacer)
					]
				]

			// 3rd Row: The Generate Button
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.Padding((0, 50))
				[
					SAssignNew(Input_GenerateGridButton, SButton)
						.OnClicked(FOnClicked::CreateRaw(this, &FMinesweeperModule::OnGenerateGridButtonClicked)) //OnClicked_Raw(this, &FMinesweeperModule::GenerateGridButtonClicked
						
					[
						SAssignNew(Input_GenerateGridLabel, STextBlock)
							.Text(FText::FromString(TEXT("Generate New Grid")))
							.Justification(ETextJustify::Center)
					]					
				]

			// 4th Row: The Buttons
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.FillHeight(8)
				[
					SAssignNew(GridVerticalBoxRoot, SVerticalBox)
				]
		];

	// Add the new spawned window
	IMainFrameModule& MainFrameModule =FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));

	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(MinesweeperWindow, MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(MinesweeperWindow);
	}
}

void FMinesweeperModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMinesweeperCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMinesweeperCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

FReply FMinesweeperModule::OnGenerateGridButtonClicked() 
{
	// Store values from SpinBox
	GridWidth = Input_WidthSpinBox.Get()->GetValue();
	GridHeight = Input_HeightSpinBox.Get()->GetValue();
	GridMines = Input_NumberOfMinesSpinBox.Get()->GetValue();

	// Generate Grid Main Action
	GenerateGridMain(GridWidth, GridHeight, GridMines);

	// After Sucessful, Give a Message in Button
	Input_GenerateGridLabel.Get()->SetText(FText::FromString(TEXT("Generated")));

	return FReply::Handled();
}

void FMinesweeperModule::GenerateGridMain(int InWidth, int InHeight, int InMines)
{
	//GridRoot + SVerticalBox::Slot();
	//GridRoot->AddSlot().AttachWidget(Input_GenerateGridLabel);

	/** This can be compiled, but didn't show up
	auto temp1 = GridRoot->AddSlot();
	temp1
		[
			SNew(SButton)
		];
	*/

	//// Add Vertical Box Slot
	//GridVerticalBoxRoot->AddSlot()
	//	.HAlign(HAlign_Fill)
	//	.VAlign(VAlign_Center)
	//	[
	//		SNew(SButton)
	//		//.AddMetaData()
	//		//.OnClicked_Raw()
	//		[
	//			SNew(STextBlock)
	//			.Text(FText::FromString(TEXT("Generate New Grid")))
	//			.Justification(ETextJustify::Center)
	//		]
	//	];

	// Check Conditions. Only play when number of mines is smaller than the total grid size.
	if (InMines < InWidth * InHeight)
	{
		// Clear All Buttons Under Root
		ClearAllButtons();

		// Height, Row
		for (int h = 0; h < InHeight; h++)
		{
			// Add a Horizontal Box
			TSharedPtr<SHorizontalBox> tempHorizontalBox;

			// Construct a Vertical Box's Slot for Height
			GridVerticalBoxRoot->AddSlot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.AutoHeight()
				[
					SAssignNew(tempHorizontalBox, SHorizontalBox)
				];

			// Width, Column
			for (int w = 0; w < InWidth; w++)
			{
				FIntPoint tempLocation = FIntPoint(h, w);
				GeneratedButtonIDs.Add(tempLocation, AddGridButtonCore(tempHorizontalBox, tempLocation));
			}
		}

		// Spawn Mines Map
		MinesMap = GenerateMinesMapMain(InWidth, InHeight, InMines);
	} 
	else
	{
		return;
	}
}

TSharedPtr<SMinesWidget> FMinesweeperModule::AddGridButtonCore(TSharedPtr<SHorizontalBox> InHorizontalBox, FIntPoint InLocation)
{
	// Temp Spawned Button
	TSharedPtr<SMinesWidget> tempWidget;

	// Add Slot, Button, Button's Label
	InHorizontalBox->AddSlot()
		.AutoWidth()
		[
			SNew(SBox)
				.WidthOverride(ButtonSize_Width)
				.HeightOverride(ButtonSize_Height)
				[
					SAssignNew(tempWidget, SMinesWidget) //SAssignNew(tempWidget, SMinesWidget)
						.MinesLocation(InLocation) // Give its Location Info
						.OnClicked_Raw(this, &FMinesweeperModule::OnMineButtonClicked) // Bind Delegate
				]
		];

	return tempWidget; // This OK
	//return TSharedPtr<SMinesWidget>(); // This ok

}

TSharedPtr<SMinesWidget> FMinesweeperModule::temp1()
{
	return TSharedPtr<SMinesWidget>();
}

void FMinesweeperModule::ClearAllButtons()
{
	// Clear Stored Buttons
	//GeneratedButtonIDs.Empty();

	// Remove All Childs
	GridVerticalBoxRoot->ClearChildren();
}

FReply  FMinesweeperModule::OnMineButtonClicked(FIntPoint InLocation)
{
	//// Has InButton?
	//if (InButton)
	//{
	//	// Check if Valid InButton
	//	if (GeneratedButtonIDs.Contains(InButton))
	//	{
	//		// Find out which location did user pressed
	//		const FIntPoint tempClickedLocation = *GeneratedButtonIDs.Find(InButton);

	//		// Check if clicked a mine? 
	//		if (MinesMap.Contains(tempClickedLocation))
	//		{
	//			// Lose Game
	//			LoseGameMain();
	//		}
	//	}
	//}
	//else
	//{
	//	FText DialogText = FText::FromString(TEXT("Invalid InButton!"));

	//	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	//}



	FText DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "InLocation: X: {0} & Y: {1}"),
		InLocation.X,
		InLocation.Y
	 );


	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	 //Check if clicked a mine? 
	if (MinesMap.Contains(InLocation))
	{
		// Lose Game
		LoseGameMain();
	}
	else
	{
		// Still have empty grid?
		if (OpenedMap.Num() < ((GridWidth * GridHeight) - GridMines))
		{
			// Add record of user's opened location
			OpenedMap.Add(InLocation);

			// Get the related button
			TSharedPtr<SMinesWidget>* tempRelatedWidget = GeneratedButtonIDs.Find(InLocation);

			// Change it style to show it is "Empty" to user
			tempRelatedWidget->Get()->DisplayEmptyStyle();
		}
		// No empty grid, Win the Game!
		else
		{
			
		}
	}



	return FReply::Handled();
}

FReply FMinesweeperModule::OnMineButtonClicked_BACKUP(TSharedPtr<SButton> InButton)
{
/*
	// Has InButton?
	if (InButton)
	{
		// Check if Valid InButton
		if (GeneratedButtonIDs.Contains(InButton))
		{
			// Find out which location did user pressed
			const FIntPoint tempClickedLocation = *GeneratedButtonIDs.Find(InButton);

			// Check if clicked a mine? 
			if (MinesMap.Contains(tempClickedLocation))
			{
				// Lose Game
				LoseGameMain();
			}
		}
	}
	else
	{
		FText DialogText = FText::FromString(TEXT("Invalid InButton!"));

		FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	}
	*/

	return FReply::Handled();
}


TArray<FIntPoint> FMinesweeperModule::GenerateMinesMapMain(int InWidth, int InHeight, int InMines)
{
	// Init a FIntPoint
	TArray<FIntPoint> tempMinesLocations;

	// Keep doing random place a mine
	while (tempMinesLocations.Num() < InMines)
	{
		// First random spawn a number first
		int tempW = FMath::RandHelper(InWidth - 1);
		int tempH = FMath::RandHelper(InHeight - 1);

		// Only accept when there is no same mine location
		if (!tempMinesLocations.Contains(FIntPoint(tempW, tempH)))
		{
			tempMinesLocations.Add(FIntPoint(tempW, tempH));
		}
	}

	return tempMinesLocations;
}

void FMinesweeperModule::LoseGameMain()
{
	FText DialogText = FText::FromString(TEXT("You Clicked a Mine! Press \"OK\" to play again!"));

	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperModule, Minesweeper)