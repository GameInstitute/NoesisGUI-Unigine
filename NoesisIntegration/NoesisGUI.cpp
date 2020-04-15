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

#include "NoesisGUI.h"

#include <NsApp\LocalXamlProvider.h>
#include <NsApp\LocalTextureProvider.h>
#include <NsApp\LocalFontProvider.h>

#include "D3D11RenderDevice.h"

#include <UnigineApp.h>
#include <UnigineInterpreter.h>
#include <UnigineRenderContext.h>
#include <UnigineTextures.h>
#include <UnigineCustomApp.h>


using namespace Unigine;
using namespace Noesis;
using namespace NoesisApp;

NoesisGUI::NoesisGUI()
{
	width_texture = 1920;
	height_texture = 1080;

	width_app = 1600;
	height_app = 900;
}

NoesisGUI::~NoesisGUI()
{
}

void NoesisLogHandler(const char* filename, uint32_t line, uint32_t level, const char* channel, const char* message)
{
	if (strcmp(channel, "") == 0)
	{
		// [TRACE] [DEBUG] [INFO] [WARNING] [ERROR]
		const char* prefixes[] = { "T", "D", "I", "W", "E" };
		const char* prefix = level < NS_COUNTOF(prefixes) ? prefixes[level] : " ";
		
		Unigine::Log::message("NOESIS %s %s", prefix, message);
	}
}

void NoesisGUI::Render()
{
	if (!view)
		return;

	//ppOldRtv = nullptr;
	//ppOldDsv = nullptr;
	//pContext->OMGetRenderTargets(1, &ppOldRtv, &ppOldDsv);

	view->GetRenderer()->UpdateRenderTree();
	view->GetRenderer()->RenderOffscreen();

	view->GetRenderer()->Render();

	ID3D11RenderTargetView* pTexNoesisRTV = static_cast<ID3D11RenderTargetView*>(my_texture->getD3D11RenderTargetView());
	pContext->OMSetRenderTargets(1, &pTexNoesisRTV, nullptr);

	//pContext->OMSetRenderTargets(1, &ppOldRtv, ppOldDsv);
}

void NoesisGUI::Update()
{
	if (!view)
		return;

	static uint64_t startTime = Noesis::HighResTimer::Ticks();

	uint64_t time = Noesis::HighResTimer::Ticks();
	view->Update(Noesis::HighResTimer::Seconds(time - startTime));

	// Scale mouse movement
	float diffX = (float)((float)width_texture / (float)width_app);
	float diffY = (float)((float)height_texture / (float)height_app);
	int mx = App::getMouseX();
	int my = App::getMouseY();
	float x = (float)mx * diffX;
	float y = (float)my * diffY;
	//int mouseX = (float)App::->getMouseX() * diff;
	int mouseX = (int)x;
	//int mouseY = App::->getMouseY();
	int mouseY = (int)y;

	view->MouseMove(mouseX, mouseY);

	if (!pressed && App::getMouseButtonState(App::BUTTON_LEFT))
	{
		view->MouseButtonDown(mouseX, mouseY, MouseButton_Left);
		pressed = true;
	}
	else if (!App::getMouseButtonState(App::BUTTON_LEFT))
	{
		if (pressed)
		{
			view->MouseButtonUp(mouseX, mouseY, MouseButton_Left);
			pressed = false;
		}
	}

	//if (App::->clearKeyState('j'))
	//{
	//	view->Char('j');
	//}

}

void NoesisGUI::Initialize()
{
	Noesis::GUI::Init(nullptr, NoesisLogHandler, nullptr);

	Noesis::GUI::SetXamlProvider(MakePtr<LocalXamlProvider>("."));
	Noesis::GUI::SetTextureProvider(MakePtr<LocalTextureProvider>("."));
	Noesis::GUI::SetFontProvider(MakePtr<LocalFontProvider>("."));

	pDevice = static_cast<ID3D11Device*>(Unigine::CustomApp::getD3D11Device());
	pContext = nullptr;
	pDevice->GetImmediateContext(&pContext);

	device = *new NoesisApp::D3D11RenderDevice(pContext, false);
	
	gui = Unigine::Gui::get();
	hud = WidgetSprite::create(gui);
	hud->setPosition(0, 0);
	hud->setWidth(width_texture);
	hud->setHeight(height_texture);
	hud->setLayerBlendFunc(0, Unigine::Gui::BLEND_ONE, Unigine::Gui::BLEND_ONE_MINUS_SRC_ALPHA);
	gui->setTransparentEnabled(1);

	gui->addChild(hud, Unigine::Gui::ALIGN_OVERLAP);

	my_texture = Unigine::Texture::create();

	Clear();
	hud->setRender(my_texture);
}

NoesisGUI &NoesisGUI::getInstance()
{
	static NoesisGUI INSTANCE;
	return INSTANCE;
}

NoesisGUI & NoesisGUI::getInstanceInner()
{
	return getInstance();
}

void NoesisGUI::Shutdown()
{
	my_texture.clear();
	hud.clear();
	gui.clear();
	
	xaml.Reset();
	if (view)
	{
		view->GetRenderer()->Shutdown();
		view.Reset();
	}
	
	device.Reset();
	Noesis::GUI::Shutdown();
}

// Load xaml file by US
void NoesisGUI::LoadUIbyUS(const Variable &name)
{
	if (name.getType() == Variable::STRING ) {
		ui_file_name = name.getString();
	}
	else
	{
		Unigine::Log::error("Error load gui file");
		return;
	}

	if (view)
	{
		view->GetRenderer()->Shutdown();
	}

	Clear();

	xaml = Noesis::GUI::LoadXaml<FrameworkElement>(ui_file_name.c_str());
	
	view = Noesis::GUI::CreateView(xaml);
	view->SetSize(width_texture, height_texture);

	view->GetRenderer()->Init(device);

	gui->setMouseGrab(1);
}

void NoesisGUI::ReloadGUI()
{
	Clear();
	xaml = Noesis::GUI::LoadXaml<FrameworkElement>(ui_file_name.c_str());
	view->GetRenderer()->Shutdown();
	view = Noesis::GUI::CreateView(xaml);
	view->SetSize(width_texture, height_texture);
	view->GetRenderer()->Init(device);
}

void NoesisGUI::GrabMouse(bool grab)
{
	gui->setMouseGrab(grab);
}

void NoesisGUI::LoadUI(std::string name)
{
	ui_file_name = name;

	xaml = Noesis::GUI::LoadXaml<FrameworkElement>(ui_file_name.c_str());
	
	if (view)
	{
		view->GetRenderer()->Shutdown();
	}

	Clear();

	view = Noesis::GUI::CreateView(xaml);
	view->SetIsPPAAEnabled(true);
	//view->SetTessellationQuality(TessellationQuality_High);
	view->SetTessellationMaxPixelError(Noesis::TessellationMaxPixelError::HighQuality());
	view->SetSize(width_texture, height_texture);
	view->GetRenderer()->Init(device);
}

void NoesisGUI::Clear()
{
	int flags = Unigine::Texture::TEXTURE_2D | Unigine::Texture::FORMAT_RGBA8 |
		Unigine::Texture::FORMAT_D16 | Unigine::Texture::FORMAT_D24S8 |
		Unigine::Texture::MULTISAMPLE_8 | Unigine::Texture::USAGE_RENDER;

	my_texture->create2D(width_texture, height_texture, Unigine::Texture::FORMAT_RGBA8, flags);
}
