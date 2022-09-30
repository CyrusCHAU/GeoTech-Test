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
				FIntPoint tempLocation = FIntPoint(h, w); // This is (Y, X)
				GeneratedButtonIDs.Add(tempLocation, AddGridButtonCore(tempHorizontalBox, tempLocation));

				// This is OK in GeneratedButtonIDs
				/*TArray<FIntPoint> tempKeys;
				GeneratedButtonIDs.GetKeys(tempKeys);
				Debug_LogFIntPoint(tempKeys, "P1: New added to GeneratedButtonIDs");*/ 
			}
		}

		// Spawn Mines Map
		MinesMap = GenerateMinesMapMain(InWidth, InHeight, InMines);
		Debug_LogFIntPoint(MinesMap, "MinesMap: ");

		// Calculate number of mines in each grid
		NumberOfMinesSurroundMap = CalculateNumberOfMinesMap(InWidth, InHeight);
		Debug_LogFIntPointInt(NumberOfMinesSurroundMap);

	} 
	else
	{
		return;
	}
}

// The location stored in(Y, X) pr(H, W).
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
	//return TSharedPtr<SButton>(); // This ok

}


void FMinesweeperModule::ClearAllButtons()
{
	// Clear Stored Buttons
	GeneratedButtonIDs.Empty();

	// Remove All Childs
	GridVerticalBoxRoot->ClearChildren();
}

FReply  FMinesweeperModule::OnMineButtonClicked(FIntPoint InLocation, SMinesWidget* InWidget)
{
	FString tempDebug1;
	for (auto it : NumberOfMinesSurroundMap)
	{
		tempDebug1 += "[";
		tempDebug1 += FString::FromInt(it.Key.X);
		tempDebug1 += ",";
		tempDebug1 += FString::FromInt(it.Key.Y);
		tempDebug1 += ":";
		tempDebug1 += FString::FromInt(it.Value);
		tempDebug1 += "]";
	}
	FText tempDebug2 = FText::FromString(tempDebug1);
	FMessageDialog::Open(EAppMsgType::Ok, tempDebug2);


	FText DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "InLocation: Y: {0} & X: {1}. Number: {2}."),
		InLocation.Y,
		InLocation.X,
		*NumberOfMinesSurroundMap.Find(InLocation)
	 );


	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	 //Check if clicked a mine? 
	if (CheckIsMines(InLocation)) //MinesMap.Contains(InLocation)
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

			// Pressing the number
			if (NumberOfMinesSurroundMap.Find(InLocation) != 0)
			{
				auto temp1 = NumberOfMinesSurroundMap.Find(InLocation);
				InWidget->DisplayNumberStyle(*NumberOfMinesSurroundMap.Find(InLocation));
			}
			// Pressing the empty
			else
			{
				InWidget->DisplayEmptyStyle();
			}

		}
		// No empty grid, Win the Game!
		else
		{
			
		}
	}



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
		if (!tempMinesLocations.Contains(FIntPoint(tempH, tempW)))
		{
			tempMinesLocations.Add(FIntPoint(tempH, tempW));
		}
	}

	return tempMinesLocations;
}

TMap<FIntPoint, int> FMinesweeperModule::CalculateNumberOfMinesMap(int InWidth, int InHeight)
{
	// Local Map
	TMap<FIntPoint, int>* tempMap = new TMap<FIntPoint, int>();

	//for (int h = 0; h < InHeight; h++)
	//{
	//	for (int w = 0; w < InWidth; w++)
	//	{
	//		if (MinesMap.Contains((h, w)))
	//		{
	//			tempMap.Add((h, w), -1);

	//			Debug_LogFIntPointInt(tempMap, "Contains Mines in this location");
	//			//continue;
	//		}
	//		else
	//		{
	//			// Store local number
	//			int tempNumber = 0;

	//			// Check near 8 spaces
	//			TArray<FIntPoint> tempNeighbor;
	//			tempNeighbor.Add((h - 1, w - 1));
	//			tempNeighbor.Add((h - 1, w    ));
	//			tempNeighbor.Add((h - 1, w + 1));
	//			tempNeighbor.Add((h    , w - 1));
	//			tempNeighbor.Add((h    , w + 1));
	//			tempNeighbor.Add((h + 1, w - 1));
	//			tempNeighbor.Add((h + 1, w    ));
	//			tempNeighbor.Add((h + 1, w + 1));
	//			
	//			for (FIntPoint tempLoc : tempNeighbor)
	//			{
	//				if (CheckIsMines(tempLoc))
	//				{
	//					tempNumber += 1;
	//				} 
	//			}
	//			
	//			tempMap.Add((h, w), tempNumber);

	//			Debug_LogFIntPointInt(tempMap, "No Mines in this location");
	//		}
	//	}
	//}

	TArray<FIntPoint> tempArray;
	GeneratedButtonIDs.GetKeys(tempArray);
	Debug_LogFIntPoint(tempArray, "New Array for Looping");

	for (FIntPoint it : tempArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("Current it: %d, %d"), it.X, it.Y);

		if (MinesMap.Contains((it.X, it.Y)))
		{
			FIntPoint tempGridOP = FIntPoint(it.X, it.Y);
			tempMap->Add(tempGridOP, -1);

			Debug_LogFIntPointInt(*tempMap, "Contains Mines in this location:");
			//continue;
		}
		else
		{
			// Local it
			int tempIT_X = it.X;	
			int tempIT_Y = it.Y;
			
			// Store local number
			int tempNumber = 0;

			// Check near 8 spaces
			TArray<FIntPoint>* tempNeighbor = new TArray<FIntPoint>;
			int tempLeft = tempIT_X - 1;
			int tempRight = tempIT_X + 1;
			int tempUp = tempIT_Y - 1;
			int tempDown = tempIT_Y + 1;

			FIntPoint tempGrid0 = FIntPoint(tempLeft, tempUp);
			FIntPoint tempGrid1 = FIntPoint(tempIT_X, tempUp);
			FIntPoint tempGrid2 = FIntPoint(tempRight, tempUp);
			FIntPoint tempGrid4 = FIntPoint(tempLeft, tempIT_Y);
			FIntPoint tempGrid5 = FIntPoint(tempRight, tempIT_Y);
			FIntPoint tempGrid6 = FIntPoint(tempLeft, tempDown);
			FIntPoint tempGrid7 = FIntPoint(tempIT_X, tempDown);
			FIntPoint tempGrid8 = FIntPoint(tempRight, tempDown);
			tempNeighbor->Add(tempGrid0);
			tempNeighbor->Add(tempGrid1);
			tempNeighbor->Add(tempGrid2);
			tempNeighbor->Add(tempGrid4);
			tempNeighbor->Add(tempGrid5);
			tempNeighbor->Add(tempGrid6);
			tempNeighbor->Add(tempGrid7);
			tempNeighbor->Add(tempGrid8);

			Debug_LogFIntPoint(*tempNeighbor, "TempNeighbor");

			for (FIntPoint tempLoc : *tempNeighbor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Current Looping Neighbor: %d, %d"), tempLoc.X, tempLoc.Y);

				if (CheckIsMines(tempLoc))
				{
					tempNumber += 1;
				}
			}

			FIntPoint tempGridOP = FIntPoint(tempIT_X, tempIT_Y);
			tempMap->Add(tempGridOP, tempNumber);

			Debug_LogFIntPointInt(*tempMap, "No Mines in this location");
		}
	}
	return *tempMap;
}

bool FMinesweeperModule::CheckIsMines(FIntPoint InLocaion)
{
	return MinesMap.Contains(InLocaion);
}

void FMinesweeperModule::LoseGameMain()
{
	FText DialogText = FText::FromString(TEXT("You Clicked a Mine! Press \"OK\" to play again!"));

	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FMinesweeperModule::Debug_LogFIntPoint(TArray<FIntPoint> InArray, FString InPrefix)
{
	FString tempDebug1;

	tempDebug1 += InPrefix;

	for (auto it : InArray)
	{
		tempDebug1 += "[";
		tempDebug1 += FString::FromInt(it.X);
		tempDebug1 += ",";
		tempDebug1 += FString::FromInt(it.Y);
		tempDebug1 += "]";
	}
	FText tempDebug2 = FText::FromString(tempDebug1);
	FMessageDialog::Open(EAppMsgType::Ok, tempDebug2);
	//UE_LOG(LogTemp, Display, tempDebug1);

}

void FMinesweeperModule::Debug_LogFIntPointInt(TMap<FIntPoint, int> InMap, FString InPrefix)
{
	FString tempDebug1;

	tempDebug1 += InPrefix;

	for (auto it : InMap)
	{
		tempDebug1 += "[";
		tempDebug1 += FString::FromInt(it.Key.X);
		tempDebug1 += ",";
		tempDebug1 += FString::FromInt(it.Key.Y);
		tempDebug1 += ":";
		tempDebug1 += FString::FromInt(it.Value);
		tempDebug1 += "]";
	}
	FText tempDebug2 = FText::FromString(tempDebug1);
	FMessageDialog::Open(EAppMsgType::Ok, tempDebug2);
	//UE_LOG(LogTemp, Display, tempDebug1);

}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperModule, Minesweeper)