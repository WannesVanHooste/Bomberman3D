#include "stdafx.h"
#include "PostChromaticAberration.h"
#include "RenderTarget.h"

PostChromaticAberration::PostChromaticAberration()
	: PostProcessingMaterial(L"./Resources/Effects/Post/ChromaticAberration.fx", 4),
	m_pTextureMapVariabele(nullptr)
{
}

void PostChromaticAberration::LoadEffectVariables() {
	if (!m_pTextureMapVariabele)
	{
		m_pTextureMapVariabele = GetEffect()->GetVariableByName("gTexture")->AsShaderResource();
		if (!m_pTextureMapVariabele->IsValid())
		{
			Logger::LogWarning(L"DiffuseMaterial::LoadEffectVariables() > \'gTexture\' variable not found!");
			m_pTextureMapVariabele = nullptr;
		}
	}
}

void PostChromaticAberration::UpdateEffectVariables(RenderTarget* pRendertarget) {
	if (m_pTextureMapVariabele)
	{
		m_pTextureMapVariabele->SetResource(pRendertarget->GetShaderResourceView());
	}
}
