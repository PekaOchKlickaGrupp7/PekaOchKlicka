/*
Use this class to create and show a text
*/

#pragma once
#include "tga2d/math/Vector2.h"
#include "tga2d/math/Color.h"
#include <string>

namespace DX2D
{
	class CTextService;
	class CText
	{
	public:
		CText(const char* aPathAndName);
		~CText();
		void Render();
		float GetWidth() const;
		static float GetWidth(const std::string &aString, const float &aSize, const std::string &aPathAndName);

		float GetHeight() const;
		static float GetHeight(const std::string &aString, const float &aSize, const std::string &aPathAndName);

		inline const std::string& GetPathAndName();


		std::string myText;
		Vector2f myPosition;
		float mySize;
		CColor myColor;
	private:
		std::string myPathAndName;
		CTextService* myTextService;
	};
}

