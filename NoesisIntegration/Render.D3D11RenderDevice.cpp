////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <NsCore/Package.h>
#include <NsRender/D3D11Factory.h>
#include <NsRender/Texture.h>

#include "D3D11RenderDevice.h"


using namespace Noesis;
using namespace NoesisApp;


Noesis::Ptr<Noesis::RenderDevice> CreateDevice(ID3D11DeviceContext* context, bool sRGB)
{
	return *new D3D11RenderDevice(context, sRGB);
}


Noesis::Ptr<Noesis::Texture> D3D11Factory::WrapTexture(ID3D11Texture2D* texture, uint32_t width,
	uint32_t height, uint32_t levels, bool isInverted)
{
	TextureFormat::Enum format= TextureFormat::RGBA8;
	return D3D11RenderDevice::WrapTexture(texture, width,height, levels, format, isInverted);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_REGISTER_REFLECTION(Render, D3D11RenderDevice)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INIT_PACKAGE(Render, D3D11RenderDevice)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_SHUTDOWN_PACKAGE(Render, D3D11RenderDevice)
{
}
