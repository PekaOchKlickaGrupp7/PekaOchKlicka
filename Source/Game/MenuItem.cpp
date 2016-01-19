#include "stdafx.h"
#include "MenuItem.h"
#include "Synchronizer.h"
#include "..\CommonUtilities\Intersection.h"


MenuItem::MenuItem(eAction anAction, Vector2<float> aPos
	, Vector2<float> aSize)
	: myAction(anAction)
	, myPos(aPos)
	, mySize(aSize)
	, myHighlight(false)
{

}

MenuItem::~MenuItem()
{
}



