#pragma once
#include "Material.h"

class TextureData;

class SkyBoxMaterial : public Material
{
public:
	SkyBoxMaterial();
	~SkyBoxMaterial();

	void SetCubeTexture(const std::wstring& assetFile);
protected:
	void LoadEffectVariables();
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:
	TextureData* m_pCubeTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

	SkyBoxMaterial(const SkyBoxMaterial &obj) = delete;
	SkyBoxMaterial& operator=(const SkyBoxMaterial& obj) = delete;
};

