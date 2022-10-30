#pragma once

#include <IO_API/IO_API.h>
#define USE_AABB
#include "ExampleSystems/2DExample/Systems.h"
#include <profiler.h>
#include <Asset.h>
#include <iostream>
#include <FixedPoint.h>
#include <EntityObjectLoader.h>

DDECS<24, 100008> ecs;
PhysicsEngineAABB2D<512/2, 512/2, 128> physics;

constexpr uint32_t sizeOfECS = sizeof(ecs);
constexpr uint32_t sizeOfPhysics = sizeof(physics);



bool componentExistsErrorCheck(EntityObject& eo, const std::string& componentName, const std::string& errorLocation,
		ComponentObject::TYPE type, uint32_t arraySize = 0) {
	ComponentObject* c = eo.getComponent(componentName);
	if (c == nullptr) {
		std::string errorMsg = "Error: ";
		errorMsg += errorLocation;
		errorMsg += " didn't have a ";
		errorMsg += componentName;
		errorMsg += " component.";
		std::cout << errorMsg << std::endl;
		return false;
	}
	if (c->type != type) {
		std::string errorMsg = "Error: ";
		errorMsg += errorLocation;
		errorMsg += " ";
		errorMsg += componentName;
		errorMsg += " component isn't of type ";
		errorMsg += c->typeAsString();
		std::cout << errorMsg << std::endl;
		return false;
	}
	if (c->type == ComponentObject::TYPE::TYPE_ARRAY) {
		if (c->getArrayIntLen() != arraySize) {
			std::string errorMsg = "Error: ";
			errorMsg += errorLocation;
			errorMsg += " ";
			errorMsg += componentName;
			errorMsg += " oes not have correct number of elements.";
			std::cout << errorMsg << std::endl;
			return false;
		}
	}
	return true;
}



void stressTest() {
	ComponentID bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
	ComponentID textureComponentID = ecs.registerComponent("texture", sizeof(TextureID));
	ComponentID healthComponentID = ecs.registerComponent("health", sizeof(SystemUtilities::Health));
	ComponentID damageOnCollisionComponentID = ecs.registerComponent("damageOnCollision", sizeof(SystemUtilities::DamageOnCollision));
	ComponentID controllerComponentID = ecs.registerComponent("controller", NULL);
	std::string koishiFilepath = getDirData();
	koishiFilepath += "/Textures/Koishi.png";
	for (uint32_t i = 0; i < 40000; i++) {
		EntityID entity = ecs.getNewEntity();
		BodyID bodyID = physics.addBodyRect(i * 96, (uint32_t)96, (uint32_t)64, (uint32_t)64);
		physics.addVelocity(bodyID, 2 * i % 3, 2 * i % 5);
		physics.setUserData(bodyID, (void*)entity);
		ecs.emplace(entity, bodyComponentID, &bodyID);
		TextureID koiTexture = TextureCodex::add(koishiFilepath);
		ecs.emplace(entity, textureComponentID, &koiTexture);
		SystemUtilities::Health health = { 1, 500.0f, {0.0f, .5f, .5f, .5f} };
		ecs.emplace(entity, healthComponentID, &health);
	}
	{
		EntityID entity = ecs.getNewEntity();
		BodyID bodyID = physics.addBodyRect((uint32_t)96 * 3, (uint32_t)96 * 5, (uint32_t)64, (uint32_t)64);
		physics.setUserData(bodyID, (void*)entity);
		ecs.emplace(entity, bodyComponentID, &bodyID);
		TextureID koiTexture = TextureCodex::add(koishiFilepath);
		ecs.emplace(entity, textureComponentID, &koiTexture);
		ComponentID cameraComponentID = ecs.registerComponent("camera", NULL);
		ecs.emplace(entity, cameraComponentID, NULL);
		SystemUtilities::DamageOnCollision dmg = { 0, 50.0f, {0.0f, 0.0f, 0.0f, 0.1f} };
		ecs.emplace(entity, damageOnCollisionComponentID, &dmg);
		ecs.emplace(entity, controllerComponentID, NULL);
	}
}



void appStart() {
	profileLinesStart();
	initSystems();
	profileLinesEnd();

	stressTest();
	EntityID camEntity = ecs.getNewEntity();
	unsigned int width, height;
	getCanvasSize(&width, &height);
}

void appLoop() {
	ecs.runSystems();
	auto systemsDebugInfo = ecs.getDebugInfo();
	int x = 16, y = 16, w = 16;
	std::string entityCountStr = "EntityCount->";
	entityCountStr += std::to_string(ecs.getEntityCount());
	drawText(entityCountStr.c_str(), x, y, w);
	y += w + w / 2;
	for (auto& di : systemsDebugInfo) {
		drawText(di.c_str(), x, y, w);
		y += w + w / 2;
	}
	//drawText(std::to_string(TextureCodex::refCount[1]).c_str(), 600, 200, 16);
}

void appEnd() {

}