#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VOut
{
    float4 positionView : SV_POSITION;
    float4 positionWorld : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

VOut VS(VIn vIn)
{
    VOut output;

    // Transform input position from model to world space.
    output.positionWorld = mul(float4(vIn.position, 1.0f), c_modelToWorld);
    // Transform position from world to projection space.
    output.positionView = mul(output.positionWorld, c_viewProj);
    output.uv = vIn.uv;
    // Transform normal from model to world space.
    output.normal = normalize(mul(vIn.normal, (float3x3)c_modelToWorld));

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    // Texture
    float4 uv = DiffuseTexture.Sample(DefaultSampler, pIn.uv);

    return uv;
}
