#include "MeshInfo.hlsli"
#include "ShaderUtilitys.hlsli"

// Light Type
#define NONE 0
#define DIRECTIONAL 1
#define POINT 2
#define SPOT 3

Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
Texture2D txSpecular : register(t2);
Texture2D txEmissive : register(t3);
Texture2D txOpacity : register(t4);
Texture2D txLight : register(t5);

SamplerState samLinear : register(s0);

cbuffer LightDesc : register(b0)
{
    float3 lightDirection;
    float light_padding;
    float4 lightAmibent;
    float4 lightDiffuse;
    float4 lightSpecular;
}

cbuffer CameraDesc : register(b1)
{
    float3 cameraLook;
    float cam_padding1;
    float3 cameraPosition;
    float cam_padding2;
}

float4 main(PS_INPUT input) : SV_Target
{    
    float3 L = normalize(lightDirection);
    float3 V = normalize(cameraPosition - input.worldPosition);
    float3 H = normalize(-L + V);
    
    float3 normalTex = txNormal.Sample(samLinear, input.texCoord);
    float3x3 TBN = float3x3(input.tangent, input.biTangent, input.normal);
    float3 N = normalize(input.normal);
    
    // 노말맵이 있으면 사용
    if (0.f < length(normalTex))
        N = normalize(mul(normalTex * 2.0f - 1.0f, TBN));
        
    float HDotN = max(0.f, dot(H, N));

    //float2 uv = N.xy * 0.5f + 0.5f;
    //float4 diffuse = txDiffuse.Sample(samLinear, uv);
    float4 diffuse = float4(GammaToLinearSpace(txDiffuse.Sample(samLinear, input.texCoord).rgb), 1.f);
    diffuse = ComputeDirectionalLight(N, L) * lightDiffuse * diffuse;
    float4 specular = pow(HDotN, 128.f) * lightSpecular * txSpecular.Sample(samLinear, input.texCoord);
    float4 ambient = lightAmibent;
    
    float4 finalColor = diffuse + specular + ambient;
    
//#ifdef Emission
    float4 emissive = txEmissive.Sample(samLinear, input.texCoord);
    finalColor += emissive;
//#endif

//#ifdef Opacity
    float4 opacity = txOpacity.Sample(samLinear, input.texCoord);
    finalColor += opacity;
//#endif
    
    finalColor = float4(LinearToGammaSpace(finalColor.rgb), 1.f);   
        
    return finalColor;
}
