cbuffer TransformBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

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
    
    // Transform the normal vector
    output.Normal = mul(input.Normal, (float3x3) model);
    
    // Pass the texture coordinates to the output
    output.TexCoord = input.TexCoord;
    
    return output;
}
