#pragma once
#include "Model.h"


namespace RTE::Deprecated {


	class GameObject
	{
	public:
		bool Initialize(const std::string & path, ConstantBuffer<CB_VS_MATRIX4x4>& cb_vs_vertexshader);
		void Draw(const XMMATRIX & viewProjectionMatrix);

		const XMVECTOR & GetPositionVec();
		const XMFLOAT3 & GetPositionFloat3();
		const XMVECTOR & GetRotationVec();
		const XMFLOAT3 & GetRotationFloat3();

		void SetPosition(const XMVECTOR & pos);
		void SetPosition(const XMFLOAT3 & pos);
		void SetPosition(float x, float y, float z);
		void AdjustPosition(const XMVECTOR & pos);
		void AdjustPosition(const XMFLOAT3 & pos);
		void AdjustPosition(float x, float y, float z);
		void SetRotation(const XMVECTOR & rot);
		void SetRotation(const XMFLOAT3 & rot);
		void SetRotation(float x, float y, float z);
		void SetScale(float x, float y, float z);
		void AdjustRotation(const XMVECTOR & rot);
		void AdjustRotation(const XMFLOAT3 & rot);
		void AdjustRotation(float x, float y, float z);
		void SetLookAtPos(XMFLOAT3 lookAtPos);
		const XMVECTOR & GetForwardVector();
		const XMVECTOR & GetRightVector();
		const XMVECTOR & GetBackwardVector();
		const XMVECTOR & GetLeftVector();
		void SetTexturePath(unsigned int meshIndex, unsigned int textureIndex, const std::string& path);
	private:
		Model model;

		void UpdateWorldMatrix();
		XMMATRIX worldMatrix = XMMatrixIdentity();

		XMVECTOR scaleVec;
		XMVECTOR posVec;
		XMVECTOR rotVec;
		XMFLOAT3 pos;
		XMFLOAT3 rot;

		const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		XMVECTOR vec_forward;
		XMVECTOR vec_left;
		XMVECTOR vec_right;
		XMVECTOR vec_backward;
	};


}
