#pragma once
#include <tga2d\math\vector2.h>
#include "..\CommonUtilities\Vector2.h"

class Particle;

class Forcefield
{
public:
	Forcefield();
	virtual ~Forcefield();

	virtual void AffectParticle(Particle* aParticle);

	void SetStrength(const float aStrength);
	void SetSize(const float aSize);
	void SetPosition(const Vector2f& aPosition);

	inline float GetStrength() const;
	inline float GetSize() const;
	inline Vector2f& GetPosition();
protected:
	float myStrength;
	float mySize;
	Vector2f myPosition;
};

float Forcefield::GetStrength() const
{
	return myStrength;
}

float Forcefield::GetSize() const
{
	return mySize;
}

Vector2f& Forcefield::GetPosition()
{
	return myPosition;
}
