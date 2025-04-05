cbuffer TransformBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    // Perform the transformations
    float4 worldPosition = mul(float4(input.Position.xyz, 1.0f), model);
    float4 viewPosition = mul(worldPosition, view);
    output.Position = mul(viewPosition, projection);
    
    // Pass the input normal to the output
    output.Normal = input.Normal;
    
    // Pass the texture coordinates to the output
    output.TexCoord = input.TexCoord;
    
    return output;
}
