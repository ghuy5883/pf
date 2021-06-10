#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
};

struct VOut
{
    float4 positionView : SV_POSITION;
    float4 positionWorld : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent: TANGENT0;
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
    // Normalize the tangent
    output.tangent = normalize(mul(vIn.tangent, (float3x3)c_modelToWorld));

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    // Texturing -------------------------------------------------------
    float4 uv = DiffuseTexture.Sample(DefaultSampler, pIn.uv);
    float4 normalmaptex = NormalMapTexture.Sample(DefaultSampler, pIn.uv);

    // Unbias the normal map
    float3 normalmap = (normalmaptex.xyz * 2) - float3(1.0f, 1.0f, 1.0f);

    // Generate the binormal
    float3 binormal = normalize(cross(pIn.normal, pIn.tangent));

    // Create the TBN matrix
    float3x3 TBN = float3x3(pIn.tangent.x, pIn.tangent.y, pIn.tangent.z,
        binormal.x, binormal.y, binormal.z,
        pIn.normal.x, pIn.normal.y, pIn.normal.z);

    // Transform the sampled normal from tangent space into world space
    pIn.normal = normalize(mul(normalmap, TBN));

    // Lighting ---------------------------------------------------------
    float3 ambient = c_ambient;
    float3 diffuseColor;
    float3 directionToLight;
    float3 specularColor;
    float3 reflectionForLight;
    float3 directionToViewer;
    float specularPower;
    float3 lightPosition;
    float3 lightingSum = { 0.0f, 0.0f, 0.0f };

    for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
        if (c_pointLight[i].isEnabled == true) {
            diffuseColor = c_pointLight[i].diffuseColor;
            specularColor = c_pointLight[i].specularColor;
            lightPosition = c_pointLight[i].position;
            directionToLight = normalize(lightPosition - pIn.positionWorld.xyz);
            reflectionForLight = normalize(reflect(-directionToLight, pIn.normal));
            directionToViewer = normalize(c_cameraPosition - pIn.positionWorld.xyz);
            specularPower = c_pointLight[i].specularPower;

            float d = dot(pIn.normal, directionToLight);
            d = max(0.0f, d);
            float3 diffuse = diffuseColor * d;

            float s = dot(reflectionForLight, directionToViewer);
            s = max(0.0f, s);
            float3 specular = specularColor * pow(s, specularPower);

            float dist = distance(pIn.positionWorld.xyz, lightPosition);
            float colorFallOff = smoothstep(c_pointLight[i].outerRadius, c_pointLight[i].innerRadius, dist);
            specular *= colorFallOff;
            diffuse *= colorFallOff;

            lightingSum += diffuse + specular;
        }
    }

    float4 finalLighting = float4(ambient + lightingSum, 1.0f);

    // Return final value -----------------------------------------------
    return uv * finalLighting;
}
