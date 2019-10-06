#include "stdafx.h"
#include "BomberMan3DLevel.h"
#include "SolidBlock.h"
#include "TransformComponent.h"
#include "BreakableBlock.h"

std::vector<BlockInfo> BomberMan3DLevel::m_BlockInfo;
std::vector<GameObject*> BomberMan3DLevel::m_Blocks;

BomberMan3DLevel::BomberMan3DLevel(int cols, int rows)
	:m_Cols(cols),m_Rows(rows)
{
	for(int i{}; i < rows; ++i) {
		for(int j{}; j < cols; ++j) {
			if((i == 1 && (j == 1 || j == 2) || (i == 2 && j == 1))) {
				m_BlockInfo.push_back(BlockInfo{ true,false,DirectX::XMFLOAT3{j * 5.0f,0,i*5.0f} });
			}
			else if((i == (rows-2) && (j == cols - 2 || j == cols - 3)) || (i == (rows - 3) && j == cols -2)) {
				m_BlockInfo.push_back(BlockInfo{ true,false,DirectX::XMFLOAT3{j * 5.0f,0,i*5.0f} });
			}
			else if (i == 0 || i == (rows -1) || j == 0 || j == cols - 1) {
				m_BlockInfo.push_back(BlockInfo{ false,true,DirectX::XMFLOAT3{j * 5.0f,0,i*5.0f} });
			}
			else if(i % 2 == 0 && j % 2 == 0) {
				m_BlockInfo.push_back(BlockInfo{ false,true,DirectX::XMFLOAT3{j * 5.0f,0,i*5.0f} });
			}
			else {
				m_BlockInfo.push_back(BlockInfo{ true,true,DirectX::XMFLOAT3{j * 5.0f,0,i * 5.0f} });
			}
		}
	}
}

void BomberMan3DLevel::SetHasBlock(int index, bool hasBlock) {
	m_BlockInfo[index].hasBlock = hasBlock;
}

void BomberMan3DLevel::ResetBlocks() {
	for(auto element: m_Blocks) {
		SolidBlock* solidBlock = dynamic_cast<SolidBlock*>(element);
		BreakableBlock* breakableBlock = dynamic_cast<BreakableBlock*>(element);
		if(solidBlock != nullptr) {
			solidBlock->ResetBlock();
		}
		if(breakableBlock != nullptr) {
			breakableBlock->ResetBlock();
		}
	}
}

void BomberMan3DLevel::Initialize(const GameContext& ) {
	for (int i{}; i < m_Rows; ++i) {
		for (int j{}; j < m_Cols; ++j) {
			if(!m_BlockInfo[i * m_Cols + j].isBreakable) {
				m_Blocks.push_back(new SolidBlock{});
				AddChild(m_Blocks[i * m_Cols + j]);
				m_Blocks[i * m_Cols + j]->GetTransform()->Translate(m_BlockInfo[i * m_Cols + j].position);
				m_Blocks[i * m_Cols + j]->GetTransform()->Rotate(90, 0, 0);
			}
			else {
				if (m_BlockInfo[i * m_Cols + j].hasBlock)
				{
					m_Blocks.push_back(new BreakableBlock{});
					AddChild(m_Blocks[i * m_Cols + j]);
					m_Blocks[i * m_Cols + j]->GetTransform()->Translate(m_BlockInfo[i * m_Cols + j].position);
					m_Blocks[i * m_Cols + j]->GetTransform()->Rotate(90, 0, 0);
				}
				else {
					m_Blocks.push_back(new GameObject{});
					AddChild(m_Blocks[i * m_Cols + j]);
					m_Blocks[i * m_Cols + j]->GetTransform()->Translate(m_BlockInfo[i * m_Cols + j].position);
				}
			}
		}
	}
}

void BomberMan3DLevel::PostInitialize(const GameContext&) {}

void BomberMan3DLevel::Update(const GameContext&) {
	
}
