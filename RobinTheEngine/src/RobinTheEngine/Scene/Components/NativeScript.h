#pragma once
#include "RobinTheEngine/Scene/Component.h"

namespace RTE
{
	struct NativeScript : public Component
	{
	public:
		NativeScript() = default;
		NativeScript(const NativeScript&) = default;

		virtual void OnCreate() {}
		virtual void Update(float deltaTime) {}
		virtual void OnDestroy() {}
	};
}