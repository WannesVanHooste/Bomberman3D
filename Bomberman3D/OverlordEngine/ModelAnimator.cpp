#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(std::vector<DirectX::XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	UNREFERENCED_PARAMETER(clipNumber);
	//TODO: complete
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if(!(clipNumber < m_pMeshFilter->m_AnimationClips.size())) {
		Reset();
		Logger::LogFormat(LogLevel::Warning, L"Clip number cant be bigger than vector size");
		return;
	}
	else {
		m_CurrentClip = m_pMeshFilter->m_AnimationClips[clipNumber];
		SetAnimation(m_CurrentClip);
	}
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
}

void ModelAnimator::SetAnimation(std::wstring clipName)
{
	UNREFERENCED_PARAMETER(clipName);
	//TODO: complete
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for(unsigned int i{}; i < m_pMeshFilter->m_AnimationClips.size(); ++i) {
		if(clipName == m_pMeshFilter->m_AnimationClips[i].Name) {
			m_CurrentClip = m_pMeshFilter->m_AnimationClips[i];
			SetAnimation(m_CurrentClip);
			return;
		}
		else {
			Reset();
			Logger::LogFormat(LogLevel::Warning, L"Clip not found");
		}
	}
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message
}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	UNREFERENCED_PARAMETER(clip);
	//TODO: complete
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	UNREFERENCED_PARAMETER(pause);
	//TODO: complete
	//If pause is true, set m_IsPlaying to false
	if(pause) {
		m_IsPlaying = false;
	}

	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;

	//If m_ClipSet is true
	if (m_ClipSet)
	{
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		auto transforms = m_CurrentClip.Keys[0].BoneTransforms;
		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		m_Transforms.assign(transforms.begin(), transforms.end());
	}
	else {
		//Else
		//	Create an IdentityMatrix 
		DirectX::XMMATRIX identityMatrix = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT4X4 identityMatrix4X4{};
		DirectX::XMStoreFloat4x4(&identityMatrix4X4, identityMatrix);
		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		m_Transforms.assign(m_Transforms.size(), identityMatrix4X4);
	}
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	//TODO: complete
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		auto passedTicks = gameContext.pGameTime->GetElapsed() * m_CurrentClip.TicksPerSecond * m_AnimationSpeed;
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		passedTicks = fmod(passedTicks, m_CurrentClip.Duration);

		//2. 
		//IF m_Reversed is true
		if (m_Reversed)
		{
			//	Subtract passedTicks from m_TickCount
			m_TickCount -= passedTicks;
			//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
			if(m_TickCount < 0) {
				m_TickCount += m_CurrentClip.Duration;
			}
		}
		else {
			//ELSE
			//	Add passedTicks to m_TickCount
			m_TickCount += passedTicks;
			//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
			if(m_TickCount > m_CurrentClip.Duration) {
				m_TickCount -= m_CurrentClip.Duration;
			}
		}

		//3.
		//Find the enclosing keys
		AnimationKey keyA, keyB, keyToFind;
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		keyToFind.Tick = m_TickCount;
		auto pred = [](AnimationKey& key1, AnimationKey key2) {return key1.Tick < key2.Tick; };
		auto it = std::lower_bound(m_CurrentClip.Keys.begin(), m_CurrentClip.Keys.end(),keyToFind ,pred);
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		keyA = *it;
		keyB = *prev(it);
		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		auto blendFactor = (m_TickCount - keyA.Tick) / (keyB.Tick - keyA.Tick);
		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		for (unsigned int i{}; i < m_pMeshFilter->m_BoneCount; ++i)
		{
			//	Retrieve the transform from keyA (transformA)
			//	auto transformA = ...
			auto transformA = keyA.BoneTransforms[i];
			auto transformB = keyB.BoneTransforms[i];
			// 	Retrieve the transform from keyB (transformB)
			//	auto transformB = ...
			//	Decompose both transforms
			DirectX::XMVECTOR  posA;
			DirectX::XMVECTOR  scaleA;
			DirectX::XMVECTOR  rotationA;
			DirectX::XMMatrixDecompose(&scaleA, &rotationA, &posA, DirectX::XMLoadFloat4x4(&transformA));

			DirectX::XMVECTOR  posB;
			DirectX::XMVECTOR  scaleB;
			DirectX::XMVECTOR  rotationB;
			DirectX::XMMatrixDecompose(&scaleB, &rotationB, &posB, DirectX::XMLoadFloat4x4(&transformB));
			//	Lerp between all the transformations (Position, Scale, Rotation)
			DirectX::XMVECTOR pos = DirectX::XMVectorLerp(posA, posB, blendFactor);
			DirectX::XMVECTOR scale = DirectX::XMVectorLerp(scaleA, scaleB, blendFactor);
			DirectX::XMVECTOR rotation = DirectX::XMQuaternionSlerp(rotationA, rotationB, blendFactor);

			//	Compose a transformation matrix with the lerp-results
			auto posMatrix = DirectX::XMMatrixTranslationFromVector(pos);
			auto scaleMatrix = DirectX::XMMatrixScalingFromVector(scale);
			auto rotationMatrix = DirectX::XMMatrixRotationQuaternion(rotation);

			auto result = scaleMatrix * rotationMatrix * posMatrix;
			DirectX::XMFLOAT4X4 result4x4{};
			DirectX::XMStoreFloat4x4(&result4x4, result);
			//	Add the resulting matrix to the m_Transforms vector
			m_Transforms.push_back(result4x4);
		}
	}
}
