#pragma once
#include "GameScene.h"
#include "MeshDrawComponent.h"
#include <vector>
#include "SkinnedVertex.h"
//#include "BoneObject.h"
class BoneObject;

class SoftwareSkinningScene_2 final : public GameScene
{
public:
	SoftwareSkinningScene_2();
	virtual ~SoftwareSkinningScene_2() = default;

	SoftwareSkinningScene_2(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2(SoftwareSkinningScene_2&& other) noexcept = delete;
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2& other) = delete;
	SoftwareSkinningScene_2& operator=(SoftwareSkinningScene_2&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
private:
	BoneObject *m_pBone0, *m_pBone1;
	float m_BoneRotation;
	int m_RotationSign;

	void CreateMesh(float length);
	MeshDrawComponent* m_pMeshDrawer;
	std::vector<SkinnedVertex> m_SkinnedVertices;
};
