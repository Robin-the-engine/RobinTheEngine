#pragma once
#include <DirectXMath.h>
using namespace DirectX;


struct Light
{
public:
	Light() = default;
	Light(const Light&) = default;
	enum LightType : uint32_t
	{
		POINT_LIGHT,
		SPOT_LIGHT,
		DIRECTIONAL_LIGHT
	};


	//Position for point and spot lights (View space).
	XMFLOAT4 PositionVS;

	//Direction for spot and directional lights (View space).
	XMFLOAT4 DirectionVS;

	//Color of the light. Diffuse and specular colors are not seperated.
	XMFLOAT4 Color;

	//The half angle of the spotlight cone.
	float    SpotlightAngle;

	//The range of the light.
	float    Range;

	//The intensity of the light.
	float    Intensity;

	//Disable or enable the light.
	uint32_t    Enabled;

	//The type of the light.
	LightType    Type;
	XMFLOAT2 Padding;
};

