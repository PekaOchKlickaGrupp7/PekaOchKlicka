#pragma once
#include "render_common.h"
#include <vector>
#include <map>

struct ID3D11ShaderResourceView;
namespace DX2D
{
	class CRenderObject;
	class CTexturedQuad;
	class CLight;
	class CRenderer
	{
	public:
		CRenderer(bool aActivateDebug);
		~CRenderer();
		bool DoRender();
		void AddToRender(CRenderObject* aObject);
		void AddToRender(CTexturedQuad* aObject);
		void Update();
	private:
		bool myActivateDebug;

		struct SOptimTip
		{
			SOptimTip()
			{
				myCount = 0;
				myHasWarned = false;
			}
			unsigned int myCount;
			bool myHasWarned;
			std::string myPath;
		};
		
		typedef std::map<ID3D11ShaderResourceView*, SOptimTip> OptimisationMap;
		OptimisationMap myRenderedSpritesOfSameType;
	};
}