#include "Constants.hlsl"

struct VIn
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    uint4 bones : BLENDINDICES0;
    float4 weights : BLENDWEIGHT0;
    float2 uv : TEXCOORD0;
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

    // Skinning the vertex position -----------------------
    float4 finalWeightedVertexPos = { 0.0f, 0.0f, 0.0f, 0.0f };
    float3 finalWeightedVertexNormal = { 0.0f, 0.0f, 0.0f };
    // Iterate for up to 4 weight influences
    for (int i = 0; i < 4; i++) {
        float skinningWeight = vIn.weights[i];
        int index;
        if (i == 0) {
            index = vIn.bones.x;
        }
        else if (i == 1) {
            index = vIn.bones.y;
        }
        else if (i == 2) {
            index = vIn.bones.z;
        }
        else {
            index = vIn.bones.w;
        }

        finalWeightedVertexPos += skinningWeight * mul(float4(vIn.position, 1.0f), c_skinMatrix[index]);
        finalWeightedVertexNormal += (skinningWeight * mul(float4(vIn.normal, 0.0f), c_skinMatrix[index])).xyz;
    } 

    // Transform input position from model to world space.
    output.positionWorld = mul(finalWeightedVertexPos, c_modelToWorld);
    // Transform position from world to projection space.
    output.positionView = mul(output.positionWorld, c_viewProj);
    output.uv = vIn.uv;
    // Transform normal from model to world space.
    output.normal = normalize(mul(finalWeightedVertexNormal, (float3x3)c_modelToWorld));

    return output;
}

float4 PS(VOut pIn) : SV_TARGET
{
    // Texture
    float4 uv = DiffuseTexture.Sample(DefaultSampler, pIn.uv);

    // Lighting
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
    return uv * finalLighting;
}
