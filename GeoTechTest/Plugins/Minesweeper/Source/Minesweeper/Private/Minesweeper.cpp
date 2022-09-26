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

	//TSharedRef<SSpinBox<int>, ESPMode::ThreadSafe> Input_WidthSpinBox;

	// Cyrus
	TSharedRef<SWindow> MinesweeperWindow = SNew(SWindow)
		.Title(FText::FromString(TEXT("Minesweeper Window")))
		.ClientSize(FVector2D(800, 400))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
			SNew(SVerticalBox)

			// 1st Row
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
				SNew(STextBlock).Text(FText::FromString(TEXT("Welcome to Minesweeper!!!")))
				]

			// 2nd Row
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
						SNew(SSpacer)//.Size((10, 30))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center).FillWidth(1)
					[
						 SAssignNew(Input_WidthSpinBox, SSpinBox<int>).Value(5).MinFractionalDigits(0).MaxFractionalDigits(0).MinValue(0)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(1)
					[
						SNew(SSpacer)//.Size((10, 30))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(1)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Height")))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(SSpacer)//.Size((10, 30))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center).FillWidth(1)
					[
						SAssignNew(Input_HeightSpinBox, SSpinBox<int>).Value(5).MinFractionalDigits(0).MaxFractionalDigits(0).MinValue(0)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
					[
						SNew(SSpacer)//.Size((10, 30))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(3)
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Number Of Mines")))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(1)
					[
						SNew(SSpacer)//.Size((10, 30))
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Fill).VAlign(VAlign_Center).FillWidth(1)
					[
						SAssignNew(Input_NumberOfMinesSpinBox, SSpinBox<int>).Value(2).MinFractionalDigits(0).MaxFractionalDigits(0).MinValue(1)
					]
				+ SHorizontalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center).FillWidth(1)
					[
						SNew(SSpacer)//.Size((10, 30))
					]
				]

			// 3rd Row
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.Padding((0, 50))
				[
					SAssignNew(Input_GenerateGridButton, SButton)
						.OnClicked(FOnClicked::CreateRaw(this, &FMinesweeperModule::GenerateGridButtonClicked)) //OnClicked_Raw(this, &FMinesweeperModule::GenerateGridButtonClicked
						
					[
						SAssignNew(Input_GenerateGridLabel, STextBlock)
							.Text(FText::FromString(TEXT("Generate New Grid")))
							.Justification(ETextJustify::Center)
					]					
				]

			// 4th Row
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.FillHeight(5)
				[
					SAssignNew(GridRoot, SVerticalBox)
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

FReply FMinesweeperModule::GenerateGridButtonClicked() 
{
	Input_GenerateGridLabel.Get()->SetText(FText::FromString(TEXT("Generated")));

	GenerateGridMain(0, 0, 0);

	return FReply::Handled();
}

void FMinesweeperModule::GenerateGridMain(int Width, int Height, int Mines)
{
	//GridRoot + SVerticalBox::Slot();
	//GridRoot->AddSlot().AttachWidget(Input_GenerateGridLabel);

	// This can be compiled, but didn't show up
	/*auto temp1 = GridRoot->AddSlot();
	temp1
		[
			SNew(SButton)
		];*/
	GridRoot->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			[
				SAssignNew(Input_GenerateGridLabel, STextBlock)
				.Text(FText::FromString(TEXT("Generate New Grid")))
				.Justification(ETextJustify::Center)
			]
		];
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperModule, Minesweeper)