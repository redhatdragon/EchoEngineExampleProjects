#pragma once
#include <stdint.h>
#include <FlatBuffer.h>



/*
NOTE: in prototype phase, likely volatile API.
A (not yet but will be) modular voxel+phyyyyysics system??~
Origan for this 2D variant is bottom left. +x is right and +y is up.
To use this library impliment getVec2DFromWorldVec2D to convert from
your coordinate system to this system before using your vectors elsewhere.
Check constexpr variables to find/modify dimensions.
*/



typedef uint16_t VoxelBlockIndex;
typedef uint16_t VoxelChunkIndex;
typedef uint8_t VoxelBlockType;

#define AIR_TYPE_ID 0

//TODO: reconsider the existance of this or redesign API
struct BlockPos {
	VoxelChunkIndex chunkIndex;
	VoxelBlockIndex blockIndex;

	bool operator==(const BlockPos& other) {
		uint32_t* selfAsInt = (uint32_t*)this;
		uint32_t* otherAsInt = (uint32_t*)&other;
		if (*selfAsInt == *otherAsInt)
			return true;
		return false;
	}
};

struct VoxelBlockMetaData {
	uint8_t mass, maxLoad, lightOpacity;
	uint32_t max_health, hardness;
	void* userData;
};

VoxelBlockMetaData voxelBlockMetaData[256] = {
	{
		0, 0, 0,
		0, 0
	},
	{
		10, 20, 0,
		100, 1
	}
};



struct VoxelBlock {
	struct FlagData {
		uint8_t isSupporting : 1;  //Has no air blocks under itself
	};

	VoxelBlockType typeID;
	FlagData flags;
	uint16_t stability;
	uint32_t damageTaken;

	inline void init(VoxelBlockType _typeID) {
		typeID = _typeID;
		damageTaken = 0;
	}
	inline void init(VoxelBlockType _typeID, uint32_t _damageTaken) {
		typeID = _typeID;
		damageTaken = _damageTaken;
	}

	inline const VoxelBlockMetaData& getVoxelBlockMetaData() {
		return voxelBlockMetaData[typeID];
	}
	inline bool isAir() {
		if (typeID)
			return true;
		return false;
	}
	inline bool isSupporting() {
		return flags.isSupporting;
	}
};
struct VoxelChunk {
	static constexpr uint32_t MAX_WIDTH = 256, MAX_HEIGHT = 256;
	static constexpr uint32_t MAX_AREA = MAX_WIDTH * MAX_HEIGHT;
	VoxelBlock blocks[MAX_WIDTH * MAX_HEIGHT];
};
struct VoxelWorld {
	static constexpr uint32_t MAX_WIDTH = 256, MAX_HEIGHT = 1;
	static constexpr uint32_t MAX_AREA = MAX_WIDTH * MAX_HEIGHT;
	static constexpr uint32_t MAX_STABILITY_QUE = VoxelChunk::MAX_WIDTH * VoxelChunk::MAX_HEIGHT * 50;
	static constexpr uint32_t updateWidth = 15;
	VoxelChunk chunks[MAX_WIDTH * MAX_HEIGHT];
	FlatBuffer<BlockPos, MAX_STABILITY_QUE> stabilityQue;
	struct {
		uint32_t buffer[updateWidth * VoxelChunk::MAX_HEIGHT];
		uint32_t offset;

		inline void init(uint32_t _offset) {
			clear();
			offset = _offset;
		}
		inline void clear() {
			//WARNING: this needs optimizing probably when moved to 3D
			memset(&buffer[0], 0, sizeof(buffer));
		}
		inline void applyValues() {
			for (uint32_t x = 0; x < updateWidth; x++) {
				for (uint32_t y = 1; y < 256; y++) {
					Vec2D<uint32_t> vecPos = {x, y};
					uint32_t memberOffset = offsetof(VoxelWorld, stabilityBackBuffer);
					VoxelWorld* voxelWorld = (VoxelWorld*)((uint8_t*)this-memberOffset);
					BlockPos blockPos = voxelWorld->getBlockPosFromVecPos(vecPos);
					VoxelBlock& block = voxelWorld->chunks[blockPos.chunkIndex].blocks[blockPos.blockIndex];
					if (block.stability = buffer[x + y * updateWidth] == 0)
						voxelWorld->removeBlock(blockPos);
				}
			}
		}
		inline uint32_t get(const Vec2D<uint32_t>& pos) {
			Vec2D<uint32_t> actualPos = pos;
			actualPos.x -= offset;
			return buffer[actualPos.x + actualPos.y * updateWidth];
		}
		inline void add(const Vec2D<uint32_t>& pos, uint32_t value) {
			Vec2D<uint32_t> actualPos = pos;
			actualPos.x -= offset;
			buffer[actualPos.x + actualPos.y * updateWidth] = value;
		}
	} stabilityBackBuffer;

	void init() {
		memset(chunks, 0, sizeof(chunks));
	}
	void tick();

	inline uint32_t getWorldTotalWidth() {
		return MAX_WIDTH * VoxelChunk::MAX_WIDTH;
	}

	inline VoxelBlock& getBlockRef(const BlockPos blockPos) {
		return chunks[blockPos.chunkIndex].blocks[blockPos.blockIndex];
	}

	inline void placeBlock(const VoxelBlock& block, const Vec2D<uint32_t>& pos);
	inline void placeBlock(const VoxelBlock& block, BlockPos blockPos);
	inline void placeBlock(VoxelBlockType typeID, const Vec2D<uint32_t>& pos);
	inline void placeBlock(VoxelBlockType typeID, BlockPos blockPos);

	inline void removeBlock(const Vec2D<uint32_t>& pos);
	inline void removeBlock(BlockPos blockPos);

	inline Vec2D<uint32_t> getVecPosFromWorldVecPos(const Vec2D<uint32_t>& pos);  // USER DEFINED!
	inline Vec2D<uint32_t> getWorldVecPosFromVecPos(const Vec2D<uint32_t>& pos);  // USER DEFINED!

	inline BlockPos getBlockPosFromWorldVecPos(const Vec2D<uint32_t>& worldPos) {
		Vec2D<uint32_t> localPos = getVecPosFromWorldVecPos(worldPos);
		return getBlockPosFromVecPos(localPos);
	}
	inline Vec2D<uint32_t> getWorldVecPosFromBlockPos(BlockPos blockPos) {
		Vec2D<uint32_t> localPos = getVecPosFromBlockPos(blockPos);
		return getWorldVecPosFromVecPos(localPos);
	}

private:
	inline BlockPos getBlockPosFromVecPos(const Vec2D<uint32_t>& pos);
	inline Vec2D<uint32_t> getVecPosFromBlockPos(BlockPos blockPos);
	inline void deferStabilityCheck(BlockPos blockPos);
	inline FlatBuffer<BlockPos, 4> getValidNeighborPositions(BlockPos blockPos);
	inline VoxelBlock& getBlock(const BlockPos& blockPos);

	void tickCheckStability();
	void recalculateStabilityInArea(BlockPos blockPos);
	void startRecalculateStabilityFromAnchoredBlock(BlockPos blockPos);
	void recursiveRecalculateStabilityForBlock(BlockPos blockPos, BlockPos parentBlockPos);
};
constexpr uint32_t sizeofChunk = sizeof(VoxelChunk);
constexpr uint32_t sizeofWorld = sizeof(VoxelWorld);
constexpr uint32_t sizeof3DChunk = sizeof(VoxelBlock) * 64 * 64 * 256;

#include "VoxelImpl.h"