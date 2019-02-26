

struct PixelShaderInput
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;

};


TextureCube SkyCubemap : register(t4);

SamplerState samTriLinearSam : register(s0);


float4 main(PixelShaderInput PSInput) : SV_TARGET
{

    return SkyCubemap.Sample(samTriLinearSam,PSInput.PosL);

}
