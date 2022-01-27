#pragma once
#include "RobinTheEngine/Scene/Component.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

namespace RTE {

	struct BoundingRay
	{
		DirectX::XMFLOAT4 origin;
		DirectX::XMFLOAT4 direction;
	};


	struct Collider : public Component
	{
		enum ColliderType : uint32_t
		{
			SPHERE,
			BOX
		};

		DirectX::BoundingSphere sphere;
		DirectX::BoundingOrientedBox box;

	public:
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 origin;
		DirectX::XMFLOAT4 direction;
		float sphereRadius;
		DirectX::XMFLOAT3 boxExtends;
		ColliderType type;
		Collider() : sphereRadius(1), boxExtends(1, 1, 1), color(1.f,1.f,1.f,1.f) {}
		void Update(XMFLOAT4X4 world) {
			sphere.Radius = sphereRadius;
		sphere.Center = origin;

			//sphere.Transform(sphere, XMLoadFloat4x4(&world));

			box.Center = origin;
			box.Extents = boxExtends;
			box.Orientation = direction;
			//box.Transform(box, XMLoadFloat4x4(&world));
			//BoundingSphere newSphere;
			//sphere.Transform(&sphere, XMLoadFloat4x4(&world));
			//box = BoundingOrientedBox(origin, boxExtends, direction);

		}

		bool Intersects(Collider& coll) {
			switch (coll.type)
			{
			case (SPHERE):
				if (this->type == BOX) {
					auto a = box.Contains(coll.sphere);
					return a == DirectX::ContainmentType::INTERSECTS;
				}
				else {
					auto a = sphere.Contains(coll.sphere);
					return a == DirectX::ContainmentType::INTERSECTS;
				}
				break;

			case (BOX):
				if (this->type == BOX) {
					auto a = box.Contains(coll.box);
					return a == DirectX::ContainmentType::INTERSECTS;
				}
				else {
					auto a = sphere.Contains(coll.box);
					return a == DirectX::ContainmentType::INTERSECTS;
				}

				break;
			default:
				return false;
				break;
			}

		}
	};




}
