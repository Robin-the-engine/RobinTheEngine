#pragma once
#include <DirectXMath.h>
#include "RobinTheEngine/Core.h"
#include "Platform/DirectX11/ConstantBuffer.h"
using namespace DirectX;



namespace RTE {

	class RTE_API Camera {

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

		const XMVECTOR & GetForwardVector();
		const XMVECTOR & GetBackwardVector();
		const XMVECTOR & GetRightVector();
		const XMVECTOR & GetLeftVector();
		ConstantBuffer<CB_VS_CAMERA> constBuffer;


	private:
		void UpdateViewMatrix();
		XMVECTOR m_posVec;
		XMVECTOR m_rotVec;
		XMFLOAT3 m_pos;
		XMFLOAT3 m_rot;
		XMMATRIX m_viewMatrix;
		XMMATRIX m_projectionMatrix;


		const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0, 0, 1, 0);
		const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0, 1, 0, 0);
		const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0, 0, -1, 0);
		const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1, 0, 0, 0);
		const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1, 0, 1, 0);
		const XMVECTOR DEFAULT_DOWN_VECTOR = XMVectorSet(0, 1, 0, 0);
		
		XMVECTOR vec_left;
		XMVECTOR vec_right;
		XMVECTOR vec_forward;
		XMVECTOR vec_backward;
		
		

	};






}
