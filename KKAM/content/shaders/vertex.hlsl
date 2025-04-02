cbuffer TransformBuffer : register(b0)
{
    matrix model;
    matrix view;
    matrix projection;
};

struct VSInput
{
    float3 Position : POSITION;
};

struct VSOutput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR; // Adding color output for the pixel shader
};

VSOutput main(VSInput input)
{
    VSOutput output;
    float4 worldPosition = mul(float4(input.Position, 1.0f), model);
    float4 viewPosition = mul(worldPosition, view);
    output.Position = mul(viewPosition, projection);
    
    // Generate some simple color based on position
    output.Color = float4(input.Position.x + 0.5f, input.Position.y + 0.5f, 0.5f, 1.0f);
    
    return output;
}