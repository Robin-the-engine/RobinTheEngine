#pragma once
#include "RobinTheEngine/Scene/Component.h"
#include "Platform/DirectX11/LightStructure.h"

namespace RTE
{
	struct LightComponent : public Component
	{
		Light lightComponent;
	};
}