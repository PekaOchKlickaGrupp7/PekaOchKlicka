#include "stdafx.h"

#include "text/text.h"
#include "engine.h"
#include "text/text_service.h"

using namespace DX2D;

CText::CText(const char* aPathAndName)
: myTextService(&DX2D::CEngine::GetInstance()->GetTextService())
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
	if (!myTextService)
	{
		return;
	}
	myTextService->AddTextToRender(myText, myPosition, myColor, mySize, myPathAndName);
}

float DX2D::CText::GetWidth() const
{
	if (!myTextService)
	{
		return 0.0f;
	}
	return myTextService->GetSentenceWidth(myText, mySize, myPathAndName);
}

float DX2D::CText::GetWidth(const std::string &aString, const float &aSize, const std::string &aPathAndName)
{
	DX2D::CTextService* textService = &DX2D::CEngine::GetInstance()->GetTextService();
	if (!textService)
	{
		return 0.0f;
	}
	return textService->GetSentenceWidth(aString, aSize, aPathAndName);
}

const std::string& DX2D::CText::GetPathAndName()
{
	return myPathAndName;
}