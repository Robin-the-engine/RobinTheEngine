
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
	float3 outPositionVS   : TEXCOORD0; 
	float2 outTexCoord : TEXCOORD1;
    float3 tangentVS    : TANGENT;      // View space tangent.
    float3 binormalVS   : BINORMAL;     // View space binormal.
    float3 normalVS     : NORMAL;       // View space normal.	
	
    float4 outPos : SV_POSITION;
	
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

    float4x4 mvp = mul( model, mul( view, projection));
	float4x4 mv = mul( model, view);
	
    VS_OUTPUT output;
    
    output.outPos = mul(float4(input.inPos, 1), mvp);
	output.outPositionVS = mul(float4(input.inPos, 1), mv).xyz;
	output.tangentVS = mul( ( float3x3 )mv, input.inTangent );
    output.binormalVS = mul( ( float3x3 )mv, input.inBitangent );
    output.normalVS = mul( ( float3x3 )mv, input.inNormal );
	output.outTexCoord = input.inTexCoord;

    return output;
}