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
				FIntPoint tempLocation = FIntPoint(w, h); // This is (X, Y)
				GeneratedButtonIDs.Add(tempLocation, AddGridButtonCore(tempHorizontalBox, tempLocation));
			}
		}

		// Spawn Mines Map
		MinesMap = GenerateMinesMapMain(InWidth, InHeight, InMines);
		//Debug_LogFIntPoint(MinesMap, "MinesMap: ");

		// Calculate number of mines in each grid
		NumberOfMinesSurroundMap = CalculateNumberOfMinesMap();
		//Debug_LogFIntPointInt(NumberOfMinesSurroundMap);

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
	 //Check if clicked a mine? 
	if (CheckIsMines(InLocation)) //MinesMap.Contains(InLocation)
	{
		// Lose Game
		LoseGameMain();
	}
	else
	{
		// Still have grid didn't press?
		if (OpenedMap.Num() < ((GridWidth * GridHeight) - GridMines))
		{
			// Add record of user's opened location
			OpenedMap.Add(InLocation);

			// Pressing the number
			if (*NumberOfMinesSurroundMap.Find(InLocation) != 0)
			{
				InWidget->DisplayNumberStyle(*NumberOfMinesSurroundMap.Find(InLocation));

				
			}
			// Pressing the empty
			else
			{
				CheckEmptySpacesMain(InLocation);
				//InWidget->DisplayEmptyStyle();
			}

			// Check if any grid left. Win game if no.
			if (OpenedMap.Num() == ((GridWidth * GridHeight) - GridMines))
			{
				WinGameMain();
			}
		}
		// No empty grid, Win the Game!
		else
		{
			WinGameMain();
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
		int tempW = FMath::RandHelper(InWidth);
		int tempH = FMath::RandHelper(InHeight);

		// Only accept when there is no same mine location
		if (!tempMinesLocations.Contains(FIntPoint(tempW, tempH)))
		{
			tempMinesLocations.Add(FIntPoint(tempW, tempH));
		}
	}

	return tempMinesLocations;
}

TMap<FIntPoint, int> FMinesweeperModule::CalculateNumberOfMinesMap()
{
	// Local Map
	TMap<FIntPoint, int>* tempMap = new TMap<FIntPoint, int>();

	TArray<FIntPoint> tempArray;
	GeneratedButtonIDs.GetKeys(tempArray);

	for (FIntPoint it : tempArray)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Current it: %d, %d"), it.X, it.Y);

		if (CheckIsMines(it)) //MinesMap.Contains((it.X, it.Y)
		{
			FIntPoint tempGridOP = FIntPoint(it.X, it.Y);
			tempMap->Add(tempGridOP, -1);

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

			for (FIntPoint tempLoc : *tempNeighbor)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Current Looping Neighbor: %d, %d"), tempLoc.X, tempLoc.Y);

				if (CheckIsMines(tempLoc))
				{
					tempNumber += 1;
				}
			}

			FIntPoint tempGridOP = FIntPoint(tempIT_X, tempIT_Y);
			tempMap->Add(tempGridOP, tempNumber);
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
	FText DialogText = FText::FromString(TEXT("You Clicked a Mines!"));

	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FMinesweeperModule::WinGameMain()
{
	FText DialogText = FText::FromString(TEXT("WIN! You Found All Mines!"));

	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FMinesweeperModule::CheckEmptySpacesMain(FIntPoint InStartLocation)
{
	// Display as Empty style
	/*TSharedPtr<SMinesWidget>* tempWidget = GeneratedButtonIDs.Find(InStartLocation);
	tempWidget->Get()->DisplayEmptyStyle();*/

	// Recursion
	CheckEmptySpaceCore(InStartLocation, 0);
}

int FMinesweeperModule::CheckEmptySpaceCore(FIntPoint InLocation, int InCounter)
{
	UE_LOG(LogTemp, Warning, TEXT("P1 Entry: Location: %d,%d."), InLocation.X, InLocation.Y);
	//Debug_LogFIntPoint(OpenedMap, "P1 Before OpenedMap");

	// Add to OpenedMap
	OpenedMap.Add(InLocation);

	// Display as Empty style
	TSharedPtr<SMinesWidget>* tempWidget = GeneratedButtonIDs.Find(InLocation);
	tempWidget->Get()->DisplayEmptyStyle();

	int tempCounter = InCounter;

	//FIntPoint tempLoc = FIntPoint();
	//tempLoc = InLocation;
	//int tempX = InLocation.X;
	//int tempY = InLocation.Y;

	//int tempXLeft = int();
	//tempXLeft = tempX - 1;
	//int tempXRight =int();
	//tempXRight = tempX + 1;
	//int tempYUp = int();
	//tempYUp = tempY - 1;
	//int tempYDown = int();
	//tempYDown = tempY + 1;

	//FIntPoint tempUp =  FIntPoint();
	//tempUp = FIntPoint(tempX, tempYUp); //*&
	//FIntPoint tempDown = FIntPoint();
	//tempDown = FIntPoint(tempX, tempYDown);
	//FIntPoint tempLeft =  FIntPoint();
	//tempLeft = FIntPoint(tempXLeft, tempY);
	//FIntPoint tempRight = FIntPoint();
	//tempRight = FIntPoint(tempXRight, tempY);

	//FIntPoint tempNewTryPoint_Up =  FIntPoint();
	//tempNewTryPoint_Up = FIntPoint(tempLoc + tempUp);
	////const FIntPoint tempUp_Loc = tempNewTryPoint_Up;
	//int tempUp_Number = int();
	//if (NumberOfMinesSurroundMap.Contains(tempNewTryPoint_Up))
	//{
	//	tempUp_Number = *NumberOfMinesSurroundMap.Find(tempNewTryPoint_Up);
	//	bool tempUp_HaveNumber = (tempUp_Number == 0) ? true : false;

	//	UE_LOG(LogTemp, Warning, TEXT("Up Location: %d, %d"), tempNewTryPoint_Up.X, tempNewTryPoint_Up.Y);
	//
	//	if (NumberOfMinesSurroundMap.Contains(tempNewTryPoint_Up) && tempUp_HaveNumber && !OpenedMap.Contains(tempNewTryPoint_Up)) //if (NumberOfMinesSurroundMap.Contains(*tempNewTryPoint_Up) && *NumberOfMinesSurroundMap.Find(tempNewTryPoint_Up) == 0 && !OpenedMap.Contains(tempNewTryPoint_Up))

	//	{
	//		int tempDebug1 = int();
	//		tempDebug1 = tempNewTryPoint_Up.X;
	//		UE_LOG(LogTemp, Warning, TEXT("P3: Up Location: %d,%d"), tempNewTryPoint_Up.X, tempNewTryPoint_Up.Y);
	//		//UE_LOG(LogTemp, Warning, TEXT("P3: Up Location: %d,%d"), *&tempNewTryPoint_Up.X, *&tempNewTryPoint_Up.Y);
	//		CheckEmptySpaceCore(tempNewTryPoint_Up);
	//	}
	//}

	// Local it
	int tempLoc_X = InLocation.X;
	int tempLoc_Y = InLocation.Y;

	// Check 4 direction
	TArray<FIntPoint>* tempCheckList = new TArray<FIntPoint>;
	int tempLeft = tempLoc_X - 1;
	int tempRight = tempLoc_X + 1;
	int tempUp = tempLoc_Y - 1;
	int tempDown = tempLoc_Y + 1;

	FIntPoint tempGridUp = FIntPoint(tempLoc_X, tempUp);
	FIntPoint tempGridDown = FIntPoint(tempLoc_X, tempDown);
	FIntPoint tempGridLeft = FIntPoint(tempLeft, tempLoc_Y);
	FIntPoint tempGridRight = FIntPoint(tempRight, tempLoc_Y);
	tempCheckList->Add(tempGridUp);
	tempCheckList->Add(tempGridDown);
	tempCheckList->Add(tempGridLeft);
	tempCheckList->Add(tempGridRight);

	for (FIntPoint tempLoc : *tempCheckList)
	{
		UE_LOG(LogTemp, Warning, TEXT("P3 Check Empty Location: %d, %d"), tempLoc.X, tempLoc.Y);

		// Prevent exception
		if (NumberOfMinesSurroundMap.Contains(tempLoc))
		{
			int tempNumber = *NumberOfMinesSurroundMap.Find(tempLoc);
			bool tempIsZeroNumber = (tempNumber == 0) ? true : false;

			//Only do the non opened location
			if (!OpenedMap.Contains(tempLoc))
			{
				// If that location is a empty location?
				if (tempIsZeroNumber)
				{
					if (NumberOfMinesSurroundMap.Contains(tempLoc)) // && !OpenedMap.Contains(tempLoc)
					{
						tempCounter += 1;

						tempCounter = CheckEmptySpaceCore(tempLoc, tempCounter);
					}
				}
				// It is a number grid
				else
				{
					OpenedMap.Add(tempLoc);

					TSharedPtr<SMinesWidget>* tempWidget2 = GeneratedButtonIDs.Find(tempLoc);
					tempWidget2->Get()->DisplayNumberStyle(tempNumber);
				}
			}
		}
	}

	
	if (tempCounter > 0)
	{
		// After recursion
		// Check 4 Corner
		//TArray<FIntPoint>* tempCheckList_Corner = new TArray<FIntPoint>;

		// Local it
		//int tempLoc_X_Coun = InLocation.X;
		//int tempLoc_Y = InLocation.Y;


		UE_LOG(LogTemp, Warning, TEXT("[C1] Corner Start: Location:  %d, %d"), tempLoc_X, tempLoc_Y);

		FIntPoint tempGridUpCorner = FIntPoint(tempLoc_X, tempUp);
		FIntPoint tempGridDownCorner = FIntPoint(tempLoc_X, tempDown);
		FIntPoint tempGridLeftCorner = FIntPoint(tempLeft, tempLoc_Y);
		FIntPoint tempGridRightCorner = FIntPoint(tempRight, tempLoc_Y);

		FIntPoint tempGridLeftUpCorner = FIntPoint(tempLeft, tempUp);
		FIntPoint tempGridRightUpCorner = FIntPoint(tempRight, tempUp);
		FIntPoint tempGridLeftDownCorner = FIntPoint(tempLeft, tempDown);
		FIntPoint tempGridRightDownCorner = FIntPoint(tempRight, tempDown);

		UE_LOG(LogTemp, Warning, TEXT("[C2] tempGridUpCorner: %d, %d"), tempGridUpCorner.X, tempGridUpCorner.Y);
		UE_LOG(LogTemp, Warning, TEXT("[C2] tempGridDownCorner: %d, %d"), tempGridDownCorner.X, tempGridDownCorner.Y);
		UE_LOG(LogTemp, Warning, TEXT("[C2] tempGridLeftCorner: %d, %d"), tempGridLeftCorner.X, tempGridLeftCorner.Y);
		UE_LOG(LogTemp, Warning, TEXT("[C2] tempGridRightCorner: %d, %d"), tempGridRightCorner.X, tempGridRightCorner.Y);

		UE_LOG(LogTemp, Warning, TEXT("[C3] LeftUpCorner: %d, %d"), tempGridLeftUpCorner.X, tempGridLeftUpCorner.Y);
		UE_LOG(LogTemp, Warning, TEXT("[C3] RightUpCorner: %d, %d"), tempGridRightUpCorner.X, tempGridRightUpCorner.Y);
		UE_LOG(LogTemp, Warning, TEXT("[C3] LeftDownCorner: %d, %d"), tempGridLeftDownCorner.X, tempGridLeftDownCorner.Y);
		UE_LOG(LogTemp, Warning, TEXT("[C3] RightDownCorner: %d, %d"), tempGridRightDownCorner.X, tempGridRightDownCorner.Y);

		bool tempUp_Contains = OpenedMap.Contains(tempGridUpCorner);
		bool tempLeft_Contains = OpenedMap.Contains(tempGridLeftCorner);
		bool tempDown_Contains = OpenedMap.Contains(tempGridDownCorner);
		bool tempRight_Contains = OpenedMap.Contains(tempGridRightCorner);

		UE_LOG(LogTemp, Warning, TEXT("[C4] Left Up Condition: Left %s, Up %s"), (tempLeft_Contains? TEXT("True") : TEXT("False")), (tempUp_Contains ? TEXT("True") : TEXT("False")));

		// Left Up
		if (tempUp_Contains && tempLeft_Contains) //OpenedMap.Contains(tempGridUpCorner)  && OpenedMap.Contains(tempGridLeftCorner)
		{
			UE_LOG(LogTemp, Warning, TEXT("[C5] Inside Left Up"));

			// Only in not opened
			if (!OpenedMap.Contains(tempGridLeftUpCorner))
			{
				// The corner number
				int tempCornerNumber = *NumberOfMinesSurroundMap.Find(tempGridLeftUpCorner);

				// If that is number grid
				if (tempCornerNumber > 0)
				{
					OpenedMap.Add(tempGridLeftUpCorner);

					TSharedPtr<SMinesWidget>* tempWidgetCorner = GeneratedButtonIDs.Find(tempGridLeftUpCorner);
					tempWidgetCorner->Get()->DisplayNumberStyle(*NumberOfMinesSurroundMap.Find(tempGridLeftUpCorner));
				}
				// That is 0 empty grid, keep recursion
				else
				{
					tempCounter += 1;

					tempCounter = CheckEmptySpaceCore(tempGridLeftUpCorner, tempCounter);
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("[C4] Right Up Condition: Right %s, Up %s"), (tempRight_Contains ? TEXT("True") : TEXT("False")), (tempUp_Contains ? TEXT("True") : TEXT("False")));

		// Right Up
		if (tempUp_Contains && tempRight_Contains) //OpenedMap.Contains(tempGridUpCorner) && OpenedMap.Contains(tempGridRightCorner)
		{
			UE_LOG(LogTemp, Warning, TEXT("[C5] Inside Right Up"));

			if (!OpenedMap.Contains(tempGridRightUpCorner))
			{
				// The corner number
				int tempCornerNumber = *NumberOfMinesSurroundMap.Find(tempGridRightUpCorner);

				// If that is number grid
				if (tempCornerNumber > 0)
				{
					OpenedMap.Add(tempGridRightUpCorner);
					TSharedPtr<SMinesWidget>* tempWidgetCorner = GeneratedButtonIDs.Find(tempGridRightUpCorner);
					tempWidgetCorner->Get()->DisplayNumberStyle(*NumberOfMinesSurroundMap.Find(tempGridRightUpCorner));
				}
				// That is 0 empty grid, keep recursion
				else
				{
					tempCounter += 1;

					tempCounter = CheckEmptySpaceCore(tempGridRightUpCorner, tempCounter);
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("[C4] Left Down Condition: Left %s, Down %s"), (tempLeft_Contains ? TEXT("True") : TEXT("False")), (tempDown_Contains ? TEXT("True") : TEXT("False")));

		// Left Down
		if (OpenedMap.Contains(tempGridDownCorner) && OpenedMap.Contains(tempGridLeftCorner))
		{
			UE_LOG(LogTemp, Warning, TEXT("[C5] Inside Left Down"));

			if (!OpenedMap.Contains(tempGridLeftDownCorner))
			{
				// The corner number
				int tempCornerNumber = *NumberOfMinesSurroundMap.Find(tempGridLeftDownCorner);

				// If that is number grid
				if (tempCornerNumber > 0)
				{
					OpenedMap.Add(tempGridLeftDownCorner);
					TSharedPtr<SMinesWidget>* tempWidgetCorner = GeneratedButtonIDs.Find(tempGridLeftDownCorner);
					tempWidgetCorner->Get()->DisplayNumberStyle(*NumberOfMinesSurroundMap.Find(tempGridLeftDownCorner));
				}
				// That is 0 empty grid, keep recursion
				else
				{
					tempCounter += 1;

					tempCounter = CheckEmptySpaceCore(tempGridLeftDownCorner, tempCounter);
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("[C4] Right Down Condition: Right %s, Down %s"), (tempRight_Contains ? TEXT("True") : TEXT("False")), (tempDown_Contains ? TEXT("True") : TEXT("False")));

		// Right Down
		if (OpenedMap.Contains(tempGridDownCorner) && OpenedMap.Contains(tempGridRightCorner))
		{
			UE_LOG(LogTemp, Warning, TEXT("[C5] Inside Right Down"));

			if (!OpenedMap.Contains(tempGridRightDownCorner))
			{
				// The corner number
				int tempCornerNumber = *NumberOfMinesSurroundMap.Find(tempGridRightDownCorner);

				// If that is number grid
				if (tempCornerNumber > 0)
				{
					OpenedMap.Add(tempGridRightDownCorner);
					TSharedPtr<SMinesWidget>* tempWidgetCorner = GeneratedButtonIDs.Find(tempGridRightDownCorner);
					tempWidgetCorner->Get()->DisplayNumberStyle(*NumberOfMinesSurroundMap.Find(tempGridRightDownCorner));
				}
				// That is 0 empty grid, keep recursion
				else
				{
					tempCounter += 1;

					tempCounter = CheckEmptySpaceCore(tempGridRightDownCorner, tempCounter);
				}
			}
		}

		//for (FIntPoint tempLoc_Corner : *tempCheckList_Corner)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("P3 Check Empty Location: %d, %d"), tempLoc_Corner.X, tempLoc_Corner.Y);

		//	if (InLocation)
		//		//if (NumberOfMinesSurroundMap.Contains(tempLoc_Corner))
		//		//{
		//		//	int tempNumber = *NumberOfMinesSurroundMap.Find(tempLoc);
		//		//	bool tempIsZeroNumber = (tempNumber == 0) ? true : false;

		//		//	// If that location is a empty location?
		//		//	if (tempIsZeroNumber)
		//		//	{
		//		//		if (NumberOfMinesSurroundMap.Contains(tempLoc) && !OpenedMap.Contains(tempLoc))
		//		//		{
		//		//			CheckEmptySpaceCore(tempLoc);
		//		//		}
		//		//	}
		//		//	// It is a number grid
		//		//	else
		//		//	{
		//		//		TSharedPtr<SMinesWidget>* tempWidget2 = GeneratedButtonIDs.Find(tempLoc);
		//		//		tempWidget2->Get()->DisplayNumberStyle(tempNumber);
		//		//	}

		//		//}
		//}
	}
	
	


	//FIntPoint* tempNewTryPoint_Down = new FIntPoint();
	//*tempNewTryPoint_Down = FIntPoint(*tempLoc + *tempDown);
	//if (NumberOfMinesSurroundMap.Contains(*tempNewTryPoint_Down) && *NumberOfMinesSurroundMap.Find(*tempNewTryPoint_Down) == 0 && !OpenedMap.Contains(*tempNewTryPoint_Down))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("P3: Down Location: %d,%d"), *&tempNewTryPoint_Down->X, *&tempNewTryPoint_Down->Y);

	//	CheckEmptySpaceCore(*tempNewTryPoint_Down);
	//}

	//FIntPoint* tempNewTryPoint_Left = new FIntPoint();
	//*tempNewTryPoint_Left = FIntPoint(*tempLoc + *tempLeft);
	//if (NumberOfMinesSurroundMap.Contains(*tempNewTryPoint_Left) && *NumberOfMinesSurroundMap.Find(*tempNewTryPoint_Left) == 0 && !OpenedMap.Contains(*tempNewTryPoint_Left))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("P3: Left Location: %d,%d"), *&tempNewTryPoint_Left->X, *&tempNewTryPoint_Left->Y);

	//	CheckEmptySpaceCore(*tempNewTryPoint_Left);
	//}

	//FIntPoint* tempNewTryPoint_Right = new FIntPoint();
	//*tempNewTryPoint_Right = FIntPoint(*tempLoc + *tempRight);
	//if (NumberOfMinesSurroundMap.Contains(*tempNewTryPoint_Right) && *NumberOfMinesSurroundMap.Find(*tempNewTryPoint_Right) == 0 && !OpenedMap.Contains(*tempNewTryPoint_Right))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("P3: Right Location: %d,%d"), *&tempNewTryPoint_Right->X, *&tempNewTryPoint_Right->Y);

	//	CheckEmptySpaceCore(*tempNewTryPoint_Right);
	//}




	return tempCounter;
}

void FMinesweeperModule::Debug_LogFIntPoint(TArray<FIntPoint> InArray, FString InPrefix)
{
	FString* tempDebug_LogFIntPoint = new FString();

	tempDebug_LogFIntPoint->Append(InPrefix);

	for (auto it : InArray)
	{
		tempDebug_LogFIntPoint->Append("[");
		tempDebug_LogFIntPoint->Append(FString::FromInt(it.X));
		tempDebug_LogFIntPoint->Append(",");
		tempDebug_LogFIntPoint->Append(FString::FromInt(it.Y));
		tempDebug_LogFIntPoint->Append("]");
	}

	UE_LOG(LogTemp, Warning, TEXT("%s: %s"), &InPrefix, tempDebug_LogFIntPoint);
	/*FText tempDebug2 = FText::FromString(tempDebug1);
	FMessageDialog::Open(EAppMsgType::Ok, tempDebug2);*/

}

void FMinesweeperModule::Debug_LogFIntPointInt(TMap<FIntPoint, int> InMap, FString InPrefix)
{
	FString* tempDebug1 = new FString();

	//tempDebug1 += InPrefix;
	tempDebug1->Append(InPrefix);

	for (auto it : InMap)
	{
		tempDebug1->Append("[");
		tempDebug1->Append(FString::FromInt(it.Key.X));
		tempDebug1->Append(",");
		tempDebug1->Append(FString::FromInt(it.Key.Y));
		tempDebug1->Append(":");
		tempDebug1->Append(FString::FromInt(it.Value));
		tempDebug1->Append("]");
	}

	UE_LOG(LogTemp, Warning, TEXT("%s"), tempDebug1);

	//FText tempDebug2 = FText::FromString(tempDebug1);
	//FMessageDialog::Open(EAppMsgType::Ok, tempDebug2);
	//UE_LOG(LogTemp, Display, tempDebug1);

}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperModule, Minesweeper)