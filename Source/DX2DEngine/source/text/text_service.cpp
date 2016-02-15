#include "stdafx.h"

#include "text/text_service.h"
#include "texture/texture_manager.h"
#include "text/textfile.h"
#include "text/fontfile.h"
#include "sprite/sprite_batch.h"
#include "sprite/sprite.h"
#include "sprite/textured_quad_batch.h"
#include "engine.h"

#include <iostream>
#include <fstream>

#define MAX_SPRITES 20000

using namespace DX2D;

CTextService::CTextService()
{
	minS = 0;
	minT = 0;
	maxS =0;
	maxT = 0;
	myCurrentSprite = 0;
	myIsLoaded = false;
	myTextsToDraw = 0;

}


CTextService::~CTextService()
{
	for (CSprite* sprite : mySprites)
	{
		delete sprite;
	}
	mySprites.clear();

	for (int i = 0; i < myTextsToRender.TotalCount(); i++)
	{
		delete myTextsToRender.myData[i];
	}
}

void CTextService::Init()
{

	for (int i = 0; i< MAX_SPRITES; i++)
	{
		DX2D::CSprite* sprite = new DX2D::CSprite();
		mySprites.push_back(sprite);
	}

	myTextsToRender.Init(128);

	for (int i = 0; i < 128; i++)
	{
		SText* text = new SText();
		myTextsToRender.myData[i] = text;
	}
}

inline bool FileExists(const std::string& name) 
{
	std::ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}

void CTextService::InitAndLoad(std::string aFontPathAndName)
{
	std::map<std::string, SFontData*>::iterator iter = myFontDatas.find(aFontPathAndName);
	if (iter != myFontDatas.end())
	{
		return;
	}

	std::string name = aFontPathAndName;

	std::string image = name;
	image.append(".dds");

	if (!FileExists(image))
	{
		ERROR_AUTO_PRINT("%s %s", "Text load error! File not found: ", image.c_str());
		return;
	}

	std::string fontFile = name;
	fontFile.append(".txt");

	if (!FileExists(fontFile))
	{
		ERROR_AUTO_PRINT("%s %s", "Text load error! File not found: ", fontFile.c_str());
		return;
	}

	SFontData* fontData = new SFontData();
	fontData->myBatch = new CSpriteBatch(true);
	fontData->myBatch->Init(image.c_str());
	fontData->myBatch->myQuadBatch->myIsScreenSpace = true;
	fontData->myBatch->SetEffect(SEffect_SignedDistanceField);
	fontData->myTexture = CEngine::GetInstance()->GetTextureManager().GetTexture(image.c_str());

	ParseFontFile(fontData->myFontChars, fontFile, static_cast<unsigned int>(fontData->myTexture->myImageSize.x), static_cast<unsigned int>(fontData->myTexture->myImageSize.y));

	myFontDatas[aFontPathAndName] = fontData;
	myIsLoaded = true;
}

void CTextService::Update()
{
	if (!myIsLoaded)
	{
		return;
	}
	for (std::map<std::string, SFontData*>::iterator iterator = myFontDatas.begin(); iterator != myFontDatas.end(); iterator++)
	{
		if (iterator->second)
		{
			iterator->second->myBatch->ClearAll();
		}
	}
	myCurrentSprite = 0;
}


float DX2D::CTextService::GetSentenceWidth(const std::string& aText, float aSize, const std::string& aName)
{
	if (aText.size() <= 0)
	{
		return 0.0f;
	}
	SFontData* fontData = myFontDatas[aName];
	if (!fontData || !myIsLoaded)
	{
		return 0;
	}
	float nextX = 0.0f;
	float maxX = 0.0f;
	for (unsigned int i = 0; i < aText.size(); i++)
	{
		int aChar = aText[i];
		if (aChar == '\n')
		{
			if (nextX > maxX)
			{
				maxX = nextX;
				nextX = 0.0f;
				continue;
			}
		}
		aChar = aChar < 0 ? 63 : aChar;
		const fontChar& theChar = fontData->myFontChars[aChar];
		nextX += (((theChar.xadvance_ - theChar.xoffset_) / 1920.0f) * aSize);
		if (i == aText.size() - 1)
		{
			if (nextX > maxX)
			{
				maxX = nextX;
				nextX = 0.0f;
				continue;
			}
		}
	}

	return maxX;

}

#define NEW_LINE_HEIGHT 0.086f

float DX2D::CTextService::GetSentenceHeight(const std::string& aText, float aSize, const std::string& aName)
{
	if (aText.size() <= 0)
	{
		return 0.0f;
	}
	SFontData* fontData = myFontDatas[aName];
	if (!fontData || !myIsLoaded)
	{
		return 0;
	}
	float nextY = aSize * NEW_LINE_HEIGHT;
	for (unsigned int i = 0; i < aText.size(); i++)
	{
		int aChar = aText[i];
		if (aChar == '\n')
		{
			nextY += aSize * NEW_LINE_HEIGHT;
			/*aChar = aChar < 0 ? 63 : aChar;
			const fontChar& theChar = fontData->myFontChars[aChar];
			nextY += ((theChar.y_ / 1920.0f) * aSize) * (DX2D::CEngine::GetInstance()->GetWindowRatio() * 0.91f);*/
		}
	}

	return nextY;

}


void CTextService::UpdateTextAndDraw(const std::string& aText, const Vector2f& aPosition, CColor aColor, float aSize, const std::string& aName)
{
	if (!myIsLoaded)
	{
		return;
	}
	
	SFontData* fontData = myFontDatas[aName];
	if (!fontData)
	{
		return;
	}

	BuildText(aText, aPosition, aColor, aSize, aName, fontData->myTextBuffer);
	fontData->myBatch->ClearAll();
	for (STextToRender& vertex : fontData->myTextBuffer)
	{
		if (myCurrentSprite < MAX_SPRITES)
		{
			DX2D::CSprite* sprite = mySprites[myCurrentSprite];
			sprite->SetPosition(vertex.myPosition);
			sprite->SetSize(vertex.mySize);
			sprite->SetColor(vertex.myColor);
			sprite->SetUVOffset(vertex.myUV);
			sprite->SetPivot({ 0, 0.5f });
			sprite->SetUVScale(vertex.uvScale);
			fontData->myBatch->AddObject(sprite);
			myCurrentSprite++;
		}
	}
	fontData->myTextBuffer.clear();


	fontData->myBatch->Render();
//	Render();

}

void CTextService::BuildText(const std::string& aText, const Vector2f& aPosition, CColor aColor, float aSize, const std::string& aName, std::vector<STextToRender>& aTextBuffer)
{
	SFontData* fontData = myFontDatas[aName];
	if (!fontData || !myIsLoaded)
	{
		return;
	}
	float nextX = 0.0f;
	float nextY = 0.0f;

	for (unsigned int i = 0; i< aText.size(); i++)
	{
		int aChar = aText[i];

		aChar = aChar < 0 ? 63 : aChar;
		const fontChar& theChar = fontData->myFontChars[aChar];
		minS = theChar.sMin_;
		maxT = theChar.tMin_;
		maxS = theChar.sMax_;
		minT = theChar.tMax_;

		bool isNewLine = aChar == 10;
		if (isNewLine)
		{
			nextY += aSize * NEW_LINE_HEIGHT;
			nextX = 0;
			continue;
		}

		float sizeX = (theChar.width_ / (float)fontData->myTexture->myImageSize.x);
		float sizeY = (theChar.height_ / (float)fontData->myTexture->myImageSize.y);

		Vector2f offset(theChar.xoffset_ / (float)fontData->myTexture->myImageSize.x, theChar.yoffset_ / (float)fontData->myTexture->myImageSize.y);
		offset /= 2.0f;

		Vector2f size;
		size.x = (sizeX * aSize);
		size.y = (sizeY)* aSize;
		size.y *= 1.777f;

		STextToRender renderText;

		float baseOffset = 0.0032f;
		float offsetTheDammOffset = 0;
		if (aChar == 'g')
		{
			offsetTheDammOffset = baseOffset;
		}
		else if (aChar == 'j')
		{
			offsetTheDammOffset = baseOffset;
		}
		else if (aChar == 'p')
		{
			offsetTheDammOffset = baseOffset;
		}
		else if (aChar == 'q')
		{
			offsetTheDammOffset = baseOffset;
		}

		renderText.myPosition.Set((aPosition.x + nextX), nextY + (aPosition.y - (offset.y * aSize)) + offsetTheDammOffset);
		renderText.mySize = size;
		renderText.myUV.Set(minS, maxT);
		renderText.uvScale.x = sizeX;
		renderText.uvScale.y = sizeY;
		renderText.myColor = aColor;

		nextX += (((((theChar.xadvance_ - theChar.xoffset_) / 1920.0f))) * aSize);// *1.777f;

		aTextBuffer.push_back(renderText);
	}
}


void CTextService::AddTextToRender(const std::string& aText, const Vector2f& aPosition, CColor aColor, float aSize, const std::string& aName)
{
	UpdateTextAndDraw(aText, aPosition, aColor, aSize, aName);
}
