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


#include <UnigineEngine.h>
#include <UnigineInterpreter.h>
#include <UnigineInterface.h>

#include "AppEditorLogic.h"
#include "AppSystemLogic.h"
#include "AppWorldLogic.h"

#include "NoesisGUI.h"

#pragma comment(lib, "Noesis.lib")
#pragma comment(lib, "NoesisApp.lib")
#pragma comment(lib, "NoesisIntegration.lib")

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	ExternClass<NoesisGUI> *export_gui = MakeExternClass<NoesisGUI>();
	export_gui->addConstructor();
	export_gui->addFunction("LoadUIbyUS", &NoesisGUI::LoadUIbyUS);
	export_gui->addFunction("getInstance", &NoesisGUI::getInstanceInner);
	Interpreter::addExternClass("NoesisGUIObject", export_gui);

	Unigine::Engine* pEngine = Unigine::Engine::init(UNIGINE_VERSION, argc, argv);
	AppSystemLogic *system_logic = new AppSystemLogic();
	AppWorldLogic *world_logic = new AppWorldLogic();
	AppEditorLogic *editor_logic = new AppEditorLogic();

	pEngine->addSystemLogic(system_logic);
	pEngine->addWorldLogic(world_logic);
	pEngine->addEditorLogic(editor_logic);

	NoesisGUI* noesisGUI = &NoesisGUI::getInstance();
	noesisGUI->Initialize();

	noesisGUI->LoadUI("start.xaml");

	while (!pEngine->isDone())
	{
		pEngine->update();
		noesisGUI->Update();

		pEngine->render();
		noesisGUI->Render();

		//noesisGUI->Clear();
		pEngine->swap();
	}

	noesisGUI->Shutdown();

	//delete editor_logic;
	//delete world_logic;
	//delete system_logic;

	//editor_logic = NULL;
	//world_logic = NULL;
	//system_logic = NULL;

	pEngine->shutdown();

	return 0;
}

#ifdef _WIN32
#include <Windows.h>
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif
