#include <DDECS.h>
#include <PhysicsEngineAABB.h>
#include <Asset.h>
#include <iostream>
#include <IO_API/IO_API.h>

class SystemDisplay : public System {
	ComponentID textureComponentID = -1;
	ComponentID bodyComponentID = -1;
	ComponentID cameraComponentID = -1;
public:
	virtual void init() {
		textureComponentID = ecs.registerComponent("texture", sizeof(TextureID));
		bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
		cameraComponentID = ecs.registerComponent("camera", NULL);
	}
	virtual void run() {
		clock_t startTime = clock();

		Vec2D<int32_t> camPos = { 0, 0 };
		if (ecs.getComponentCount(cameraComponentID)) {
			EntityID entity = ecs.getOwner(cameraComponentID, 0);
			if (ecs.entityHasComponent(entity, bodyComponentID)) {
				BodyID bodyID = ecs.getEntityComponentAs<BodyID>(entity, bodyComponentID);
				camPos = physics.getPos<int32_t>(bodyID);
			}
		}
		unsigned int winWidth, winHeight;
		getCanvasSize(&winWidth, &winHeight);
		int cx = -(camPos.x-winWidth / 2), cy = -(camPos.y-winHeight/2);

		uint32_t texCount = ecs.getComponentCount(textureComponentID);
		for (uint32_t i = 0; i < texCount; i++) {
			EntityID entity = ecs.getOwner(textureComponentID, i);
			if (ecs.entityHasComponent(entity, bodyComponentID) == false)
				continue;
			BodyID bodyID = ecs.getEntityComponentAs<BodyID>(entity, bodyComponentID);
			auto pos = physics.getPos<int32_t>(bodyID);
			auto siz = physics.getSize<int32_t>(bodyID);
			TextureID texID = ecs.getEntityComponentAs<TextureID>(entity, textureComponentID);
			drawTexture(TextureCodex::get(texID), pos.x+cx, pos.y+cy, siz.x, siz.y);
		}

		Vec2D<uint32_t> camStart = { cx, cy };
		Vec2D<uint32_t> camEnd = camStart;
		camEnd.x += winWidth;
		camEnd.y += winHeight;
		BlockPos voxelPosStart = voxelWorld.getBlockPosFromWorldVecPos(camStart);
		BlockPos voxelPosEnd = voxelWorld.getBlockPosFromWorldVecPos(camEnd);

		for (uint32_t chunkIndex = voxelPosStart.chunkIndex; chunkIndex < voxelPosEnd.chunkIndex; chunkIndex++) {
			for (uint32_t blockIndex = 0; blockIndex < VoxelChunk::MAX_AREA; blockIndex++) {
				BlockPos blockPos = { chunkIndex, blockIndex };
				if (voxelWorld.getBlockRef(blockPos).isAir() == false) {
					Vec2D<uint32_t> pos = voxelWorld.getWorldVecPosFromBlockPos(blockPos);
					drawRect(pos.x, pos.y, 64, 64, 255, 125, 65, 255);
				}
			}
		}

		ms = clock() - startTime;
	}
	virtual const char* getName() {
		return "SystemDisplay";
	}
};