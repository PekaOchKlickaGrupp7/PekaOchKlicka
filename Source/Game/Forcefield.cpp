#include "stdafx.h"
#include "Forcefield.h"
#include "Particle.h"

Forcefield::Forcefield()
{
}

Forcefield::~Forcefield()
{
}

void Forcefield::AffectParticle(Particle* aParticle)
{
	Vector2f velocity = aParticle->GetVelocity();
	Vector2f position = aParticle->GetPosition();
	Vector2f diff = myPosition - position;
	float mag = diff.Length();
	if (mag <= mySize)
	{
		float l = mySize - mag;
		l = fminf(mySize / 3.0f, l);
		velocity += (diff.GetNormalized() * l) * myStrength;
		aParticle->SetVelocity(velocity);
	}
}

void Forcefield::SetStrength(const float aStrength)
{
	myStrength = aStrength;
}

void Forcefield::SetSize(const float aSize)
{
	mySize = aSize;
}

void Forcefield::SetPosition(const Vector2f& aPosition)
{
	myPosition = aPosition;
}
