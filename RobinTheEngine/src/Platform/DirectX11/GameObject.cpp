#include "rtepch.h"
#include "GameObject.h"

namespace RTE::Deprecated
{
	/*
	bool GameObject::Initialize(const std::string& path, ConstantBuffer<CB_VS_MATRIX4x4>& cb_vs_vertexshader)
	{
		if (!model.Initialize(path, cb_vs_vertexshader))
			return false;

		XMFLOAT3 scale = XMFLOAT3(1, 1, 1);
		this->scaleVec = XMLoadFloat3(&scale);
		this->SetPosition(0.0f, 0.0f, 0.0f);
		this->SetRotation(0.0f, 0.0f, 0.0f);
		this->UpdateWorldMatrix();
		return true;
	}

	void GameObject::Draw(const XMMATRIX& viewProjectionMatrix)
	{
		this->model.Draw(this->worldMatrix, viewProjectionMatrix);
	}

	void GameObject::UpdateWorldMatrix()
	{
		this->worldMatrix = XMMatrixScalingFromVector(this->scaleVec);
		this->worldMatrix *= XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, this->rot.z) * XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
		XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, this->rot.y, 0.0f);
		this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
		this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
		this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
		this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
	}

	const XMVECTOR& GameObject::GetPositionVec()
	{
		return this->posVec;
	}

	const XMFLOAT3& GameObject::GetPositionFloat3()
	{
		return this->pos;
	}

	const XMVECTOR& GameObject::GetRotationVec()
	{
		return this->rotVec;
	}

	const XMFLOAT3& GameObject::GetRotationFloat3()
	{
		return this->rot;
	}

	void GameObject::SetPosition(const XMVECTOR& pos)
	{
		XMStoreFloat3(&this->pos, pos);
		this->posVec = pos;
		this->UpdateWorldMatrix();
	}

	void GameObject::SetPosition(const XMFLOAT3& pos)
	{
		this->pos = pos;
		this->posVec = XMLoadFloat3(&this->pos);
		this->UpdateWorldMatrix();
	}

	void GameObject::SetPosition(float x, float y, float z)
	{
		this->pos = XMFLOAT3(x, y, z);
		this->posVec = XMLoadFloat3(&this->pos);
		this->UpdateWorldMatrix();
	}

	void GameObject::AdjustPosition(const XMVECTOR& pos)
	{
		this->posVec += pos;
		XMStoreFloat3(&this->pos, this->posVec);
		this->UpdateWorldMatrix();
	}

	void GameObject::AdjustPosition(const XMFLOAT3& pos)
	{
		this->pos.x += pos.y;
		this->pos.y += pos.y;
		this->pos.z += pos.z;
		this->posVec = XMLoadFloat3(&this->pos);
		this->UpdateWorldMatrix();
	}

	void GameObject::AdjustPosition(float x, float y, float z)
	{
		this->pos.x += x;
		this->pos.y += y;
		this->pos.z += z;
		this->posVec = XMLoadFloat3(&this->pos);
		this->UpdateWorldMatrix();
	}

	void GameObject::SetRotation(const XMVECTOR& rot)
	{
		this->rotVec = rot;
		XMStoreFloat3(&this->rot, rot);
		this->UpdateWorldMatrix();
	}

	void GameObject::SetScale(float x, float y, float z)
	{
		XMFLOAT3 vec = XMFLOAT3(x, y, z);
		this->scaleVec = XMLoadFloat3(&vec);
		UpdateWorldMatrix();

	}

	void GameObject::SetRotation(const XMFLOAT3& rot)
	{
		this->rot = rot;
		this->rotVec = XMLoadFloat3(&this->rot);
		this->UpdateWorldMatrix();
	}

	void GameObject::SetRotation(float x, float y, float z)
	{
		this->rot = XMFLOAT3(x, y, z);
		this->rotVec = XMLoadFloat3(&this->rot);
		this->UpdateWorldMatrix();
	}

	void GameObject::AdjustRotation(const XMVECTOR& rot)
	{
		this->rotVec += rot;
		XMStoreFloat3(&this->rot, this->rotVec);
		this->UpdateWorldMatrix();
	}

	void GameObject::AdjustRotation(const XMFLOAT3& rot)
	{
		this->rot.x += rot.x;
		this->rot.y += rot.y;
		this->rot.z += rot.z;
		this->rotVec = XMLoadFloat3(&this->rot);
		this->UpdateWorldMatrix();
	}

	void GameObject::AdjustRotation(float x, float y, float z)
	{
		this->rot.x += x;
		this->rot.y += y;
		this->rot.z += z;
		this->rotVec = XMLoadFloat3(&this->rot);
		this->UpdateWorldMatrix();
	}

	void GameObject::SetLookAtPos(XMFLOAT3 lookAtPos)
	{
		//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
		if (lookAtPos.x == this->pos.x && lookAtPos.y == this->pos.y && lookAtPos.z == this->pos.z)
			return;

		lookAtPos.x = this->pos.x - lookAtPos.x;
		lookAtPos.y = this->pos.y - lookAtPos.y;
		lookAtPos.z = this->pos.z - lookAtPos.z;

		float pitch = 0.0f;
		if (lookAtPos.y != 0.0f)
		{
			const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
			pitch = atan(lookAtPos.y / distance);
		}

		float yaw = 0.0f;
		if (lookAtPos.x != 0.0f)
		{
			yaw = atan(lookAtPos.x / lookAtPos.z);
		}
		if (lookAtPos.z > 0)
			yaw += XM_PI;

		this->SetRotation(pitch, yaw, 0.0f);
	}

	const XMVECTOR& GameObject::GetForwardVector()
	{
		return this->vec_forward;
	}

	const XMVECTOR& GameObject::GetRightVector()
	{
		return this->vec_right;
	}

	const XMVECTOR& GameObject::GetBackwardVector()
	{
		return this->vec_backward;
	}

	const XMVECTOR& GameObject::GetLeftVector()
	{
		return this->vec_left;
	}

	void GameObject::SetTexturePath(unsigned int meshIndex, unsigned int textureIndex, const std::string& path)
	{
		model.meshes[meshIndex].textures[textureIndex] = Texture(path, aiTextureType_DIFFUSE);
	}
	*/

}
