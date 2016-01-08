#include "stdafx.h"
#include "Synchronizer.h"


Synchronizer::Synchronizer()
{
	myLogicIsDone = false;
	myRenderIsDone = false;
	myQuit = false;
	myCurrentBuffer = 0;
	myRenderCommandBuffers[0].Init(4);
	myRenderCommandBuffers[1].Init(4);
}


Synchronizer::~Synchronizer()
{
}