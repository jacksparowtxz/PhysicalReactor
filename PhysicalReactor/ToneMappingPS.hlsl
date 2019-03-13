Texture2D sceneColor : register(t0);
SamplerState defaultSampler : register(s0);
static const float gamma = 2.2;
static const float exposure = 1.0;
static const float pureWhite = 1.0;
struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};


float4 main(PixelShaderInput pin):SV_Target
{
    float3 color = sceneColor.Sample(defaultSampler, pin.texcoord).rgb * exposure;

    float luminace = dot(color, float3(0.2126, 0.7152, 0.0722));
    float mappedLuminace = (luminace * (1.0 + luminace / (pureWhite * pureWhite))) / (1 + luminace);

    float3 mappedColor = (mappedLuminace / luminace) * color;

    return float4(pow(mappedColor, 1.0 / gamma), 1.0);

}