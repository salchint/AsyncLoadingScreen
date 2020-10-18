/************************************************************************************
 *																					*
 * Copyright (C) 2020 Thomas Strunz													*
 * Website:	https://github.com/salchint/AsyncLoadingScreen							*
 * Licensed under the MIT License. See 'LICENSE' file for full license information. *
 *																					*
 ************************************************************************************/

#include "IWidgetFactory.h"

IWidgetFactory* AbstractWidgetFactory::TheFactory = nullptr;

void AbstractWidgetFactory::RegisterFactory(IWidgetFactory* obj)
{
	TheFactory = obj;
}

IWidgetFactory* AbstractWidgetFactory::GetFactory()
{
	return TheFactory;
}


