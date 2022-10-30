#pragma once

#include <DDECS.h>
#include "Pathfinding.h"

extern DDECS<24, 100008> ecs;
#ifdef USE_AABB
#include <PhysicsEngineAABB2D.h>
extern PhysicsEngineAABB2D<512 / 2, 512 / 2, 128> physics;
#elif defined(USE_CONVEX)
#include <PhysicsEngineConvex2D.h>
extern PhysicsEngineConvex2D<512 / 2, 512 / 2, 128> physics;
#else
#error "didn't define either USE_AABB nor USE_CONVEX"
#endif

#include "SystemDeath.h"
#include "SystemDisplay.h"
#include "SystemPhysics.h"
#include "SystemGetTexture.h"
#include "SystemDamage.h"
#include "SystemController.h"
#include "SystemQueSpawn.h"
#include "SystemMoveTo.h"
#include "SystemUnitAI.h"
#include "SystemDestructOnOverlap.h"

void initSystems() {
	ecs.registerSystem<SystemDeath>();
	ecs.registerSystem<SystemDisplay>();
	ecs.registerSystem<SystemPhysics>();
	ecs.registerSystem<SystemGetTexture>();
	ecs.registerSystem<SystemDamage>();
	ecs.registerSystem<SystemController>();
	ecs.registerSystem<SystemQueSpawn>();
	ecs.registerSystem<SystemMoveTo>();
	ecs.registerSystem<SystemUnitAI>();
	ecs.registerSystem<SystemDestructOnOverlap>();
}