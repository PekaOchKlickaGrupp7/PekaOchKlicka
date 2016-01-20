#pragma once
#include "RenderCommand.h"
#include "..\CommonUtilities\GrowingArray.h"
#include "..\CommonUtilities\StaticArray.h"
#include "..\CommonUtilities\Matrix33.h"

class Synchronizer
{
public:
	Synchronizer();
	~Synchronizer();

	void WaitForRender();
	void WaitForLogic();
	volatile bool HasQuit() const;
	void RenderIsDone();
	void LogicIsDone();
	void SwapBuffer();
	void Quit();
	void AddRenderCommand(const RenderCommand& aRenderCommand);
	int GetSize() const;
	RenderCommand& GetRenderCommand(unsigned short anIndex);
	void AddCameraMatrix(const Matrix33<float>& aCameraMatrix);
	const Matrix33<float>& GetCameraMatrix() const;

private:
	volatile bool myRenderIsDone;
	volatile bool myLogicIsDone;
	volatile bool myQuit;
	CommonUtilities::StaticArray<CommonUtilities::GrowingArray<RenderCommand,int>, 2> myRenderCommandBuffers;
	CommonUtilities::StaticArray<Matrix33<float>, 2> myCameraMatrixBuffers;
	int myCurrentBuffer;
};

inline void Synchronizer::Quit()
{
	myLogicIsDone = true;
	myRenderIsDone = true;
	myQuit = true;

}

inline void Synchronizer::WaitForRender()
{
	while (myRenderIsDone == false);
	myRenderIsDone = false;
}

inline void Synchronizer::WaitForLogic()
{
	while (myLogicIsDone == false);
	myLogicIsDone = false;
}

inline volatile bool Synchronizer::HasQuit() const
{
	return myQuit;
}

inline void Synchronizer::RenderIsDone()
{
	myRenderIsDone = true;
}

inline void Synchronizer::LogicIsDone()
{
	myLogicIsDone = true;
}

inline void Synchronizer::SwapBuffer()
{
	myRenderCommandBuffers[myCurrentBuffer].RemoveAll();
	myCurrentBuffer ^= 1;
}

inline void Synchronizer::AddRenderCommand(const RenderCommand& aRenderCommand)
{
	myRenderCommandBuffers[myCurrentBuffer ^ 1].Add(aRenderCommand);
}

inline int Synchronizer::GetSize() const
{
	return myRenderCommandBuffers[myCurrentBuffer].Size();
}

inline RenderCommand& Synchronizer::GetRenderCommand(unsigned short anIndex)
{
	return myRenderCommandBuffers[myCurrentBuffer][anIndex];
}

inline void Synchronizer::AddCameraMatrix(const Matrix33<float>& aCameraMatrix)
{
	myCameraMatrixBuffers[myCurrentBuffer ^ 1] = aCameraMatrix;
}

inline const Matrix33<float>& Synchronizer::GetCameraMatrix() const
{
	return myCameraMatrixBuffers[myCurrentBuffer];
}