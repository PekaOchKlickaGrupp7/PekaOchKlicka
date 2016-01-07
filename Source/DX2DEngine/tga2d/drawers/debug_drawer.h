#pragma once

#include "tga2d/math/Vector2.h"
#include "tga2d/math/Vector4.h"
#include <vector>


#define MAXSAMPLES 100
namespace DX2D
{
	class CText;
	class LinePrimitive;
	class CSprite;
	class CDebugDrawer
	{
	public:
		CDebugDrawer(bool aActivateDebugging);
		~CDebugDrawer(void);
		void Init();
		void Update(float aTimeDelta);
		void Render();
		void DrawLine(Vector2f aFrom, Vector2f aTo, Vector4f aColor = Vector4f(1, 1, 1, 1));
		void DrawArrow(Vector2f aFrom, Vector2f aTo, Vector4f aColor = Vector4f(1, 1, 1, 1));
		void ShowErrorImage();
	private:
		double CalcAverageTick(int newtick);
		std::vector<LinePrimitive*> myLineBuffer;
		int myNumberOfRenderedLines;
		int myMaxLines;
		CText* myFPS; 
		CText* myMemUsage;
		CText* myDrawCallText;
		CText* myCPUText;

		CSprite* myErrorSprite;
		
		int tickindex;
		int ticksum;
		int ticklist[MAXSAMPLES];
		bool myHasDebuggingActive;
		float myShowErrorTimer;
	};
}
