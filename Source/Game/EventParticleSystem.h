#pragma once
#include "Event.h"
#include "Particle.h"
#include "Forcefield.h"

class EventParticleSystem :
	public Event
{
public:
	EventParticleSystem();
	virtual ~EventParticleSystem();

	virtual void Init(Room* aRoom, CGameWorld* aGameWorld) override;
	virtual bool Update(const float aDeltaTime) override;
	virtual void PostRender(Synchronizer &aSynchronizer) override;
	virtual void Reset() override;

	void DoRender(Synchronizer& aSynchronizer);

	std::string myFile;
protected:
	const char* ReadFile(const char* aFile);
	float RandomInRange(float aMin, float aMax);
	void EmitParticle();
	void InitParticle(Particle* aParticle);

	DX2D::CSprite* mySprite;

	Vector2f myEmissionAreaMin;
	Vector2f myEmissionAreaMax;

	Vector2f myMinEmissionVelocity;
	Vector2f myMaxEmissionVelocity;

	float myMinLifeTime;
	float myMaxLifeTime;
	float mySpawnTime;
	float mySpawnRate;
	float myEmissionLifeTime;
	float myEmissionLifeTimeLeft;

	int myPoolOffset;
	bool myEnabled;

	DX2D::CColor myParticleColor;

	CommonUtilities::GrowingArray<Particle, int> myParticlesPool;
	CommonUtilities::GrowingArray<Particle*, int> myParticlesActive;
	CommonUtilities::GrowingArray<Forcefield, int> myForcefields;
};

