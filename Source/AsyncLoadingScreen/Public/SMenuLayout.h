/************************************************************************************
 *																					*
 * Copyright (C) 2020 Thomas Strunz			.										*
 * Website:	https://github.com/truong-bui/AsyncLoadingScreen						*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "SLoadingScreenLayout.h"

struct FALoadingScreenSettings;
struct FCenterLayoutSettings;

/**
 * Menu layout screen
 */
class SMenuLayout : public SLoadingScreenLayout
{
public:
	SLATE_BEGIN_ARGS(SMenuLayout) {}

	SLATE_END_ARGS()

	/**
	 * Construct this widget
	 */
	void Construct(const FArguments& InArgs, const FALoadingScreenSettings& Settings, const FCenterLayoutSettings& LayoutSettings);

};
