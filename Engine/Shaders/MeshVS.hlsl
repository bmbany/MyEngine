#include "MeshInfo.hlsli"

cbuffer WVP : register(b0)
{
    matrix World;
    matrix View;
    matrix Proj;
    matrix WVP;
}

cbuffer MatrixPallete : register(b1)
{
    matrix matrixPallete[128];
}

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT0;
    float3 biTangent : BITANGENT;
    float2 texCoord : TEXCOORD0;
    float2 lightCoord : TEXCOORD1;
    uint4 blendIndices : BLENDINDICES0;
    float4 blendWeights : BLENDWEIGHT0;
};

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    matrix worldTransform;
    
#ifdef Skinned
    matrix boneTransform = mul(input.blendWeights.x, matrixPallete[input.blendIndices.x]);
    boneTransform += mul(input.blendWeights.y, matrixPallete[input.blendIndices.y]);
    boneTransform += mul(input.blendWeights.z, matrixPallete[input.blendIndices.z]);
    boneTransform += mul(input.blendWeights.w, matrixPallete[input.blendIndices.w]);
    worldTransform = mul(boneTransform, World);
    
    output.position = mul(input.position, boneTransform);
    output.position = mul(output.position, WVP);
#else
    worldTransform = World;
    output.position = mul(input.position, WVP);
#endif
    
    output.worldPosition = mul(input.position, worldTransform);
    output.normal = normalize(mul(input.normal, (float3x3) worldTransform));
    //output.normal = mul(input.normal, (float3x3) worldTransform);
    //output.normal = normalize(mul(output.normal, (float3x3) View));
    output.tangent = normalize(mul(input.tangent, (float3x3) worldTransform));
    output.biTangent = normalize(mul(input.biTangent, (float3x3) worldTransform));
    
    output.texCoord = input.texCoord;
    output.lightCoord = input.lightCoord;
    
    return output;
}