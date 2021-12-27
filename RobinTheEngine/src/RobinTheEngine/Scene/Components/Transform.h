#pragma once
#include "RobinTheEngine/Scene/Component.h"
#include "RobinTheEngine/Core.h"
#include <sstream>
#include <DirectXMath.h>

namespace RTE
{
	struct Transform : public Component
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;
		using XMMATRIX = DirectX::XMMATRIX;

	private:
		Transform* parent = nullptr;
		XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

	public:
		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const XMFLOAT3 position, const XMFLOAT3 rotation, const XMFLOAT3 scale)
			: position(position), rotation(rotation), scale(scale) { }
		Transform(const XMFLOAT3 position, const XMFLOAT3 rotation, const XMFLOAT3 scale, Transform& parent)
			: position(position), rotation(rotation), scale(scale), parent(&parent) { }

		inline XMFLOAT3 GetPosition() const { return position; };
		inline XMFLOAT3 GetRotation() const { return rotation; };
		inline XMFLOAT3 GetScale() const { return scale; };

		inline void SetPosition(float x, float y, float z) { SetPosition({ x, y, z }); }
		inline void SetPosition(const XMFLOAT3& position) { this->position = position; }

		inline void SetRotation(float x, float y, float z) { SetRotation({ x, y, z }); }
		inline void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }

		inline void SetScale(float x, float y, float z) { SetScale({ x, y, z }); }
		inline void SetScale(const XMFLOAT3 & scale) { this->scale = scale; }

		inline bool HasParent() const { return parent; }
		inline Transform& GetParent() const
		{
			RTE_ASSERT(HasParent(), "This GameObject hasn't parent.");
			return *parent;
		}
		inline void SetParent(Transform& parent) { this->parent = &parent; }


		XMMATRIX GetMatrix() const
		{
			using namespace DirectX;

			XMMATRIX transformMatrix = XMMatrixScalingFromVector(XMLoadFloat3(&scale));
			transformMatrix *= XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)) * XMMatrixTranslationFromVector(XMLoadFloat3(&position));
			return transformMatrix;
		}

		std::ostream& operator << (std::ostream& outs)
		{
			return outs << to_string();
		}

		std::string to_string()
		{
			std::ostringstream ss;
			ss << 
				"Position: { " << position.x << ", " << position.y << ", " << position.z << " }\n" <<
				"Rotation: { " << rotation.x << ", " << rotation.y << ", " << rotation.z << " }\n" <<
				"Scale: { " << scale.x << ", " << scale.y << ", " << scale.z << " }";
			return ss.str();
		}

		friend class Serializer;
	};
}