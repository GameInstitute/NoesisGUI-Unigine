/* Copyright (C) 2005-2020, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2.11 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */


#include "AppSystemLogic.h"
#include "UnigineApp.h"

using namespace Unigine;

// System logic, it exists during the application life cycle.
// These methods are called right after corresponding system script's (UnigineScript) methods.

AppSystemLogic::AppSystemLogic()
{

}

AppSystemLogic::~AppSystemLogic()
{
}

int AppSystemLogic::init()
{
	noesisGUI = &NoesisGUI::getInstance();
	//NoesisGUI* noesisGUI = new NoesisGUI();
	noesisGUI->Initialize();
	noesisGUI->LoadUI("start.xaml");

	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// start of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppSystemLogic::update()
{
	noesisGUI->Update();
	return 1;
}

//int AppSystemLogic::render() {
//	// Write here code to be called before rendering each render frame.
//	noesisGUI->Render();
//
//	return 1;
//}

int AppSystemLogic::postUpdate()
{
	// Write here code to be called after updating each render frame.
	noesisGUI->Render();
	return 1;
}

////////////////////////////////////////////////////////////////////////////////
// end of the main loop
////////////////////////////////////////////////////////////////////////////////

int AppSystemLogic::shutdown()
{
	noesisGUI->Shutdown();
	return 1;
}
