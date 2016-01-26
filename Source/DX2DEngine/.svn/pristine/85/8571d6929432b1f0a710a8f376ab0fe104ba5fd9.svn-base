#pragma once
#include "render_common.h"
#include <vector>

namespace DX2D
{
	class CRenderObject;
	class CLight;
	class CRenderer
	{
	public:
		CRenderer(int aMaxRenderObjectPerFrame);
		~CRenderer();
		bool DoRender();
		void AddToRender(CRenderObject* aObject);
	private:
		CRenderObject** myToRenderObjects;
		int myCurrentCount;
		int myMaxRenderObjectPerFrame;
	};
}