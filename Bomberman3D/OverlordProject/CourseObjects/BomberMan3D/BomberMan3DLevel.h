#pragma once
#include <GameObject.h>
#include "BlockInfo.h"
class BomberMan3DLevel :
	public GameObject
{
public:
	BomberMan3DLevel(int cols, int rows);
	~BomberMan3DLevel() = default;

	BomberMan3DLevel(const BomberMan3DLevel& other) = delete;
	BomberMan3DLevel(BomberMan3DLevel&& other) noexcept = delete;
	BomberMan3DLevel& operator=(const BomberMan3DLevel& other) = delete;
	BomberMan3DLevel& operator=(BomberMan3DLevel&& other) noexcept = delete;

	static void SetHasBlock(int index, bool hasBlock);
	static std::vector<BlockInfo> GetBlockInfo() { return m_BlockInfo; }
	static std::vector<GameObject*> GetBlocks() { return m_Blocks; }
	void ResetBlocks();

	static void ClearBlocks() { m_BlockInfo.clear(); m_Blocks.clear(); }
	void Initialize(const GameContext& gameContext) override;
	void PostInitialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;

private:
	static std::vector<BlockInfo> m_BlockInfo;
	static std::vector<GameObject*> m_Blocks;
	int m_Cols;
	int m_Rows;
};

