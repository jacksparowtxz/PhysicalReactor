#include"BRDF.hlsli"


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
    coffies cosf[9];
    float3 BaseColorFactor;
    float emissive_factor;
    float metalic_factor;
    float rouhgness_factor;
    float padingf;
    float padingf1;
};



struct PixelShaderInput
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float3 TangentW : TANGENT;
    float2 Tex : TEXCOORD;
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
Texture2D specularBRDF_LUT : register(t16);


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
    float3 basecolor = SRGBtoLINEAR(BaseColorMap.Sample(BaseColorSampler, input.Tex)).rgb * BaseColorFactor;

    

    float metalness = MetalicMap.Sample(MetalicSampler, input.Tex).b* metalic_factor;
    metalness = clamp(metalness,0.0,1.0);


    float roughness = RoughnessMap.Sample(MetalicSampler, input.Tex).g * rouhgness_factor;
    roughness = clamp(roughness,0.04,1.0);
    
    float3 diffusecolor = basecolor.rgb * (1.0 - Fdielectirc);

    diffusecolor *= 1.0 - metalness;

    float3 specularColor = lerp(Fdielectirc,basecolor,metalness);
    

    float3 reflectance = max(max(specularColor.r, specularColor.g), specularColor.b);

    float reflectance90 = clamp(reflectance*25.0,0.0,1.0);
    float3 specularEnvironmentR0 = specularColor.rgb;
    float3 specularEnvironmentR90 = float3(1.0, 1.0, 1.0) * reflectance90;

    float3 v = normalize(EyePos.xyz - input.PosW);

    float3 n1 = NormalMap.Sample(NormalSampler, input.Tex).rgb;
    input.NormalW = normalize(input.NormalW);
    float3 n = NormalSampleToWorldSpace(n1, input.NormalW, input.TangentW);

    float ambient = AmbientMap.Sample(AmbientSampler, input.Tex).r;
    
    PBRInfo pbrInputs;

    float3 directLighting = 0;
  
           //Direct Light///Temp one directionlight
        float3 l = normalize(directionalights.direction);
        float3 h = normalize(l + v);
        float3 reflection = -normalize(reflect(v, n));

        float NdotL = clamp(dot(n, l), 0.001, 1.0);
        float NdotV = abs(dot(n, v)) + 0.001;
        float NdotH = clamp(dot(n, h), 0.0, 1.0);
        float LdotH = clamp(dot(l, h), 0.0, 1.0);
        float VdotH = clamp(dot(v, h), 0.0, 1.0);

        pbrInputs.NdotL = NdotL;
        pbrInputs.NdotV = NdotV;
        pbrInputs.NdotH = NdotH;
        pbrInputs.LdotH = LdotH;
        pbrInputs.VdotH = VdotH;
        pbrInputs.perceptualRoughness = roughness;
        pbrInputs.metalness = metalness;
        pbrInputs.reflectance0 = specularEnvironmentR0;
        pbrInputs.reflectance90 = specularEnvironmentR90;
        pbrInputs.alphaRoughness = roughness * roughness;
        pbrInputs.diffuseColor = diffusecolor;
        pbrInputs.specularColor = specularColor;

        float3 Lradiance = directionalights.color.rgb;
        float Intensity = directionalights.Intensity;

        float3 F = Fresnel_Schlick(pbrInputs);

        float D = GGX_NDF(pbrInputs);

        float G = GGX_Schilck(pbrInputs);


        float3 diffuseContrib = (1.0 - F) * LambertDiffuse(pbrInputs);

        float3 specContrib = F * G * D / (4.0 * NdotL * NdotV);
        float3 color = NdotL * Lradiance * Intensity * (diffuseContrib + specContrib);
    


    float3 irradiance = spherical_harmonics_Irrandice(cosf, n);

    float3 diffuseIBL = pbrInputs.diffuseColor* irradiance;


    /////Return IBL Env map;
    uint width, height, specularTextureLevels;
    specularTexture.GetDimensions(0, width, height, specularTextureLevels);
   

    float3 specularIrradiance = SRGBtoLINEAR(specularTexture.SampleLevel(BaseColorSampler, reflection, roughness * specularTextureLevels)).rgb;


    float2 val = float2(pbrInputs.NdotV, 1.0 - pbrInputs.perceptualRoughness);

    float2 IspecularBRDF = SRGBtoLINEAR(specularBRDF_LUT.Sample(spBRDF_Sampler, val)).rg;



    float3 specularIBL = (pbrInputs.specularColor * IspecularBRDF.x + IspecularBRDF.y) * specularIrradiance;

    float3 IndirectLighting = (diffuseIBL + specularIBL);

    float3 totallighting = (IndirectLighting + directLighting) ;


    float3 totallightingWithAo = lerp(totallighting, totallighting * ambient, 1.0f);

    float3 emissive = SRGBtoLINEAR(EmissiveMap.Sample(BaseColorSampler, input.Tex)).rgb * emissive_factor;
  
    totallightingWithAo += emissive;
 
      
    return float4(totallightingWithAo, 1.0f);
}
