/************************************************************************************
 *																					*
 * Copyright (C) 2020 Thomas Strunz													*
 * Website:	https://github.com/salchint/AsyncLoadingScreen							*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#pragma once

#include "LoadingScreenSettings.h"
#include "Templates/SharedPointer.h"

class IWidgetFactory
{
public:
	virtual TSharedRef<SWidget> BuildMenuLayout(const FALoadingScreenSettings& LoadingScreenSettings, const ULoadingScreenSettings& Settings) =0;
	virtual TSharedRef<SWidget> BuildMenu(const FALoadingScreenSettings& Settings) =0;
};


class ASYNCLOADINGSCREEN_API AbstractWidgetFactory
{
	static IWidgetFactory* TheFactory;

public:
	static void RegisterFactory(IWidgetFactory* obj);
	static IWidgetFactory* GetFactory();
};

