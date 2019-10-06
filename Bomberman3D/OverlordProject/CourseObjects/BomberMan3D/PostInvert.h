#pragma once
#include <PostProcessingMaterial.h>

class ID3D11EffectShaderResourceVariable;


class PostInvert :
	public PostProcessingMaterial
{
public:
	PostInvert();
	PostInvert(const PostInvert& other) = delete;
	PostInvert(PostInvert&& other) noexcept = delete;
	PostInvert& operator=(const PostInvert& other) = delete;
	PostInvert& operator=(PostInvert&& other) noexcept = delete;
	virtual ~PostInvert() = default;

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(RenderTarget* pRendertarget) override;
private:
	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele;
};

