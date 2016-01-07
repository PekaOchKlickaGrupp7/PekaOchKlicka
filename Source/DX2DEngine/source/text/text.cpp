#include "stdafx.h"

#include "text/text.h"
#include "engine.h"
#include "text/text_service.h"

using namespace DX2D;

CText::CText(const char* aPathAndName)
{
	myColor.Set(1, 1, 1, 1);
	mySize = 0.5f;
	myPathAndName = aPathAndName;
	DX2D::CEngine::GetInstance()->GetTextService().InitAndLoad(myPathAndName);
}


CText::~CText()
{
}

void DX2D::CText::Render()
{
	DX2D::CEngine::GetInstance()->GetTextService().AddTextToRender(myText, myPosition, myColor, mySize, myPathAndName); 
}
