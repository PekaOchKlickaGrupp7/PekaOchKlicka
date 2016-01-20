#pragma once
#include "../CommonUtilities/TimerManager.h"
class DX2D::CText;

class TextTalk
{
public:
	TextTalk();
	~TextTalk();

	void Init(const char* aPath, DX2D::CColor aColor);
	void Update(const DX2D::Vector2f &aPosition);
	void Render();

	void StartTalk(const std::string &aString, const float &aSize);

	void SetState(const int aState);
	void SetText(const std::string &aString);
	void SetPosition(const DX2D::Vector2f &aPosition);
	DX2D::CText* GetCText() const;

	const DX2D::Vector2f GetPosition();
private:

	void NewSubString();

	//CommonUtilities::GrowingArray <const char*> myStrings;
	std::string myString;
	DX2D::CText *myText;
	DX2D::CColor myColor;
	int myWords;
	bool myIsTalking;
	CU::TimeSys::TimerManager myTimerManager;
	unsigned char myTimerHandle;
};