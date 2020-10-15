/************************************************************************************
 *																					*
 * Copyright (C) 2020 Truong Bui.													*
 * Website:	https://github.com/truong-bui/AsyncMenuScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SVerticalMenuWidget.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Images/SImage.h"
#include "Slate/DeferredCleanupSlateBrush.h"
#include "Widgets/Text/STextBlock.h"

#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateStyle.h"
#include "Widgets/Input/SButton.h"
#include "Brushes/SlateColorBrush.h"
#include "Math/Color.h"
#include "Layout/Margin.h"
#include "Internationalization/Text.h"

#include <ctime>
#include <string>
#include <memory>
#include <sstream>
#include <iomanip>

#define LOCTEXT_NAMESPACE "MainMenu"

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)
static auto COLOR_BRUSH_BLUE = FSlateColorBrush( FLinearColor(.3, .35, .75) );
static auto COLOR_BRUSH_WHITE = FSlateColorBrush( FLinearColor(.8, .8, .8) );
static auto COLOR_BRUSH_BLUE_SAT = FSlateColorBrush( FLinearColor(.4, .45, .95) );

using std::asctime;
using std::gmtime;
using std::setw;
using std::setfill;
using std::time_t;
using std::put_time;


namespace {
	/// Caption texts
	const FText TitleText = LOCTEXT("GameTitle", "Rugby League 21");
	const FText PlayMenu = LOCTEXT("PlayMenu", "Play Game");
	const FText PlayText = LOCTEXT("PlayGame", "Quick Match");
	const FText OnlineText = LOCTEXT("Online", "Online Match");
	const FText CutMenu = LOCTEXT("CutScenes", "CutScene\nDemos");
	const FText SettingsText = LOCTEXT("Settings", "Settings");
	const FText CreditsText = LOCTEXT("Credits", "Credits");
	const FText TrainingMenu = LOCTEXT("Training", "Training");
	const FText TeamMenu = LOCTEXT("Team", "Teams\nPlayers");
	const FText QuitText = LOCTEXT("QuitGame", "Quit Game");
	const FText OtherMenu = LOCTEXT("OtherMenu", "Settings");
	FText SubText0 = LOCTEXT("SubText0", "Quick Match");
	FText SubText1 = LOCTEXT("SubText1", "Online Game");
	FText SubText2 = LOCTEXT("SubText2", "Saved Game");
	FText SubText3 = LOCTEXT("SubText3", "");

	/// Padding of the menu page
	const FMargin ContentPadding = FMargin(100.f, 100.f);
	/// Padding of the tiles
	const FMargin ButtonPadding = FMargin(10.f);
	const FMargin ButtonPadding1 = FMargin(10.f);
	const FMargin ButtonPadding2 = FMargin(10.f);
	const FMargin ButtonPadding3 = FMargin(1.f);
	const FMargin ButtonPadding4 = FMargin(1.f);

	//std::unique_ptr<FButtonStyle> NormalStyle;
	FButtonStyle* NormalStyle = nullptr;

	/// Text style for tiles
	FSlateFontInfo ButtonTextStyle = FCoreStyle::GetDefaultFontStyle("Regular", 24);
	FSlateFontInfo ButtonTextStyle1 = FCoreStyle::GetDefaultFontStyle("Regular", 20);

	/// Text style for title
	FSlateFontInfo TitleTextStyle = FCoreStyle::GetDefaultFontStyle("Regular", 40);

	/// Identifier for the menu enties
	enum { MainButton0, MainButton1, MainButton2, MainButton3, MainButton4 };

	/// Identifier for the active main menu
	enum { MainActive0, MainActive1, MainActive2, MainActive3, MainActive4 };

	/// Build the margins for the individual menu entries
	template<int IMainActive, int IPos> struct BuildButtonPadding { const FMargin operator()() { return FMargin(10.f); } };
	template<> struct BuildButtonPadding<MainActive0, MainButton0> { const FMargin operator()() { return FMargin(1.f); } };
	template<> struct BuildButtonPadding<MainActive1, MainButton1> { const FMargin operator()() { return FMargin(1.f); } };
	template<> struct BuildButtonPadding<MainActive2, MainButton2> { const FMargin operator()() { return FMargin(1.f); } };
	template<> struct BuildButtonPadding<MainActive3, MainButton3> { const FMargin operator()() { return FMargin(1.f); } };
	template<> struct BuildButtonPadding<MainActive4, MainButton4> { const FMargin operator()() { return FMargin(1.f); } };

	/// Create the widgets representing the tiles caption
	TSharedRef<SWidget> CreateCaption(TSharedPtr<SBorder> border, TSharedPtr<STextBlock> buttonText, const FText& text, const SVerticalMenuWidget::TextStyle_t& style)
	{
		//SAssignNew(border, SBorder).Padding(ButtonPadding).BorderImage(&COLOR_BRUSH_BLUE)
		SAssignNew(border, SBorder).Padding(ButtonPadding)
			[
				SAssignNew(buttonText, STextBlock).Text(text).TextStyle(style.get())
			];
		return border.ToSharedRef();
	}

	/// Create the top menu row
	template<int IMainActive>
	TSharedRef<SWidget> CreateMainMenu(const SVerticalMenuWidget* gamemenu)
	{
		TSharedPtr<SHorizontalBox> box;
		SAssignNew(box, SHorizontalBox)
			+ SHorizontalBox::Slot().Padding(BuildButtonPadding<IMainActive, MainButton0>()())[SNew(SButton).OnClicked(gamemenu, &SVerticalMenuWidget::OnPlayClicked)																	[SNew(STextBlock).Font(ButtonTextStyle).Text(PlayMenu)		.Justification(ETextJustify::Center)	]]
			+ SHorizontalBox::Slot().Padding(BuildButtonPadding<IMainActive, MainButton1>()())[SNew(SButton).OnClicked(gamemenu, &SVerticalMenuWidget::OnTeamClicked)																	[SNew(STextBlock).Font(ButtonTextStyle).Text(TeamMenu)		.Justification(ETextJustify::Center)	]]
			+ SHorizontalBox::Slot().Padding(BuildButtonPadding<IMainActive, MainButton2>()())[SNew(SButton).OnClicked(gamemenu, &SVerticalMenuWidget::OnTrainingClicked).OnHovered(gamemenu, &SVerticalMenuWidget::OnTrainingHovered)	[SNew(STextBlock).Font(ButtonTextStyle).Text(TrainingMenu)	.Justification(ETextJustify::Center)	]]
			+ SHorizontalBox::Slot().Padding(BuildButtonPadding<IMainActive, MainButton3>()())[SNew(SButton).OnClicked(gamemenu, &SVerticalMenuWidget::OnCutClicked).OnHovered(gamemenu, &SVerticalMenuWidget::OnCutHovered)			[SNew(STextBlock).Font(ButtonTextStyle).Text(CutMenu)		.Justification(ETextJustify::Center)	]]
			+ SHorizontalBox::Slot().Padding(BuildButtonPadding<IMainActive, MainButton4>()())[SNew(SButton).OnClicked(gamemenu, &SVerticalMenuWidget::OnOtherClicked).OnHovered(gamemenu, &SVerticalMenuWidget::OnOtherHovered)		[SNew(STextBlock).Font(ButtonTextStyle).Text(OtherMenu)		.Justification(ETextJustify::Center)	]];
		//+ SHorizontalBox::Slot().Padding(ButtonPadding)[SNew(SButton).OnClicked(this, &SVerticalMenuWidget::OnQuitClicked) [SNew(STextBlock).Font(ButtonTextStyle).Text(QuitText).Justification(ETextJustify::Center)]]
		return box.ToSharedRef();
	}

	/// Create a sub-menu button
	TSharedRef<SWidget> CreateSubMenuButton(const SVerticalMenuWidget* gamemenu, TSharedPtr<SButton>& subButton, const FButtonStyle* normalStyle, const SVerticalMenuWidget::SubButtonClickedHandler_t& clickedHandler, int subButtonIdx, TSharedRef<SWidget>& caption)
	{
		SAssignNew(subButton, SButton).ButtonStyle(normalStyle).OnClicked_Lambda([=]()->FReply {clickedHandler(*((SVerticalMenuWidget*)gamemenu), subButtonIdx); return FReply::Handled();})
			.HAlign(HAlign_Center).VAlign(VAlign_Center) [ caption ];
		return subButton.ToSharedRef();
	}

	/// Create the second menu row with 4 tiles
	TSharedRef<SWidget> CreateSubMenu( const SVerticalMenuWidget* gamemenu, SVerticalMenuWidget::SubButtons_t& SubButtons, const SVerticalMenuWidget::SubBorders_t& SubBorders, const SVerticalMenuWidget::SubButtonTexts_t& SubButtonTexts, const SVerticalMenuWidget::SubButtonClickedHandler_t& clickedHandler, const SVerticalMenuWidget::TextStyle_t& NormalText, const FButtonStyle* normalStyle )
	{
		auto caption0 = CreateCaption(SubBorders[0], SubButtonTexts[0], SubText0, NormalText);
		auto caption1 = CreateCaption(SubBorders[1], SubButtonTexts[1], SubText1, NormalText);
		auto caption2 = CreateCaption(SubBorders[2], SubButtonTexts[2], SubText2, NormalText);
		auto caption3 = CreateCaption(SubBorders[3], SubButtonTexts[3], SubText3, NormalText);

		auto button0 = CreateSubMenuButton(gamemenu, SubButtons[0], normalStyle, clickedHandler, SVerticalMenuWidget::SubMenuItem0, caption0);
		auto button1 = CreateSubMenuButton(gamemenu, SubButtons[1], normalStyle, clickedHandler, SVerticalMenuWidget::SubMenuItem1, caption1);
		auto button2 = CreateSubMenuButton(gamemenu, SubButtons[2], normalStyle, clickedHandler, SVerticalMenuWidget::SubMenuItem2, caption2);
		auto button3 = CreateSubMenuButton(gamemenu, SubButtons[3], normalStyle, clickedHandler, SVerticalMenuWidget::SubMenuItem3, caption3);
		UE_LOG(LogClass, Log, TEXT("Menu: Created sub-menu buttons with handler @ %p: %s"), clickedHandler.target<void(__cdecl SVerticalMenuWidget::*)(int)>(), *FString(clickedHandler.target_type().name()));

		TSharedPtr<SHorizontalBox> box;
		SAssignNew(box, SHorizontalBox)
		+ SHorizontalBox::Slot().Padding(ButtonPadding)[ button0 ]
		+ SHorizontalBox::Slot().Padding(ButtonPadding)[ button1 ]
		+ SHorizontalBox::Slot().Padding(ButtonPadding)[ button2 ]
		+ SHorizontalBox::Slot().Padding(ButtonPadding)[ button3 ];
		return box.ToSharedRef();
	}
}

void SVerticalMenuWidget::Construct(const FArguments& InArgs, const FALoadingScreenSettings& ParentSettings)
{
	UE_LOG(LogClass, Log, TEXT("Menu: Construct"));
	const FLoadingWidgetSettings& Settings = ParentSettings.LoadingWidget;

	NormalStyle = new FButtonStyle();
	//NormalStyle->SetNormal	(COLOR_BRUSH_BLUE);
	//NormalStyle->SetHovered	(COLOR_BRUSH_WHITE);
	//NormalStyle->SetPressed	(COLOR_BRUSH_BLUE);
	//NormalStyle->SetDisabled(COLOR_BRUSH_BLUE);

	NormalText.reset(new FTextBlockStyle());
	NormalText->SetFont( DEFAULT_FONT("Regular", 17.f) );
	//NormalText->SetColorAndOpacity(FSlateColor::UseForeground());
	NormalText->SetColorAndOpacity(FLinearColor::White);
	NormalText->SetShadowOffset(FVector2D::ZeroVector);
	NormalText->SetShadowColorAndOpacity(FLinearColor::Black);
	NormalText->SetHighlightColor( FLinearColor( 0.02f, 0.3f, 0.0f ) );

	SubMenuClickedHandlers[MenuItem0] = std::make_unique<SubButtonClickedHandler_t>(&SVerticalMenuWidget::PlayClicked);
	SubMenuClickedHandlers[MenuItem1] = std::make_unique<SubButtonClickedHandler_t>(&SVerticalMenuWidget::TeamsClicked);
	SubMenuClickedHandlers[MenuItem2] = std::make_unique<SubButtonClickedHandler_t>(&SVerticalMenuWidget::TrainingClicked);
	SubMenuClickedHandlers[MenuItem3] = std::make_unique<SubButtonClickedHandler_t>(&SVerticalMenuWidget::CutSceneClicked);
	SubMenuClickedHandlers[MenuItem4] = std::make_unique<SubButtonClickedHandler_t>(&SVerticalMenuWidget::SettingClicked);

	// If there's an image defined
	if (ParentSettings.Background.Images.Num() > 0)
	{
		int32 imageIndex = FMath::RandRange(0, ParentSettings.Background.Images.Num() - 1);

		if (ParentSettings.Background.bSetDisplayBackgroundManually == true)
		{
			if (ParentSettings.Background.Images.IsValidIndex(UAsyncLoadingScreenLibrary::GetDisplayBackgroundIndex()))
			{
				imageIndex = UAsyncLoadingScreenLibrary::GetDisplayBackgroundIndex();
			}
		}
		UE_LOG(LogClass, Log, TEXT("Menu: Selected image %d"), imageIndex);

		const FSoftObjectPath& ImageAsset = ParentSettings.Background.Images[imageIndex];
		UObject* ImageObject = ImageAsset.TryLoad();
		if (UTexture2D* LoadingImage = Cast<UTexture2D>(ImageObject))
		{
			UE_LOG(LogClass, Log, TEXT("Menu: Got image as texture"))
			ImageBrush = FDeferredCleanupSlateBrush::CreateBrush(LoadingImage);
			auto brush = FDeferredCleanupSlateBrush::TrySlateBrush(ImageBrush);
			if (brush)
			{
				UE_LOG(LogClass, Log, TEXT("Menu: Got image as brush"))
				NormalStyle->SetNormal(*brush);
				NormalStyle->SetHovered(*brush);
				NormalStyle->SetPressed(*brush);
				NormalStyle->SetDisabled(*brush);
			}
			//ChildSlot
				//[
				//SNew(SBorder)
					//.HAlign(HAlign_Fill)
					//.VAlign(VAlign_Fill)
					//.Padding(Settings.Padding)
					//.BorderBackgroundColor(Settings.BackgroundColor)
					//.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
					//[
					//SNew(SScaleBox)
						//.Stretch(Settings.ImageStretch)
						//[
						//SNew(SImage)
							//.Image(ImageBrush.IsValid() ? ImageBrush->GetSlateBrush() : nullptr)
						//]
					//]
				//];
		}
	}

	auto mainmenu = CreateMainMenu<MainActive0>(this);
	auto submenu = CreateSubMenu(this, SubButtons, SubBorders, SubButtonTexts, *SubMenuClickedHandlers[MenuItem0].get(), NormalText, NormalStyle);

	bIsActiveTimerRegistered = false;
	bPlayReverse = Settings.ImageSequenceSettings.bPlayReverse;

	// Root is a Vertical Box
	TSharedRef<SVerticalBox> Root = SNew(SVerticalBox);

	// Construct Menu Icon Widget
	ConstructLoadingIcon(Settings);

	EVisibility LoadingTextVisibility;

	if (Settings.LoadingText.IsEmpty())
	{
		LoadingTextVisibility = EVisibility::Collapsed;
	}
	else
	{
		LoadingTextVisibility = EVisibility::SelfHitTestInvisible;
	}

	// If loading text is on the top
	if (Settings.bLoadingTextTopPosition)
	{
		// Add Loading Text on the top first
		Root.Get().AddSlot()
			.HAlign(Settings.TextAlignment.HorizontalAlignment)
			.VAlign(Settings.TextAlignment.VerticalAlignment)
			.AutoHeight()
			[
				SNew(STextBlock)
				.Visibility(LoadingTextVisibility)
				.ColorAndOpacity(Settings.Appearance.ColorAndOpacity)
				.Font(Settings.Appearance.Font)
				.ShadowOffset(Settings.Appearance.ShadowOffset)
				.ShadowColorAndOpacity(Settings.Appearance.ShadowColorAndOpacity)
				.Justification(Settings.Appearance.Justification)
				.Text(Settings.LoadingText)
			];

		// Add a Spacer in middle
		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SSpacer)
				.Size(FVector2D(0.0f, Settings.Space))
			];

		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				mainmenu
			];

		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				submenu
			];

		// Add a Spacer in middle
		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SSpacer)
				.Size(FVector2D(0.0f, Settings.Space))
			];

		// Add Loading Icon at the bottom finally
		Root.Get().AddSlot()
			.HAlign(Settings.LoadingIconAlignment.HorizontalAlignment)
			.VAlign(Settings.LoadingIconAlignment.VerticalAlignment)
			.AutoHeight()
			[
				LoadingIcon
			];
	}

	// If loading text is at the bottom
	else
	{
		// Add Loading Icon on the top
		Root.Get().AddSlot()
			.HAlign(Settings.LoadingIconAlignment.HorizontalAlignment)
			.VAlign(Settings.LoadingIconAlignment.VerticalAlignment)
			.AutoHeight()
			[
				LoadingIcon
			];

		// Add a Spacer in middle
		Root.Get().AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SSpacer)
				.Size(FVector2D(0.0f, Settings.Space))
			];

		// Add Loading Text at the bottom
		Root.Get().AddSlot()
			.HAlign(Settings.TextAlignment.HorizontalAlignment)
			.VAlign(Settings.TextAlignment.VerticalAlignment)
			.AutoHeight()
			[
				SNew(STextBlock)
				.Visibility(LoadingTextVisibility)
				.ColorAndOpacity(Settings.Appearance.ColorAndOpacity)
				.Font(Settings.Appearance.Font)
				.ShadowOffset(Settings.Appearance.ShadowOffset)
				.ShadowColorAndOpacity(Settings.Appearance.ShadowColorAndOpacity)
				.Justification(Settings.Appearance.Justification)
				.Text(Settings.LoadingText)
			];
	}

	// Add root to this widget
	ChildSlot
	[
		Root
	];
}

void SVerticalMenuWidget::PlayClicked(int subButtonIdx)
{
	UE_LOG(LogClass, Log, TEXT("Menu: Play %d clicked"), subButtonIdx);
	switch(subButtonIdx) {
		case SubMenuItem0:
		case SubMenuItem1:
		case SubMenuItem2:
		case SubMenuItem3:
			GetMoviePlayer()->StopMovie();
			break;
		default:
			break;
	}
}


void SVerticalMenuWidget::CutSceneClicked(int subButtonIdx)
{
	UE_LOG(LogClass, Log, TEXT("Menu: CutScene %d clicked"), subButtonIdx);
	switch(subButtonIdx) {
		case SubMenuItem0:
		case SubMenuItem1:
		case SubMenuItem2:
		case SubMenuItem3:
			break;
		default: break;
	}
}

void SVerticalMenuWidget::SettingClicked(int subButtonIdx)
{
	UE_LOG(LogClass, Log, TEXT("Menu: Setting %d clicked"), subButtonIdx);
	switch(subButtonIdx) {
		case SubMenuItem0:
		case SubMenuItem1:
		case SubMenuItem2:
			break;
		case SubMenuItem3:
			OnQuitClicked();
			break;
		default: break;
	}
}

void SVerticalMenuWidget::TrainingClicked(int subButtonIdx)
{
	UE_LOG(LogClass, Log, TEXT("Menu: Training %d clicked"), subButtonIdx);
	switch(subButtonIdx) {
		case SubMenuItem0:
		case SubMenuItem1:
		case SubMenuItem2:
		case SubMenuItem3:
			break;
		default: break;
	}
}

void SVerticalMenuWidget::TeamsClicked(int subButtonIdx)
{
	UE_LOG(LogClass, Log, TEXT("Menu: Teams %d clicked"), subButtonIdx);
	switch(subButtonIdx) {
		case SubMenuItem0:
		case SubMenuItem1:
		case SubMenuItem2:
		case SubMenuItem3:
			break;
		default: break;
	}
}

FReply SVerticalMenuWidget::OnPlayClicked()
{
	return FReply::Handled();
}
FReply SVerticalMenuWidget::OnTeamClicked()
{
	return FReply::Handled();
}
FReply SVerticalMenuWidget::OnTrainingClicked()
{
	return FReply::Handled();
}
FReply SVerticalMenuWidget::OnCutClicked()
{
	return FReply::Handled();
}
FReply SVerticalMenuWidget::OnOtherClicked()
{
	return FReply::Handled();
}
FReply SVerticalMenuWidget::OnClicked()
{
	return FReply::Handled();
}
void SVerticalMenuWidget::OnPlayHovered()
{
}
void SVerticalMenuWidget::OnTeamHovered()
{
}
void SVerticalMenuWidget::OnTrainingHovered()
{
}
void SVerticalMenuWidget::OnCutHovered()
{
}
void SVerticalMenuWidget::OnOtherHovered()
{
}
void SVerticalMenuWidget::OnHovered()
{
}
FReply SVerticalMenuWidget::OnQuitClicked()
{
	//if (OwningHUD.IsValid())
	//{
		//if (APlayerController* PC = OwningHUD->PlayerOwner)
		//{
			//PC->ConsoleCommand("quit");
		//}
	//}
	return FReply::Handled();
}
