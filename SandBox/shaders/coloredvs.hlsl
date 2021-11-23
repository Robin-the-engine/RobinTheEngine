
struct VS_INPUT
{
    float3 inPos : POSITION;
    float3 inNormal : NORMAL;
};



struct VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float3 outNormal : NORMAL;
    float3 outWorldPos : WORLD_POS;
};

cbuffer Camera : register(b0)
{
    float4x4 view;
    float4x4 projection;
}

cbuffer Model : register(b1)
{
    float4x4 world;
}


VS_OUTPUT main(VS_INPUT input)
{
    float3 LightPoint = float3(0.0f, 0.f, 3.0f);
    float4 BasicCollor = float4(0.f, 0.8f, 0.5f, 1.0f);
    float4x4 mvp = mul(world, mul( view, projection));

    VS_OUTPUT output;
    output.outPos = mul(float4(input.inPos, 1), mvp);
    output.outNormal = normalize(mul(float4(input.inNormal, 0.0f), world));
    output.outWorldPos = mul(float4(input.inPos, 1), world);

    return output;
}