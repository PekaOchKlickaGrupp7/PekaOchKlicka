#pragma once
#include "tga2d\math\color.h"
#include "..\CommonUtilities\Vector2.h"
#include "rapidjson\document.h"

class Forcefield;
//namespace rapidjson
//{
//	class Value;
//}

class Particle
{
public:
	Particle();
	~Particle();

	void Init(rapidjson::Value& aValue);
	bool Update(float aDeltaTime);
	void AppendForcefield(Forcefield& aForcefield);

	void SetIsActive(bool aActive);
	bool GetIsActive();

	void SetTotalLifeTime(const float aLifetime);
	void SetPosition(const Vector2f& aPosition);
	void SetColor(const DX2D::CColor& aColor);
	void SetNormal(const Vector2f& aNormal);
	void SetVelocity(const Vector2f& aVelocity);

	float GetCurrentSize() const;
	float GetTotalLifeTime() const;
	DX2D::CColor GetColor();
	Vector2f GetPosition();
	const Vector2f GetNormal() const;
	Vector2f GetVelocity();
private:
	float myTotalLifeTime;
	float myCurrentAlpha;
	float myCurrentSize;
	float myLifetimeLeft;

	float myDeltaAlpha;
	float myDeltaSize;
	float myStartAlpha;
	float myStartSize;

	bool myIsActive;
	bool myIsAffectedGravity;
	Vector2f myPosition;
	Vector2f myVelocity;
	Vector2f myGravity;
	Vector2f myNormal;
	DX2D::CColor myColor;
};

