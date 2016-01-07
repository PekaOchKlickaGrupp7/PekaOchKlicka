#include "stdafx.h"
#include "light/light_manager.h"
#include "engine.h"
#include "render/Renderer.h"

using namespace DX2D;

CLightManager::CLightManager()
{
	myAmbientLight = 1.0f;
}

CLightManager::~CLightManager()
{
}

void CLightManager::AddLightToRender(const CLight* aLight)
{
	myLightsToRender.push_back(aLight);
}

void DX2D::CLightManager::PostFrameUpdate()
{
	myLightsToRender.clear();
}

void DX2D::CLightManager::SetAmbience(float aAmbientLight)
{
	myAmbientLight = aAmbientLight;
}