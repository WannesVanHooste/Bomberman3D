#include "stdafx.h"
#include "SkyBoxScene.h"
#include "SkyBox.h"
#include "../../Materials/SkyBoxMaterial.h"


SkyBoxScene::SkyBoxScene() :
	GameScene(L"SkyboxScene")
{}

void SkyBoxScene::Initialize()
{
	const auto gameContext = GetGameContext();
	//TODO add skybox
	SkyBox* skybox = new SkyBox{};

	AddChild(skybox);

}

void SkyBoxScene::Update()
{}

void SkyBoxScene::Draw()
{}
