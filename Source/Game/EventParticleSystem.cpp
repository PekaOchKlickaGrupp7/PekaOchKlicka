#include "stdafx.h"
#include "EventParticleSystem.h"
#include "rapidjson\document.h"
#include <fstream>
#include "GameWorld.h"

using namespace rapidjson;

EventParticleSystem::EventParticleSystem()
{
	myFile = "JSON/Smoke.json";
	myTarget = "Player";
	myEnabled = true;
}

EventParticleSystem::~EventParticleSystem()
{
	delete mySprite;
}

const char* EventParticleSystem::ReadFile(const char* aFile)
{
	std::ifstream input(aFile);

	std::string str((std::istreambuf_iterator<char>(input)),
		std::istreambuf_iterator<char>());

	char* data = new char[str.length() + 1];
	int a = 0;
	for (unsigned int i = 0; i < str.length(); ++i)
	{
		if (str[i] != 0)
		{
			data[a] = str[i];
			a++;
		}
	}

	data[a] = '\0';
	input.close();

	return data;
}

float EventParticleSystem::RandomInRange(float aMin, float aMax)
{
	return aMin + static_cast<float>(rand() % static_cast<int>((aMax - aMin) + 1));
}

void EventParticleSystem::InitParticle(Particle* aParticle)
{
	aParticle->SetTotalLifeTime(RandomInRange(myMinLifeTime * 100.0f, myMaxLifeTime * 100.0f) / 100.0f);

	ObjectData* object = GetGameObject(myTarget);
	Vector2f pos(object->myX, object->myY);

	pos += Vector2f(RandomInRange(myEmissionAreaMin.x * 100.0f, myEmissionAreaMax.x * 100.0f) / 100.0f,
		RandomInRange(myEmissionAreaMin.y * 100.0f, myEmissionAreaMax.y * 100.0f) / 100.0f);

	aParticle->SetPosition(pos);
	Vector2f velocity = Vector2f(
		RandomInRange(myMinEmissionVelocity.x * 100.0f, myMaxEmissionVelocity.x * 100.0f) / 100.0f,
		RandomInRange(myMinEmissionVelocity.y * 100.0f, myMaxEmissionVelocity.y * 100.0f) / 100.0f);

	aParticle->SetVelocity(velocity);
	aParticle->SetIsActive(true);
	myParticlesActive.Add(aParticle);
}

void EventParticleSystem::EmitParticle()
{
	for (int i = myPoolOffset; i < myParticlesPool.Size(); ++i)
	{
		if (myParticlesPool[i].GetIsActive() == false)
		{
			InitParticle(&myParticlesPool[i]);
			myPoolOffset = i + 1;
			return;
		}
	}
	for (int i = 0; i < myParticlesPool.Size(); ++i)
	{
		if (myParticlesPool[i].GetIsActive() == false)
		{
			InitParticle(&myParticlesPool[i]);
			myPoolOffset = i + 1;
			return;
		}
	}
}

void EventParticleSystem::Init(Room* aRoom, CGameWorld* aGameWorld)
{
	Event::Init(aRoom, aGameWorld);

	//Load particle file
	Document doc;
	doc.Parse(ReadFile(myFile.c_str()));

	Value& emitter = doc["Emitter"];

	mySpawnTime = 0.0f;
	mySpawnRate = 1.0f / emitter["EmissionRate"].GetFloat();

	mySprite = new DX2D::CSprite(emitter["ParticleImage"].GetString());

	myEmissionAreaMin = Vector2f(emitter["EmissionArea"]["min"]["x"].GetFloat(), emitter["EmissionArea"]["min"]["y"].GetFloat());
	myEmissionAreaMax = Vector2f(emitter["EmissionArea"]["max"]["x"].GetFloat(), emitter["EmissionArea"]["max"]["y"].GetFloat());

	myMinLifeTime = emitter["ParticleLifetime"]["min"].GetFloat();
	myMaxLifeTime = emitter["ParticleLifetime"]["max"].GetFloat();

	Value& velocity = emitter["EmissionVelocity"]["min"];
	myMinEmissionVelocity = Vector2f(velocity["x"].GetFloat(), velocity["y"].GetFloat());

	velocity = emitter["EmissionVelocity"]["max"];
	myMaxEmissionVelocity = Vector2f(velocity["x"].GetFloat(), velocity["y"].GetFloat());

	int maxSize = static_cast<int>(myMaxLifeTime / mySpawnRate) + 5;
	myPoolOffset = 0;

	myParticlesPool.Init(maxSize);
	myParticlesActive.Init(maxSize);

	myParticlesPool.Resize(maxSize);

	myEmissionLifeTime = emitter["EmissionLifeTime"].GetFloat();
	myEmissionLifeTimeLeft = myEmissionLifeTime;

	myForcefields.Init(2);
	for (unsigned int i = 0; i < doc["Forcefields"].Size(); ++i)
	{
		Forcefield forcefield;
		Value& pos = doc["Forcefields"][i]["ForcefieldPos"];
		forcefield.SetPosition(Vector2f(pos["x"].GetFloat(), pos["y"].GetFloat()));
		forcefield.SetStrength(doc["Forcefields"][i]["ForcefieldStrength"].GetFloat());
		forcefield.SetSize(doc["Forcefields"][i]["ForcefieldSize"].GetFloat());
		myForcefields.Add(forcefield);
	}

	for (int i = 0; i < myParticlesPool.Size(); ++i)
	{
		myParticlesPool[i].Init(doc["Particle"]);
	}
}

bool EventParticleSystem::Update(const float aDeltaTime)
{
	ObjectData* target = GetGameObject(myTarget);
	if (target != nullptr)
	{
		if (target->myActive == false)
		{
			return false;
		}
	}
	if (myEnabled == false || myObjectData->myActive == false)
	{
		return false;
	}

	myEmissionLifeTimeLeft -= aDeltaTime;
	if (myEmissionLifeTimeLeft >= 0 || myEmissionLifeTime == 0)
	{
		mySpawnTime += aDeltaTime;
		for (; mySpawnTime >= mySpawnRate; mySpawnTime -= mySpawnRate)
		{
			EmitParticle();
		}
	}
	else
	{
		if (myParticlesActive.Size() == 0 && myEmissionLifeTime > 0)
		{
			myEnabled = false;
		}
	}

	for (int i = 0; i < myParticlesActive.Size(); ++i)
	{
		for (int j = 0; j < myForcefields.Size(); ++j)
		{
			myForcefields[j].AffectParticle(myParticlesActive[i]);
		}
		if (myParticlesActive[i]->Update(aDeltaTime) == true)
		{
			myParticlesActive[i]->SetIsActive(false);
			myParticlesActive.RemoveCyclicAtIndex(i);
		}
	}

	return false;
}

void EventParticleSystem::PostRender(Synchronizer&)
{
	ObjectData* target = GetGameObject(myTarget);
	if (target != nullptr)
	{
		if (target->myActive == false)
		{
			return;
		}
	}
	if (myObjectData->myActive == false)
	{
		return;
	}
	myGameWorld->AddParticlePass(myTarget, this);
}

void EventParticleSystem::DoRender(Synchronizer& aSynchronizer)
{
	RenderCommand command;
	command.mySprite = mySprite;
	command.myType = eRenderType::eSprite;
	for (int i = 0; i < myParticlesActive.Size(); ++i)
	{
		DX2D::CColor color = myParticlesActive[i]->GetColor();
		float size = myParticlesActive[i]->GetCurrentSize();
		Vector2f position = myParticlesActive[i]->GetPosition();
		
		command.myHasColor = true;
		command.myColor = color;

		command.myHasSize = true;
		command.mySize = { size, size };

		command.myPosition = DX2D::Vector2f(position.x, position.y);
		aSynchronizer.AddRenderCommand(command);
	}
}

void EventParticleSystem::Reset()
{

}
