#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct VOut
{
    float4 positionView : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VOut VS(VIn vIn)
{
    VOut output;
    output.positionView = float4(vIn.position, 1.0f);
    output.uv = vIn.uv;

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    // Return final value -----------------------------------------------
    float4 uv = DiffuseTexture.Sample(DefaultSampler, pIn.uv);
    return uv;
}
