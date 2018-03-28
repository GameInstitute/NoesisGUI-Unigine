/*
  NoesisGUI - Unigine Integration
  Copyright (c) 2014-2018 R&D Lab. All right reserved.
  office@researchdevelopinglab.com
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
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
#include <windows.h>
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#endif