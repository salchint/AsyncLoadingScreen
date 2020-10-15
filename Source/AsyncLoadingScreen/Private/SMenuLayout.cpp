/************************************************************************************
 *																					*
 * Copyright (C) 2020 Thomas Strunz													*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "SMenuLayout.h"
#include "LoadingScreenSettings.h"
#include "Widgets/Layout/SSafeZone.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "SHorizontalLoadingWidget.h"
#include "SVerticalMenuWidget.h"
#include "SBackgroundWidget.h"
#include "STipWidget.h"

void SMenuLayout::Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FCenterLayoutSettings& LayoutSettings)
{
	// Root widget and background
	TSharedRef<SOverlay> Root = SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBackgroundWidget, Settings.Background)
		];

	// Placeholder for loading widget
	TSharedRef<SWidget> LoadingWidget = SNullWidget::NullWidget;
	if (Settings.LoadingWidget.LoadingWidgetType == ELoadingWidgetType::LWT_Horizontal)
	{
		//LoadingWidget = SNew(SVerticalMenuWidget, Settings.LoadingWidget);
		LoadingWidget = SNew(SVerticalMenuWidget, Settings);
	}
	else
	{
		LoadingWidget = SNew(SVerticalMenuWidget, Settings);
	}

	// Add loading widget at center
	Root->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			LoadingWidget
		];

	if (LayoutSettings.bIsTipAtBottom)
	{
		// Add tip widget at bottom
		Root->AddSlot()
			.HAlign(LayoutSettings.BorderHorizontalAlignment)
			.VAlign(VAlign_Bottom)
			.Padding(0, 0, 0, LayoutSettings.BorderVerticalOffset)
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderImage(&LayoutSettings.BorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(LayoutSettings.TipAlignment.HorizontalAlignment)
					.VAlign(LayoutSettings.TipAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(LayoutSettings.BorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SMenuLayout::GetDPIScale)
						[
							SNew(STipWidget, Settings.TipWidget)
						]
					]
				]
			];
	}
	else
	{
		// Add tip widget at top
		Root->AddSlot()
			.HAlign(LayoutSettings.BorderHorizontalAlignment)
			.VAlign(VAlign_Top)
			.Padding(0, LayoutSettings.BorderVerticalOffset, 0, 0)
			[
				SNew(SBorder)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.BorderImage(&LayoutSettings.BorderBackground)
				.BorderBackgroundColor(FLinearColor::White)
				[
					SNew(SSafeZone)
					.HAlign(LayoutSettings.TipAlignment.HorizontalAlignment)
					.VAlign(LayoutSettings.TipAlignment.VerticalAlignment)
					.IsTitleSafe(true)
					.Padding(LayoutSettings.BorderPadding)
					[
						SNew(SDPIScaler)
						.DPIScale(this, &SMenuLayout::GetDPIScale)
						[
							SNew(STipWidget, Settings.TipWidget)
						]
					]
				]
			];
	}

	// Add root to this widget
	ChildSlot
	[
		Root
	];
}
