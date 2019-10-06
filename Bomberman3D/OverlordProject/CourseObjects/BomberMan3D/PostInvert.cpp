#include "stdafx.h"
#include "PostInvert.h"
#include "RenderTarget.h"

PostInvert::PostInvert()
	: PostProcessingMaterial(L"./Resources/Effects/Post/Invert.fx", 1),
	m_pTextureMapVariabele(nullptr)
{
}

void PostInvert::LoadEffectVariables() {
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

void PostInvert::UpdateEffectVariables(RenderTarget* pRendertarget) {
	if (m_pTextureMapVariabele)
	{
		m_pTextureMapVariabele->SetResource(pRendertarget->GetShaderResourceView());
	}
}
