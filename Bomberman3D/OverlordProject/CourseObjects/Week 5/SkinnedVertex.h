#pragma once
#include "VertexHelper.h"

struct SkinnedVertex {
	SkinnedVertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT4 color, float blendWeight1 = 0, float blendWeight2 = 0)
	: TransformedVertex(position, normal, color),
	OriginalVertex(position, normal, color),
	BlendWeight1(blendWeight1),
	BlendWeight2(blendWeight2)
	{}

	VertexPosNormCol TransformedVertex;
	VertexPosNormCol OriginalVertex;
	float BlendWeight1;
	float BlendWeight2;
};