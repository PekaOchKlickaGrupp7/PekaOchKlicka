#pragma once
#include "..\CommonUtilities\Vector.h"
#include <string>

class Synchronizer;

class MenuItem
{
public:
	enum class eAction
	{
		NONE,
		PLAY,
		LEVEL_SELECT,
		CREDIT,
		EXIT,
		RESUME,
		RESTART_LEVEL,
	};

	MenuItem(eAction anAction, Vector2<float> aPos, Vector2<float> aSize);
	virtual ~MenuItem();
	virtual void Render(Synchronizer& aSynchronizer, unsigned long aARGB = 0xFFFFFFFF) = 0;
	virtual void SetHighlight(bool aHighlight);
	virtual bool Collide(const Vector2<float>& aCursorPos) const = 0;
	virtual int GetId() const;
	eAction GetAction() const;
protected:
	eAction myAction;
	Vector2<float> myPos;
	Vector2<float> mySize;
	bool myHighlight;
	std::string mySound;
};

inline void MenuItem::SetHighlight(bool aHighlight)
{
	myHighlight = aHighlight;
}

inline MenuItem::eAction MenuItem::GetAction() const
{
	return myAction;
}

inline int MenuItem::GetId() const
{
	return 0;
}