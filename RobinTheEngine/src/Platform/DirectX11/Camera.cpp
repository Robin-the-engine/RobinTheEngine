#include "rtepch.h"
#include "Camera.h"

RTE::Camera::Camera()
{
	m_pos = XMFLOAT3(0.f, 0.f, 0.f);
	m_rot = XMFLOAT3(0.f, 0.f, 0.f);
	m_rotVec = XMLoadFloat3(&m_rot);
	UpdateViewMatrix();
	constBuffer.InitializeSharedBuffer("Camera");
	DirectX::XMStoreFloat4x4(&constBuffer.data.viewMatrix, GetViewMatrix());
	DirectX::XMStoreFloat4x4(&constBuffer.data.projectionMatrix, GetProjectionMatrix());
	constBuffer.WriteBuffer();
}

void RTE::Camera::SetProjectionProperties(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	//Convert degrees to radians
	float fov = (fovDegrees / 360) * XM_PI;
	m_fovDegrees = fovDegrees;
	m_aspectRatio = aspectRatio;
	m_nearZ = nearZ;
	m_farZ = farZ;

	m_projectionMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);

}

const XMMATRIX& RTE::Camera::GetViewMatrix()
{
	return this->m_viewMatrix;
}

const XMMATRIX& RTE::Camera::GetProjectionMatrix()
{
	return this->m_projectionMatrix;
}

const DirectX::XMMATRIX& RTE::Camera::GetViewProjectionMatrix()
{
	return GetViewMatrix() * GetProjectionMatrix();
}

void RTE::Camera::UpdateBuffer()
{
	UpdateViewMatrix();
	DirectX::XMStoreFloat4x4(&constBuffer.data.viewMatrix, DirectX::XMMatrixTranspose(GetViewMatrix()));
	DirectX::XMStoreFloat4x4(&constBuffer.data.projectionMatrix, DirectX::XMMatrixTranspose(GetProjectionMatrix()) );
	constBuffer.WriteBuffer();

}

const XMVECTOR& RTE::Camera::GetPositionVec()
{
	return XMLoadFloat3(&m_pos);
}

const XMVECTOR& RTE::Camera::GetRotationVec()
{
	return this->m_rotVec;
}

const XMFLOAT3& RTE::Camera::GetPositionFloat3()
{
	return m_pos;
}

const XMFLOAT3& RTE::Camera::GetRotationFloat3()
{
	return m_rot;
}

void RTE::Camera::SetPosition(const XMVECTOR& vec)
{
	XMStoreFloat3(&m_pos, vec);
	UpdateViewMatrix();
}

void RTE::Camera::SetPosition(const XMFLOAT3& vec)
{
	m_pos = vec;
	UpdateViewMatrix();
}

void RTE::Camera::AdjustPosition(const XMVECTOR& vec)
{
	XMVECTOR tmp = XMLoadFloat3(&m_pos); tmp += vec;
	XMStoreFloat3(&m_pos, tmp);
	UpdateViewMatrix();
}

void RTE::Camera::AdjustPosition(const XMFLOAT3& vec)
{
	XMVECTOR tmp = XMLoadFloat3(&m_pos); tmp += XMLoadFloat3(&vec);
	XMStoreFloat3(&m_pos, tmp);
	UpdateViewMatrix();
}

void RTE::Camera::SetRotation(const XMVECTOR& vec)
{
	m_rotVec = vec;
	XMStoreFloat3(&m_rot, m_rotVec);
	UpdateViewMatrix();
}

void RTE::Camera::SetRotation(const XMFLOAT3& vec)
{
	m_rot = vec;
	m_rotVec = XMLoadFloat3(&m_rot);
	UpdateViewMatrix();
}

void RTE::Camera::AdjustRotation(const XMVECTOR& vec)
{
	m_rotVec += vec;
	XMStoreFloat3(&m_rot, m_rotVec);
	UpdateViewMatrix();
}

void RTE::Camera::ResizeCamera(float width, float height)
{
	float aspectRatio = width / height;
	SetProjectionProperties(m_fovDegrees, aspectRatio, m_nearZ, m_farZ);
}

void RTE::Camera::AdjustRotation(const XMFLOAT3& vec)
{
	m_rotVec += XMLoadFloat3(&vec);
	XMStoreFloat3(&m_rot, m_rotVec);
	UpdateViewMatrix();
}

const XMVECTOR RTE::Camera::GetForwardVector()
{
	return XMLoadFloat4(& vec_forward);
}

const XMVECTOR RTE::Camera::GetBackwardVector()
{
	return XMLoadFloat4(&vec_backward);
}

const XMVECTOR RTE::Camera::GetRightVector()
{
	return XMLoadFloat4(&vec_right);
}

const XMVECTOR RTE::Camera::GetLeftVector()
{
	return XMLoadFloat4(&vec_left);
}

void RTE::Camera::UpdateViewMatrix()
{
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(m_rotVec);
	XMVECTOR target = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, rotation);
	XMVECTOR position = XMLoadFloat3(&m_pos); 
	target += position;
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, rotation);

	m_viewMatrix = XMMatrixLookAtLH(position, target, upDir);

	XMMATRIX vecRotationMat = XMMatrixRotationRollPitchYaw(0.f, m_rot.y, 0.f);

	XMStoreFloat4(&vec_forward, XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMat));
	XMStoreFloat4(&vec_backward, XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMat));
	XMStoreFloat4(&vec_left, XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMat));
	XMStoreFloat4(&vec_right, XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMat));

}
