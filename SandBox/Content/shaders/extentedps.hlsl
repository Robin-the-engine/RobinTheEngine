#ifndef NUM_LIGHTS
#pragma message( "NUM_LIGHTS undefined. Default to 8.")
#define NUM_LIGHTS 8 // should be defined by the application.
#endif

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2














struct Material
{
    float4  AmbientColor;
    //-------------------------- ( 16 bytes )
    float4  EmissiveColor;
    //-------------------------- ( 16 bytes )
    float4  DiffuseColor;
    //-------------------------- ( 16 bytes )
    float4  SpecularColor;
    //-------------------------- ( 16 bytes )
    // Reflective value.
    float4  Reflectance;
    //-------------------------- ( 16 bytes )
    float   Opacity;
    float   SpecularPower;
    // For transparent materials, IOR > 0.
    float   IndexOfRefraction;
    bool    HasAmbientTexture;
    //-------------------------- ( 16 bytes )
    bool    HasEmissiveTexture;
    bool    HasDiffuseTexture;
    bool    HasSpecularTexture;
    bool    HasSpecularPowerTexture;
    //-------------------------- ( 16 bytes )
    bool    HasNormalTexture;
    bool    HasBumpTexture;
    bool    HasOpacityTexture;
    float   BumpIntensity;
    //-------------------------- ( 16 bytes )
    float   SpecularScale;
    float   AlphaThreshold;
    float2  Padding;
    //--------------------------- ( 16 bytes )
};  //--------------------------- ( 16 * 10 = 160 bytes )


struct Light
{

    //--------------------------------------------------------------( 16 bytes )
    /**
    * Position for point and spot lights (View space).
    */
    float4   PositionVS;
    //--------------------------------------------------------------( 16 bytes )
    /**
    * Direction for spot and directional lights (View space).
    */
    float4   DirectionVS;
    //--------------------------------------------------------------( 16 bytes )
    /**
    * Color of the light. Diffuse and specular colors are not seperated.
    */
    float4   Color;
    //--------------------------------------------------------------( 16 bytes )
    /**
    * The half angle of the spotlight cone.
    */
    float    SpotlightAngle;
    /**
    * The range of the light.
    */
    float    Range;

    /**
     * The intensity of the light.
     */
    float    Intensity;

    /**
    * Disable or enable the light.
    */
    bool    Enabled;
    //--------------------------------------------------------------( 16 bytes )


    /**
    * The type of the light.
    */
    uint    Type;
    float2  Padding;
    //--------------------------------------------------------------( 16 bytes )
    //--------------------------------------------------------------( 16 * 7 = 112 bytes )
};


struct Plane
{
    float3 N;   // Plane normal.
    float  d;   // Distance to origin.
};

struct Sphere
{
    float3 c;   // Center point.
    float  r;   // Radius.
};

struct Cone
{
    float3 T;   // Cone tip.
    float  h;   // Height of the cone.
    float3 d;   // Direction of the cone.
    float  r;   // bottom radius of the cone.
};

// Four planes of a view frustum (in view space).
// The planes are:
//  * Left,
//  * Right,
//  * Top,
//  * Bottom.
// The back and/or front planes can be computed from depth values in the 
// light culling compute shader.
struct Frustum
{
    Plane planes[4];   // left, right, top, bottom frustum planes.
};

struct AppData
{
    float3 position : POSITION;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float3 normal   : NORMAL;
    float2 texCoord : TEXCOORD0;
};
/*
//for vs
cbuffer PerObject : register( b0 )
{
    float4x4 ModelViewProjection;
    float4x4 ModelView;
}
*/
//for ps CHECKED
cbuffer Material : register( b2 )
{
    Material Mat;
};


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



//for nothing?
// Parameters required to convert screen space coordinates to view space params.
cbuffer ScreenToViewParams : register( b3 )
{
    float4x4 InverseProjection;
    float2 ScreenDimensions;
}

cbuffer lights : register(b1)
{
    int numLights;
	int foo;
	int bar;
	int foobar;

}



Texture2D AmbientTexture        : register( t0 );
Texture2D EmissiveTexture       : register( t1 );
Texture2D DiffuseTexture        : register( t2 );
Texture2D SpecularTexture       : register( t3 );
Texture2D SpecularPowerTexture  : register( t4 );
Texture2D NormalTexture         : register( t5 );
Texture2D BumpTexture           : register( t6 );
Texture2D OpacityTexture        : register( t7 );

StructuredBuffer<Light> Lights : register( t8 );

sampler LinearRepeatSampler     : register( s0 );
sampler LinearClampSampler      : register( s1 );

struct VertexShaderOutput
{
    float3 positionVS   : TEXCOORD0;    // View space position.
    float2 texCoord     : TEXCOORD1;    // Texture coordinate
    float3 tangentVS    : TANGENT;      // View space tangent.
    float3 binormalVS   : BINORMAL;     // View space binormal.
    float3 normalVS     : NORMAL;       // View space normal.
    float4 position     : SV_POSITION;  // Clip space position.
};

// Convert clip space coordinates to view space
float4 ClipToView( float4 clip )
{
    // View space position.
    float4 view = mul( InverseProjection, clip );
    // Perspecitive projection.
    view = view / view.w;

    return view;
}

// Convert screen space coordinates to view space.
float4 ScreenToView( float4 screen )
{
    // Convert to normalized texture coordinates
    float2 texCoord = screen.xy / ScreenDimensions;

    // Convert to clip space
    float4 clip = float4( float2( texCoord.x, 1.0f - texCoord.y ) * 2.0f - 1.0f, screen.z, screen.w );

    return ClipToView( clip );
}

// Compute a plane from 3 noncollinear points that form a triangle.
// This equation assumes a right-handed (counter-clockwise winding order) 
// coordinate system to determine the direction of the plane normal.
Plane ComputePlane( float3 p0, float3 p1, float3 p2 )
{
    Plane plane;

    float3 v0 = p1 - p0;
    float3 v2 = p2 - p0;

    plane.N = normalize( cross( v0, v2 ) );

    // Compute the distance to the origin using p0.
    plane.d = dot( plane.N, p0 );

    return plane;
}

// Check to see if a sphere is fully behind (inside the negative halfspace of) a plane.
// Source: Real-time collision detection, Christer Ericson (2005)
bool SphereInsidePlane( Sphere sphere, Plane plane )
{
    return dot( plane.N, sphere.c ) - plane.d < -sphere.r;
}

// Check to see if a point is fully behind (inside the negative halfspace of) a plane.
bool PointInsidePlane( float3 p, Plane plane )
{
    return dot( plane.N, p ) - plane.d < 0;
}

// Check to see if a cone if fully behind (inside the negative halfspace of) a plane.
// Source: Real-time collision detection, Christer Ericson (2005)
bool ConeInsidePlane( Cone cone, Plane plane )
{
    // Compute the farthest point on the end of the cone to the positive space of the plane.
    float3 m = cross( cross( plane.N, cone.d ), cone.d );
    float3 Q = cone.T + cone.d * cone.h - m * cone.r;

    // The cone is in the negative halfspace of the plane if both
    // the tip of the cone and the farthest point on the end of the cone to the 
    // positive halfspace of the plane are both inside the negative halfspace 
    // of the plane.
    return PointInsidePlane( cone.T, plane ) && PointInsidePlane( Q, plane );
}

// Check to see of a light is partially contained within the frustum.
bool SphereInsideFrustum( Sphere sphere, Frustum frustum, float zNear, float zFar )
{
    bool result = true;

    // First check depth
    // Note: Here, the view vector points in the -Z axis so the 
    // far depth value will be approaching -infinity.
    if ( sphere.c.z - sphere.r > zNear || sphere.c.z + sphere.r < zFar )
    {
        result = false;
    }

    // Then check frustum planes
    for ( int i = 0; i < 4 && result; i++ )
    {
        if ( SphereInsidePlane( sphere, frustum.planes[i] ) )
        {
            result = false;
        }
    }

    return result;
}

bool ConeInsideFrustum( Cone cone, Frustum frustum, float zNear, float zFar )
{
    bool result = true;

    Plane nearPlane = { float3( 0, 0, -1 ), -zNear };
    Plane farPlane = { float3( 0, 0, 1 ), zFar };

    // First check the near and far clipping planes.
    if ( ConeInsidePlane( cone, nearPlane ) || ConeInsidePlane( cone, farPlane ) )
    {
        result = false;
    }

    // Then check frustum planes
    for ( int i = 0; i < 4 && result; i++ )
    {
        if ( ConeInsidePlane( cone, frustum.planes[i] ) )
        {
            result = false;
        }
    }

    return result;
}

float3 ExpandNormal( float3 n )
{
    return n * 2.0f - 1.0f;
}

// This lighting result is returned by the 
// lighting functions for each light type.
struct LightingResult
{
    float4 Diffuse;
    float4 Specular;
};

float4 DoNormalMapping( float3x3 TBN, Texture2D tex, sampler s, float2 uv )
{
    float3 normal = tex.Sample( s, uv ).xyz;
    normal = ExpandNormal( normal );

    // Transform normal from tangent space to view space.
    normal = mul( normal, TBN );
    return normalize( float4( normal, 0 ) );
}

float4 DoBumpMapping( float3x3 TBN, Texture2D tex, sampler s, float2 uv, float bumpScale )
{
    // Sample the heightmap at the current texture coordinate.
    float height_00 = tex.Sample( s, uv ).r * bumpScale;
    // Sample the heightmap in the U texture coordinate direction.
    float height_10 = tex.Sample( s, uv, int2( 1, 0 ) ).r * bumpScale;
    // Sample the heightmap in the V texture coordinate direction.
    float height_01 = tex.Sample( s, uv, int2( 0, 1 ) ).r * bumpScale;

    float3 p_00 = { 0, 0, height_00 };
    float3 p_10 = { 1, 0, height_10 };
    float3 p_01 = { 0, 1, height_01 };

    // normal = tangent x bitangent
    float3 normal = cross( normalize(p_10 - p_00), normalize(p_01 - p_00) );

    // Transform normal from tangent space to view space.
    normal = mul( normal, TBN );

    return float4( normal, 0 );
}

float4 DoDiffuse( Light light, float4 L, float4 N )
{
    float NdotL = max( dot( N, L ), 0 );
    return light.Color * NdotL;
}

float4 DoSpecular( Light light, Material material, float4 V, float4 L, float4 N )
{
    float4 R = normalize( reflect( -L, N ) );
    float RdotV = max( dot( R, V ), 0 );

    return light.Color * pow( RdotV, material.SpecularPower );
}

// Compute the attenuation based on the range of the light.
float DoAttenuation( Light light, float d )
{
    return 1.0f - smoothstep( light.Range * 0.75f, light.Range, d );
}

float DoSpotCone( Light light, float4 L )
{
    // If the cosine angle of the light's direction 
    // vector and the vector from the light source to the point being 
    // shaded is less than minCos, then the spotlight contribution will be 0.
    float minCos = cos( radians( light.SpotlightAngle ) );
    // If the cosine angle of the light's direction vector
    // and the vector from the light source to the point being shaded
    // is greater than maxCos, then the spotlight contribution will be 1.
    float maxCos = lerp( minCos, 1, 0.5f );
    float cosAngle = dot( light.DirectionVS, -L );
    // Blend between the maxixmum and minimum cosine angles.
    return smoothstep( minCos, maxCos, cosAngle );
}

LightingResult DoPointLight( Light light, Material mat, float4 V, float4 P, float4 N )
{
    LightingResult result;

    float4 L = light.PositionVS - P;
    float distance = length( L );
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );

    result.Diffuse = DoDiffuse( light, L, N ) * attenuation * light.Intensity;
    result.Specular = DoSpecular( light, mat, V, L, N ) * attenuation * light.Intensity;

    return result;
}

LightingResult DoDirectionalLight( Light light, Material mat, float4 V, float4 P, float4 N )
{
    LightingResult result;

    float4 L = normalize( -light.DirectionVS );

    result.Diffuse = DoDiffuse( light, L, N ) * light.Intensity;
    result.Specular = DoSpecular( light, mat, V, L, N ) * light.Intensity;

    return result;
}

LightingResult DoSpotLight( Light light, Material mat, float4 V, float4 P, float4 N )
{
    LightingResult result;

    float4 L = light.PositionVS - P;
    float distance = length( L );
    L = L / distance;

    float attenuation = DoAttenuation( light, distance );
    float spotIntensity = DoSpotCone( light, L );

    result.Diffuse = DoDiffuse( light, L, N ) * attenuation * spotIntensity * light.Intensity;
    result.Specular = DoSpecular( light, mat, V, L, N ) * attenuation * spotIntensity * light.Intensity;

    return result;
}

LightingResult DoLighting( StructuredBuffer<Light> lights, Material mat, float4 eyePos, float4 P, float4 N )
{
    float4 V = normalize( eyePos - P );

    LightingResult totalResult = (LightingResult)0;

    for ( int i = 0; i < numLights; ++i )
    {
        LightingResult result = (LightingResult)0;

        // Skip lights that are not enabled.
        if ( !lights[i].Enabled ) continue;
        // Skip point and spot lights that are out of range of the point being shaded.
        if ( lights[i].Type != DIRECTIONAL_LIGHT && length( lights[i].PositionVS - P ) > lights[i].Range ) continue;

        switch ( lights[i].Type )
        {
        case DIRECTIONAL_LIGHT:
        {
            result = DoDirectionalLight( lights[i], mat, V, P, N );
        }
        break;
        case POINT_LIGHT:
        {
            result = DoPointLight( lights[i], mat, V, P, N );
        }
        break;
        case SPOT_LIGHT:
        {
            result = DoSpotLight( lights[i], mat, V, P, N );
        }
        break;
        }
        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    return totalResult;
}










//----------------------------------------------------
/*
VertexShaderOutput VS_main( AppData IN )
{
    VertexShaderOutput OUT;

    OUT.position = mul( ModelViewProjection, float4( IN.position, 1.0f ) );

    OUT.positionVS = mul( ModelView, float4( IN.position, 1.0f ) ).xyz;
    OUT.tangentVS = mul( ( float3x3 )ModelView, IN.tangent );
    OUT.binormalVS = mul( ( float3x3 )ModelView, IN.binormal );
    OUT.normalVS = mul( ( float3x3 )ModelView, IN.normal );

    OUT.texCoord = IN.texCoord;

    return OUT;
}*/

[earlydepthstencil]
float4 main( VertexShaderOutput IN ) : SV_TARGET
{
    // Everything is in view space.
    float4 eyePos = { 0, 0, 0, 1 };
    Material mat = Mat;

    float4 diffuse = mat.DiffuseColor;
    if ( mat.HasDiffuseTexture )
    {
        float4 diffuseTex = DiffuseTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( diffuse.rgb ) )
        {
            diffuse *= diffuseTex;
        }
        else
        {
            diffuse = diffuseTex;
        }
    }

    // By default, use the alpha from the diffuse component.
    float alpha = diffuse.a;
    if ( mat.HasOpacityTexture )
    {
        // If the material has an opacity texture, use that to override the diffuse alpha.
        alpha = OpacityTexture.Sample( LinearRepeatSampler, IN.texCoord ).r;
    }

    float4 ambient = mat.AmbientColor;
    if ( mat.HasAmbientTexture )
    {
        float4 ambientTex = AmbientTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( ambient.rgb ) )
        {
            ambient *= ambientTex;
        }
        else
        {
            ambient = ambientTex;
        }
    }
    // Combine the global ambient term.
	float4 tmAmbient = float4( ambientColor,1);
    ambient *= tmAmbient;

    float4 emissive = mat.EmissiveColor;
    if ( mat.HasEmissiveTexture )
    {
        float4 emissiveTex = EmissiveTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( emissive.rgb ) )
        {
            emissive *= emissiveTex;
        }
        else
        {
            emissive = emissiveTex;
        }
    }

    if ( mat.HasSpecularPowerTexture )
    {
        mat.SpecularPower = SpecularPowerTexture.Sample( LinearRepeatSampler, IN.texCoord ).r * mat.SpecularScale;
    }

    float4 N;

    // Normal mapping
    if ( mat.HasNormalTexture )
    {
        // For scense with normal mapping, I don't have to invert the binormal.
        float3x3 TBN = float3x3( normalize( IN.tangentVS ),
                                 normalize( IN.binormalVS ),
                                 normalize( IN.normalVS ) );

        N = DoNormalMapping( TBN, NormalTexture, LinearRepeatSampler, IN.texCoord );
    }
    // Bump mapping
    else if ( mat.HasBumpTexture )
    {
        // For most scenes using bump mapping, I have to invert the binormal.
        float3x3 TBN = float3x3( normalize( IN.tangentVS ),
                                 normalize( -IN.binormalVS ), 
                                 normalize( IN.normalVS ) );

        N = DoBumpMapping( TBN, BumpTexture, LinearRepeatSampler, IN.texCoord, mat.BumpIntensity );
    }
    // Just use the normal from the model.
    else
    {
        N = normalize( float4( IN.normalVS, 0 ) );
    }

    float4 P = float4( IN.positionVS, 1 );

    LightingResult lit = DoLighting( Lights, mat, eyePos, P, N );

    diffuse *= float4( lit.Diffuse.rgb, 1.0f ); // Discard the alpha value from the lighting calculations.

    float4 specular = 0;
    if ( mat.SpecularPower > 1.0f ) // If specular power is too low, don't use it.
    {
        specular = mat.SpecularColor;
        if ( mat.HasSpecularTexture )
        {
            float4 specularTex = SpecularTexture.Sample( LinearRepeatSampler, IN.texCoord );
            if ( any( specular.rgb ) )
            {
                specular *= specularTex;
            }
            else
            {
                specular = specularTex;
            }
        }
        specular *= lit.Specular;
    }

    return float4( ( ambient + emissive + diffuse + specular ).rgb, alpha * mat.Opacity );

}

// Pixel shader for rendering lights (debug) for forward renderer.
float4 PS_light( VertexShaderOutput IN ) : SV_TARGET
{
    float4 N = normalize( float4( IN.normalVS, 0 ) );

    return float4( ( Mat.DiffuseColor * saturate(N.z) ).rgb, Mat.Opacity );
}

// Used for rendering unlit materials.
float4 PS_unlit( VertexShaderOutput IN ) : SV_Target
{
    float4 diffuse = Mat.DiffuseColor;
    if ( Mat.HasDiffuseTexture )
    {
        float4 diffuseTex = DiffuseTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( diffuse.rgb ) )
        {
            diffuse *= diffuseTex;
        }
        else
        {
            diffuse = diffuseTex;
        }
    }

    // By default, use the alpha from the diffuse component.
    float alpha = diffuse.a;
    if ( Mat.HasOpacityTexture )
    {
        // If the material has an opacity texture, use that to override the diffuse alpha.
        alpha = OpacityTexture.Sample( LinearRepeatSampler, IN.texCoord ).a;
    }

    if ( alpha * Mat.Opacity < Mat.AlphaThreshold )
    {
        discard;
    }

    float4 ambient = Mat.AmbientColor;
    if ( Mat.HasAmbientTexture )
    {
        float4 ambientTex = AmbientTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( ambient.rgb ) )
        {
            ambient *= ambientTex;
        }
        else
        {
            ambient = ambientTex;
        }
    }
    // Combine the global ambient term.
    //ambient *= Mat.GlobalAmbient;

    float4 emissive = Mat.EmissiveColor;
    if ( Mat.HasEmissiveTexture )
    {
        float4 emissiveTex = EmissiveTexture.Sample( LinearRepeatSampler, IN.texCoord );
        if ( any( emissive.rgb ) )
        {
            emissive *= emissiveTex;
        }
        else
        {
            emissive = emissiveTex;
        }
    }

    // Do view space lighting based on normal.
    float4 N = normalize( float4( IN.normalVS, 0 ) );

    return float4( ( ambient + emissive + ( diffuse * N.z ) ).rgb, alpha * Mat.Opacity );

}