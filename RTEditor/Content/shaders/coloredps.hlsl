
cbuffer cByffer : register(b0)
{
    float3 ambientColor;
    float ambientStrength;
    
    float3 diffuseColor;
    float diffuseStrength;
    
    float3 lightPosition;
    
    float3 viewPositon;
    float specularStrength;
}

cbuffer inColor: register(b1) {

    float4 inputColor;
}

struct PS_INPUT
{
    float4 inPos : SV_POSITION;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POS;
};


float4 main(PS_INPUT input) : SV_Target
{
    //ambient
    float3 ambient = ambientColor * ambientStrength;
    
    //diffuse
    float3 normal = normalize(input.inNormal);
    float3 lightDir = normalize(lightPosition - input.inWorldPos);
    float intensity = max(dot(normal, lightDir), 0.0f);
    float3 diffuse = intensity * diffuseStrength * diffuseColor;
    
    float3 viewDir = normalize(viewPositon - input.inWorldPos);
    float3 reflectionDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.f), 8);
    float3 specular = diffuseColor * spec * specularStrength;
    
    float3 color = inputColor;
    color *= ambient + diffuse + specular;
    return float4(color, 1);
}