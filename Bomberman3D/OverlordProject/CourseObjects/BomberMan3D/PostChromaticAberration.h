#pragma once
#include <PostProcessingMaterial.h>

class ID3D11EffectShaderResourceVariable;

class PostChromaticAberration :
	public PostProcessingMaterial
{
public:
	PostChromaticAberration();
	virtual ~PostChromaticAberration() = default;

	PostChromaticAberration(const PostChromaticAberration& other) = delete;
	PostChromaticAberration(PostChromaticAberration&& other) noexcept = delete;
	PostChromaticAberration& operator=(const PostChromaticAberration& other) = delete;
	PostChromaticAberration& operator=(PostChromaticAberration&& other) noexcept = delete;
protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(RenderTarget* pRendertarget) override;
private:
	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;
};

