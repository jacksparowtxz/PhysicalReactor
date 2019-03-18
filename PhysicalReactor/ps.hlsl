#include"BRDF.hlsli"
#include"Mathematics.hlsli"

cbuffer DirectionalLightCB : register(b0)
{
    DirectionalLight directionalights;
};


cbuffer PointLightCB : register(b1)
{
    PointLight pointlights[MAX_LIGHTS];
};

cbuffer SpotLightCB : register(b2)
{
    SpotLight spotlights[MAX_LIGHTS];
};

cbuffer EyePostionCB : register(b3)
{
    float4 EyePos;
    coffies cosf[15];
};



struct PixelShaderInput
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
    float3x3 TBN : TBASIS;
};

static const float3 Fdielectirc = 0.04;
static const float epsilon = 0.00001;
 
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

TextureCube specularTexture : register(t15);
Texture2D specularBRDF_LUT : register(t17);


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

SamplerState spBRDF_Sampler : register(s15);
SamplerState Lut_Sampler : register(s16);

// (内插)颜色数据的传递函数。
float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 basecolor = BaseColorMap.Sample(BaseColorSampler, input.Tex);
    float metalness = MetalicMap.Sample(BaseColorSampler,input.Tex).r;
    float roughness = RoughnessMap.Sample(BaseColorSampler,input.Tex).r;

    float3 Lo = normalize(EyePos.xyz - input.PosW);

    float3 N = normalize(2.0 * NormalMap.Sample(BaseColorSampler,input.Tex).rgb-1.0);
    N = normalize(mul(input.TBN, N));

    float cosLo = max(0.0, dot(N, Lo));

    float Lr = 2.0 * cosLo * N - Lo;

    float3 F0 = lerp(Fdielectirc,basecolor,metalness);


    //Direct Light///Temp one directionlight
    float3 Li = -directionalights.direction;
    float3 Lradiance = directionalights.color.rgb;
    float Intensity = directionalights.Intensity;

    float3 Lh = normalize(Li+Lo);

    float cosLi = max(0.0, dot(N, Li));
    float cosLh = max(0.0, dot(N, Lh));

    float F = Fresnel_Schlick(F0, Li, Lh);

    float D = GGX_NDF(roughness,cosLh);

    float G = GGX_Schilck(roughness,Li,Lh,N);

    float kd = lerp(float3(1, 1, 1) - F, float3(0,0,0),metalness);

    float diffuseBRDF = kd * basecolor;

    float3 specularBRDF = (F * D * G) / max(epsilon,4.0*cosLi*cosLo);

    float3 directLighting = (diffuseBRDF + specularBRDF) * Lradiance * cosLi * Intensity;


    float3 irradiance = spherical_harmonics_Irrandice(cosf, N);

    float3 IF = Fresnel_Schlick(F0,Lo,N);

    float3 Ikd = lerp(1.0-IF,0.0,metalness);

    float3 diffuseIBL = Ikd * basecolor * irradiance;

    /////Return IBL Env map;
    uint width, height, levels;
    uint specularTextureLevels=SpecularMap.GetDimensions(0,width,height,levels);
    float3 specularIrradiance = SpecularMap.SampleLevel(BaseColorSampler,Lr,roughness*specularTextureLevels).rgb;

    float2 IspecularBRDF = specularBRDF_LUT.Sample(spBRDF_Sampler, float2(cosLo,roughness)).rg;

    float3 specularIBL = (F0 * IspecularBRDF.x + IspecularBRDF.y) * specularIrradiance;

    float3 ambientLighting = diffuseIBL + specularIBL;

    return float4(ambientLighting,1.0f);
}
