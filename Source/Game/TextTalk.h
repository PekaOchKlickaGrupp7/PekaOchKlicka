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
	void Render(Synchronizer& aSynchronizer, DX2D::Vector2f aPos);

	//Stats printing aString with the size of aSize and with the speed aTextSpeed in ms
	void StartTalk(const std::string &aString, const float &aSize, const int unsigned &aTextSpeed);

	void SetState(const int aState);
	void SetText(const std::string &aString);
	void SetPosition(const DX2D::Vector2f &aPosition);
	DX2D::CText* GetCText() const;

	const DX2D::Vector2f GetPosition();
private:

	void NewSubString();

	//CommonUtilities::GrowingArray <const char*> myStrings;
	unsigned int mySpeed;
	std::string myString;
	DX2D::CText *myText;
	DX2D::CColor myColor;
	int myWords;
	bool myIsTalking;
	CU::TimeSys::TimerManager myTimerManager;
	unsigned char myTimerHandle;
};