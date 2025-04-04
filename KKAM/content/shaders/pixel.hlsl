Texture2D texture0 : register(t0); // Texture resource
SamplerState sampler0 : register(s0); // Sampler state

struct PSInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD; // Adding texture coordinates input
};

float4 main(PSInput input) : SV_TARGET
{
    // Sample the texture
    float4 texColor = texture0.Sample(sampler0, input.TexCoord);
    
    // Combine the texture color with the vertex color
    return texColor * input.Color;
}
