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

    // Return actual pixel value for "bright" pixels.
    if (uv.x >= 0.8 || uv.y >= 0.8 || uv.z >= 0.8) {
        return uv;
    }

    // Return black for any "dark" pixels.
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}
