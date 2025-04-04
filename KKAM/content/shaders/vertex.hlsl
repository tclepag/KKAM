cbuffer TransformBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

struct VSInput
{
    float3 Position : POSITION;
    float3 Color : COLOR; // Adding color input for the vertex
    float2 TexCoord : TEXCOORD; // Adding texture coordinates input
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR; // Adding color output for the pixel shader
    float2 TexCoord : TEXCOORD; // Adding texture coordinates output for the pixel shader
};

VSOutput main(VSInput input)
{
    VSOutput output;
    
    // Perform the transformations
    float4 worldPosition = mul(float4(input.Position.xyz, 1.0f), model);
    float4 viewPosition = mul(worldPosition, view);
    output.Position = mul(viewPosition, projection);
    
    // Pass the input color to the output
    output.Color = float4(input.Color, 1.0f);
    
    // Pass the texture coordinates to the output
    output.TexCoord = input.TexCoord;
    
    return output;
}
