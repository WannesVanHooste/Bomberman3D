#include "stdafx.h"
#include "CameraComponent.h"
#include "OverlordGame.h"
#include "TransformComponent.h"
#include "PhysxProxy.h"
#include "GameObject.h"
#include "GameScene.h"
#include "RigidBodyComponent.h"


CameraComponent::CameraComponent():
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(DirectX::XM_PIDIV4),
	m_Size(25.0f),
	m_IsActive(true),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, DirectX::XMMatrixIdentity());
}

void CameraComponent::Initialize(const GameContext&) {}

void CameraComponent::Update(const GameContext&)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	const auto windowSettings = OverlordGame::GetGameSettings().Window;
	DirectX::XMMATRIX projection;

	if(m_PerspectiveProjection)
	{
		projection = DirectX::XMMatrixPerspectiveFovLH(m_FOV, windowSettings.AspectRatio ,m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size>0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
		const float viewHeight = (m_Size>0) ? m_Size : windowSettings.Height;
		projection = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const DirectX::XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const DirectX::XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const DirectX::XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const DirectX::XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const DirectX::XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::Draw(const GameContext&) {}

void CameraComponent::SetActive()
{
	auto gameObject = GetGameObject();
	if(gameObject == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game object is null");
		return;
	}

	auto gameScene = gameObject->GetScene();
	if(gameScene == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game scene is null");
		return;
	}

	gameScene->SetActiveCamera(this);
}

GameObject* CameraComponent::Pick(const GameContext& gameContext, CollisionGroupFlag ignoreGroups) const
{
	using namespace DirectX;
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(ignoreGroups);
		
	//TODO implement
	auto proxy = GetGameObject()->GetScene()->GetPhysxProxy();

	GameSettings::WindowSettings window = OverlordGame::GetGameSettings().Window;

	float mousePosX = float(gameContext.pInput->GetMousePosition().x);
	float mousePosY = float(gameContext.pInput->GetMousePosition().y);

	mousePosX = (mousePosX - (window.Width / 2.0f)) / (window.Width / 2.0f);
	mousePosY = ((window.Height / 2.0f) - mousePosY) / (window.Height / 2.0f);


	DirectX::XMMATRIX viewproj = DirectX::XMLoadFloat4x4(&GetViewProjectionInverse());
	XMFLOAT3 nearPoint ={ float(mousePosX), float(mousePosY), 0.0f };
	XMVECTOR nearVec = XMLoadFloat3(&nearPoint);
	nearVec = DirectX::XMVector3TransformCoord(nearVec, viewproj);
	DirectX::XMStoreFloat3(&nearPoint, nearVec);
	physx::PxVec3 pxNearP = { nearPoint.x, nearPoint.y, nearPoint.z };


	XMFLOAT3 farPoint = { float(mousePosX), float(mousePosY), 1.0f };
	XMVECTOR farVec = XMLoadFloat3(&farPoint);
	farVec = DirectX::XMVector3TransformCoord(farVec, viewproj);
	DirectX::XMStoreFloat3(&farPoint, farVec);
	physx::PxVec3 pxfarP = { farPoint.x, farPoint.y, farPoint.z };

	physx::PxVec3 startTrace = pxNearP;
	physx::PxVec3 rayDir = (pxfarP - pxNearP);
	//rayDir.normalize();


	physx::PxQueryFilterData filterData;
	filterData.data.word0 = ~ignoreGroups;

	physx::PxRaycastBuffer hit;
	if(proxy->Raycast(startTrace, rayDir.getNormalized(), PX_MAX_F32, hit, physx::PxHitFlag::eDEFAULT, filterData)) 
	{
		physx::PxRaycastHit hit1 = hit.getAnyHit(0);
		if (hit1.actor->userData)
		{
			//transformcomponent because every gameobject has one so its 100% sure this one is gona have one too
			return static_cast<RigidBodyComponent*>(hit1.actor->userData)->GetGameObject();
		}
	}
	return nullptr;
}