#include "stdafx.h"
#include "Particle.h"
#include "Forcefield.h"

Particle::Particle()
{
	myIsActive = false;
}

Particle::~Particle()
{
}

void Particle::Init(rapidjson::Value& aValue)
{
	myStartAlpha = aValue["StartAlpha"].GetFloat();
	myStartSize = aValue["StartSize"].GetFloat();
	myDeltaAlpha = aValue["DeltaAlpha"].GetFloat();
	myDeltaSize = aValue["SizeDelta"].GetFloat();
	myGravity = Vector2f(aValue["Gravitation"]["x"].GetFloat(), aValue["Gravitation"]["y"].GetFloat());
	myIsAffectedGravity = aValue["IsAffectedGravity"].GetBool();
	myColor = { 1, 1, 1, myStartAlpha };
}

bool Particle::Update(float aDeltaTime)
{
	if (myIsAffectedGravity == true)
	{
		myVelocity += myGravity * aDeltaTime;
	}

	myPosition += myVelocity * aDeltaTime;
	myLifetimeLeft -= aDeltaTime;
	myCurrentAlpha -= (myStartAlpha - myDeltaAlpha) * (aDeltaTime / myTotalLifeTime);
	myCurrentSize -= (myStartSize - myDeltaSize) * (aDeltaTime / myTotalLifeTime);

	myColor.myA = myCurrentAlpha;
	if (myLifetimeLeft <= 0.0f)
	{
		return true;
	}
	return false;
}

void Particle::SetIsActive(bool aActive)
{
	myIsActive = aActive;
	myLifetimeLeft = myTotalLifeTime;
	myCurrentAlpha = myStartAlpha;
	myCurrentSize = myStartSize;
}

bool Particle::GetIsActive()
{
	return myIsActive;
}

float Particle::GetCurrentSize() const
{
	return myCurrentSize;
}

void Particle::SetTotalLifeTime(const float aLifetime)
{
	myTotalLifeTime = aLifetime;
}

void Particle::SetVelocity(const Vector2f& aVelocity)
{
	myVelocity = aVelocity;
}

Vector2f Particle::GetVelocity()
{
	return myVelocity;
}

void Particle::SetPosition(const Vector2f& aPosition)
{
	myPosition = aPosition;
}

void Particle::SetColor(const DX2D::CColor& aColor)
{
	myColor = aColor;
}

void Particle::SetNormal(const Vector2f& aNormal)
{
	myNormal = aNormal;
}

float Particle::GetTotalLifeTime() const
{
	return myTotalLifeTime;
}

DX2D::CColor Particle::GetColor()
{
	return myColor;
}
Vector2f Particle::GetPosition()
{
	return myPosition;
}
const Vector2f Particle::GetNormal() const
{
	return myNormal;
}
