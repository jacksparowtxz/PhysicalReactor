// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
};

Texture2D BaseColorMap : register(t0);
Texture2D MetalicMap : register(t1);
Texture2D SpecularMap : register(t2);
Texture2D RoughnessMap : register(t3);
Texture2D EmissiveMap : register(t4);
Texture2D OpacityMap : register(t5);
Texture2D OpcaityMaskMap : register(t6);
Texture2D NormalMap : register(t7);
Texture2D WorldPositionOffset : register(t8);
Texture2D WorldDisplacement : register(t9);
Texture2D TessellationMultiplerMap : register(t10);
Texture2D SubsurfaceMap : register(t11);
Texture2D AmbientMap : register(t12);
Texture2D RefractionMap : register(t13);
Texture2D PixelDepthOffset : register(t14);

SamplerState BaseColorSampler : register(s0);
SamplerState MetalicSampler : register(s1);
SamplerState SpecularSampler : register(s2);
SamplerState RoughnessSampler : register(s3);
SamplerState EmissiveSampler : register(s4);
SamplerState OpacitySampler : register(s5);
SamplerState OpcaityMaskSampler : register(s6);
SamplerState NormalSampler : register(s7);
SamplerState WorldPositionOffsetSampler : register(s8);
SamplerState WorldDisplacementSampler : register(s9);
SamplerState TessellationMultiplerSampler : register(s10);
SamplerState SubsurfaceSampler : register(s11);
SamplerState AmbientSampler : register(s12);
SamplerState RefractionSampler : register(s13);
SamplerState PixelDepthOffsetSampler : register(s14);



// (内插)颜色数据的传递函数。
float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 basecolor = BaseColorMap.Sample(BaseColorSampler, input.Tex);
	return basecolor;
}
