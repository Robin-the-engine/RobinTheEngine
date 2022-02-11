
struct VS_INPUT
{
    float3 inPos : POSITION;
    float3 inNormal : NORMAL;
	float3 inTangent : TANGENT;
	float3 inBitangent : BITANGENT;
    float2 inTexCoord : TEXCOORD;
};



struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
    float3 outNormal : NORMAL;
    float3 outWorldPos : WORLD_POS;
	float3 outPositionVS   : VIEWSPACE_POS; 
};

cbuffer Camera : register(b0)
{
    float4x4 view;
    float4x4 projection;
}

cbuffer Model : register(b1)
{
    float4x4 model;
}

VS_OUTPUT main(VS_INPUT input)
{
    float3 LightPoint = float3(0.0f, 0.f, 3.0f);
    float4 BasicCollor = float4(0.f, 0.8f, 0.5f, 1.0f);
    float4x4 mvp = mul( model, mul( view, projection));

    VS_OUTPUT output;
    output.outTexCoord = input.inTexCoord;
    output.outPos = mul(float4(input.inPos, 1), mvp);
    output.outNormal = normalize(mul(float4(input.inNormal, 0.0f), model));
    output.outWorldPos = mul(float4(input.inPos, 1), model);
	output.outPositionVS = mul(view ,mul(float4(input.inPos, 1), model)).xyz;

    return output;
}