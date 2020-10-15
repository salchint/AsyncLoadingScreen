/************************************************************************************
 *																					*
 * Copyright (C) 2020 Thomas Strunz													*
 * Website:	https://github.com/truong-bui/AsyncMenuScreen							*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "SLoadingWidget.h"
#include <memory>
#include <array>
#include <functional>

/**
 * 
 */
class SVerticalMenuWidget : public SLoadingWidget
{
public:
	/// Sub menu item clicked event handler type
	using SubButtonClickedHandler_t = std::function<void(SVerticalMenuWidget&, int)>;

	/// Indentifiers for the 5 main menu items
	enum { MenuItem0, MenuItem1, MenuItem2, MenuItem3, MenuItem4, N_MainMenuItems };
	using SubMenuClickedHandlers_t = std::array<std::unique_ptr<SubButtonClickedHandler_t>, N_MainMenuItems>;

	/// Indentifiers for the 4 sub menu items
	enum { SubMenuItem0, SubMenuItem1, SubMenuItem2, SubMenuItem3, N_SubMenuItems };
	using TextStyle_t = std::unique_ptr<FTextBlockStyle>;
	using SubBorders_t = std::array<TSharedPtr<SBorder>, N_SubMenuItems>;
	using SubButtons_t = std::array<TSharedPtr<SButton>, N_SubMenuItems>;
	using SubButtonTexts_t = std::array<TSharedPtr<STextBlock>, N_SubMenuItems>;

protected:
	/// Array telling, which main menu item is currently hovered if any
	struct HoverMemory
	{
		std::array<bool, N_MainMenuItems> IsHovered;
		HoverMemory() { reset(); }
		auto reset()->void { IsHovered.fill(false); }
		auto set(int i)->void { reset(); IsHovered[i] = true; }
		auto operator[](int i)->bool& { return IsHovered[i]; }
	};

protected:
	SubButtons_t SubButtons;
	SubBorders_t SubBorders;
	SubButtonTexts_t SubButtonTexts;
	TextStyle_t NormalText;
	TextStyle_t NormalTextSat;

	HoverMemory itemHovered;
	SubMenuClickedHandlers_t SubMenuClickedHandlers;

public:
	SLATE_BEGIN_ARGS(SVerticalMenuWidget) {}

	SLATE_END_ARGS()

	//void Construct(const FArguments& InArgs, const FLoadingWidgetSettings& Settings);
	void Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings);

private:
	TSharedPtr<FDeferredCleanupSlateBrush> ImageBrush;

public:
void PlayClicked(int subButtonIdx);
void CutSceneClicked(int subButtonIdx);
void SettingClicked(int subButtonIdx);
void TrainingClicked(int subButtonIdx);
void TeamsClicked(int subButtonIdx);
FReply OnQuitClicked();

FReply OnPlayClicked();
FReply OnTeamClicked();
FReply OnTrainingClicked();
FReply OnCutClicked();
FReply OnOtherClicked();
FReply OnClicked();
void OnPlayHovered();
void OnTeamHovered();
void OnTrainingHovered();
void OnCutHovered();
void OnOtherHovered();
void OnHovered();
};
