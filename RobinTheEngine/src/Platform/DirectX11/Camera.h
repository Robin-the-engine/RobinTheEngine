#pragma once
#include <DirectXMath.h>
#include "RobinTheEngine/Core.h"
#include "Platform/DirectX11/ConstantBuffer.h"
#include "RobinTheEngine/Scene/Component.h"
using namespace DirectX;



namespace RTE {

	class RTE_API Camera : public RTE::Component {

	public:

		Camera();
		Camera& operator=(const Camera& c) = default;
		void SetProjectionProperties(float fovDegrees, float aspectRatio, float nearZ, float farZ);

		const XMMATRIX& GetViewMatrix();
		const XMMATRIX& GetProjectionMatrix();
		const XMMATRIX& GetViewProjectionMatrix();
		void UpdateBuffer();

		const XMVECTOR& GetPositionVec();
		const XMVECTOR& GetRotationVec();
		const XMFLOAT3& GetPositionFloat3();
		const XMFLOAT3& GetRotationFloat3();

		void SetPosition(const XMVECTOR& vec);
		void SetPosition(const XMFLOAT3& vec);
		void AdjustPosition(const XMVECTOR& vec);
		void AdjustPosition(const XMFLOAT3& vec);
		void SetRotation(const XMVECTOR& vec);
		void SetRotation(const XMFLOAT3& vec);
		void AdjustRotation(const XMVECTOR& vec);
		void AdjustRotation(const XMFLOAT3& vec);
		void ResizeCamera(float width, float height);

		const XMVECTOR GetForwardVector();
		const XMVECTOR GetBackwardVector();
		const XMVECTOR GetRightVector();
		const XMVECTOR GetLeftVector();
		ConstantBuffer<CB_VS_CAMERA> constBuffer;
		bool GetDirtyFlag() { return dirtyFlag; }
		void SetDirtyFlag(bool newFlag) { dirtyFlag = newFlag; }
		bool* GetActivateFlagPtr() { return &Activate; }
		bool Activate = true;
	private:
		void UpdateViewMatrix();
		//XMVECTOR m_posVec;
		XMVECTOR m_rotVec;
		XMFLOAT3 m_pos;
		XMFLOAT3 m_rot;
		XMMATRIX m_viewMatrix;
		XMMATRIX m_projectionMatrix;


		 XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0, 0, 1, 0);
		XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0, 1, 0, 0);
		XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0, 0, -1, 0);
		XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1, 0, 0, 0);
		XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1, 0, 1, 0);
		XMVECTOR DEFAULT_DOWN_VECTOR = XMVectorSet(0, 1, 0, 0);
		
		XMFLOAT4 vec_left;
		XMFLOAT4 vec_right;
		XMFLOAT4 vec_forward;
		XMFLOAT4 vec_backward;

		float m_fovDegrees;
		float m_aspectRatio;
		float m_nearZ;
		float m_farZ;

		bool dirtyFlag = false;
		
		

	};






}
