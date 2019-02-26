

struct PixelShaderInput
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;

};


TextureCube SkyCubemap : register(t0);

SamplerState samTriLinearSam : register(s0);


float4 main(PixelShaderInput PSInput):SV_Target
{

    return SkyCubemap.Sample(samTriLinearSam,PSInput.PosL);

}
