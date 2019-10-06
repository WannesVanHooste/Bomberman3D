#include "stdafx.h"
#include "SkyBox.h"
#include "../OverlordProject/Materials/SkyBoxMaterial.h"

SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::Initialize(const GameContext& gameContext) {

	model = new ModelComponent{ L"./Resources/Meshes/Box.ovm" };
	SkyBoxMaterial* material = new SkyBoxMaterial{};

	material->SetCubeTexture(L"./Resources/Textures/SkyBox.dds");
	gameContext.pMaterialManager->AddMaterial(material, 0);
	model->SetMaterial(0);

	AddComponent(model);
	
}
