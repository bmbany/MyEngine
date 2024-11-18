struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 worldPosition : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 biTangent : BITANGENT;
    float2 texCoord : TEXCOORD0;
    float2 lightCoord : TEXCOORD1;
};