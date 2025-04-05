Texture2D texture0 : register(t0); // Texture resource
SamplerState sampler0 : register(s0); // Sampler state

struct PSInput
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

float4 main(PSInput input) : SV_TARGET
{
    float4 texColor = texture0.Sample(sampler0, input.TexCoord);
    return texColor;
}
