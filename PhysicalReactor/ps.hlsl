#include"Lightcalculation.hlsli"

cbuffer DirectionalLightCB : register(b0)
{
    DirectionalLight directionalights[4];
};


cbuffer PointLightCB : register(b1)
{
    PointLight pointlights[4];
};

cbuffer SpotLightCB : register(b2)
{
    SpotLight spotlights[4];
};

cbuffer EyePostionCB : register(b3)
{
    float4 EyePos;
    float3 BaseColorFactor;
    int NumOfSpotLights;
    float emissive_factor;
    float metalic_factor;
    float rouhgness_factor;
    int NumOfPointLights;
    int NumOfSphereLights;
    int NumOfDiscLights;
    int NumOfTubeLights;
    int NumOfRectangleLights;

};


cbuffer SphereLightsCB : register(b4)
{
    SphereAreaLight spherearealight[4];
}

cbuffer DiscLightsCB : register(b5)
{
    DiscLight disclight[4];
}

cbuffer TubeLightsCB : register(b6)
{
    TubeLight tubelights[4];
}

cbuffer RectangleLightCB : register(b7)
{
    RectangleLight rectanglelight[2];
}




struct PixelShaderInput
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    //float3 NormalW : NORMAL;
   // float3 TangentW : TANGENT;
    float3x3 tangentBasis : TBASIS;
    float2 Tex : TEXCOORD;
};

static const float3 Fdielectirc = float3(0.04, 0.04, 0.04);
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
TextureCube iradaniceTexture : register(t17);

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

// (内插)颜色数据的传递函数。
float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 albedo = BaseColorMap.Sample(BaseColorSampler, input.Tex).rgb;
    float metalness = MetalicMap.Sample(MetalicSampler, input.Tex).b;
    float roughness = MetalicMap.Sample(MetalicSampler, input.Tex).g;
    metalness = clamp(metalness, 0.0, 1.0);
    roughness = clamp(roughness, 0.04, 1.0);

    float3 V = normalize(EyePos.xyz - input.PosW);

    float3 N = normalize(2.0 * NormalMap.Sample(BaseColorSampler, input.Tex).rgb - 1.0);
    N = normalize(mul(input.tangentBasis, N));

    const float NdotV = abs(dot(N, V)) + 1e-5f;

    float3 specularEnvironmentR0 = lerp(Fdielectirc, albedo, metalness);

    float3 directLighting;
    ///Directional light
    {

        float3 DLdiffuse = CalculationDirectionalLightDiffuse(directionalights[0], V, specularEnvironmentR0, NdotV, N, metalness) * LambertDiffuse1(albedo);
        float3 DLspecular = CalculationDirectionalLightSpecular(directionalights[0], V, roughness, NdotV, specularEnvironmentR0, N);
        float Lradiance = directionalights[0].Intensity;
        float LightColor = directionalights[0].color.xyz;
        float3 L = -directionalights[0].direction;
        float NdotL = max(0.0, dot(N, L));
        directLighting = (DLdiffuse + DLspecular) * Lradiance * LightColor * NdotL;

    }
    //spot light
    {
        for (int i = 0; i < NumOfSpotLights; i++)
        {
            directLighting += CalculationSpotlight(spotlights[i], input.PosW, N, V, NdotV, specularEnvironmentR0, roughness, albedo, metalness);
        }
    }

    //point light
    {
        for (int i = 0; i < NumOfPointLights; i++)
        {
            directLighting += CalculationPointlight(pointlights[i], input.PosW, N, V, NdotV, specularEnvironmentR0, roughness, albedo, metalness);
        }
    }

   {
        for (int i = 0; i < NumOfSphereLights;i++)
        {
            directLighting += CalculationSphereArealight(spherearealight[i], input.PosW, N, V, NdotV, specularEnvironmentR0, roughness, albedo, metalness);
        }
    }

    {
        for (int i = 0; i <NumOfDiscLights;i++)
        {
            directLighting += CalculationDiscLight(disclight[i], input.PosW, N, V, NdotV, specularEnvironmentR0, roughness, albedo, metalness);
        }
    }

    {
        for (int i = 0; i < NumOfTubeLights; i++)
        {
            directLighting += CalculationTubeLight(tubelights[i], input.PosW, N, V, NdotV, specularEnvironmentR0, roughness, albedo, metalness);
        }
    }

    {
        for (int i = 0; i < NumOfRectangleLights; i++)
        {
            directLighting += CalculationRectangleLight(rectanglelight[i], input.PosW, N, V, NdotV, specularEnvironmentR0, roughness, albedo, metalness);
        }
    }
    float ambient = AmbientMap.Sample(AmbientSampler, input.Tex).r;
	// Ambient lighting (IBL).
    float3 ambientLighting;
    float3 test;
	{


		// Sample diffuse irradiance at normal direction.
        float3 irradiance = iradaniceTexture.Sample(BaseColorSampler, N).rgb;

		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
        float3 F = Fresnel_Schlick(specularEnvironmentR0, NdotV, roughness);
        float3 kS = F;
        float3 kd = 1.0 - kS;
        kd *= 1.0 - metalness;
		// Get diffuse contribution factor (as with direct lighting).
        //float3 kd = lerp(1.0 - F, 0.0, metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
        float3 diffuseIBL = kd * albedo * irradiance;

		// Sample pre-filtered specular reflection environment at correct mipmap level., roughness * specularTextureLevels

	  // Specular reflection vector.
        float3 Lr = 2.0 * NdotV * N - V;


        uint specularTextureLevels = querySpecularTextureLevels(specularTexture);
        
        float3 specularIrradiance = specularTexture.SampleLevel(MetalicSampler, Lr, roughness * specularTextureLevels / 2).rgb;
      
		// Split-sum approximation factors for Cook-Torrance specular BRDF.
        float3 specularBRDF = specularBRDF_LUT.Sample(spBRDF_Sampler, float2(NdotV, roughness)).rgb;

		// Total specular IBL contribution.
        float3 specularIBL = (specularEnvironmentR0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
        ambientLighting = diffuseIBL + specularIBL;
      
        test = diffuseIBL;

    }

    
    float3 totallighting = ambientLighting + directLighting;
   
    float3 totallightingWithAo = 0;
    if (ambient > 0)
    {
        totallightingWithAo = lerp(totallighting, totallighting * ambient, 1.0f);
    }
    else
    {
        totallightingWithAo = totallighting;

    }

    float3 emissive = EmissiveMap.Sample(BaseColorSampler, input.Tex).rgb;
  
    totallightingWithAo += emissive;
 

    
    //return float4(totallightingWithAo, 1.0);
    return float4(totallightingWithAo, 1.0);

   /* float3 albedo = BaseColorMap.Sample(BaseColorSampler, input.Tex).rgb;
    float metalness = MetalicMap.Sample(MetalicSampler, input.Tex).b;
    float roughness = MetalicMap.Sample(MetalicSampler, input.Tex).g;
    metalness = clamp(metalness, 0.0, 1.0);
    roughness = clamp(roughness, 0.04, 1.0);
	// Outgoing light direction (vector from world-space fragment position to the "eye").
    float3 Lo = normalize(EyePos.xyz - input.PosW);

	// Get current fragment's normal and transform to world space.
    float3 N = normalize(2.0 * NormalMap.Sample(BaseColorSampler, input.Tex).rgb - 1.0);
    N = normalize(mul(input.tangentBasis, N));
   
	// Angle between surface normal and outgoing light direction.
    float cosLo = max(0.0, dot(N, Lo));
		
	// Specular reflection vector.
    float3 Lr = 2.0 * cosLo * N - Lo;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
    float3 F0 = lerp(Fdielectirc, albedo, metalness);

	// Direct lighting calculation for analytical lights.
    float3 directLighting = 0.0;
 {
   
    float3 Li = -directionalights[0].direction;
    float3 Lradiance = (directionalights[0].Intensity, directionalights[0].Intensity, directionalights[0].Intensity);

		// Half-vector between Li and Lo.
    float3 Lh = normalize(Li + Lo);

		// Calculate angles between surface normal and various light vectors.
    float cosLi = max(0.0, dot(N, Li));
    float cosLh = max(0.0, dot(N, Lh));///OLD VERSION

		// Calculate Fresnel term for direct lighting. 
    float3 F = Fresnel_Schlick1(F0, max(0.0, dot(Lh, Lo)));///OLD VERSION
		// Calculate normal distribution for specular BRDF.
   float D = GGX_NDF1(cosLh, roughness);///OLD VERSION
		// Calculate geometric attenuation for specular BRDF.
   float G = gaSchlickGGX_IBL(cosLi, cosLo, roughness);///OLD VERSION

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
    float3 kd =lerp(float3(1, 1, 1) - F, float3(0, 0, 0), metalness);///OLD VERSION

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
    float3 diffuseBRDF = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
    float3 specularBRDF =(F * D * G) / max(epsilon, 4.0 * cosLi * cosLo);///OLD VERSION

		// Total contribution for this light.
    directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
       

    }
    float ambient = AmbientMap.Sample(AmbientSampler, input.Tex).r;
	// Ambient lighting (IBL).
    float3 ambientLighting;
	{


		// Sample diffuse irradiance at normal direction.
        float3 irradiance = iradaniceTexture.Sample(BaseColorSampler, N).rgb;

		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
        float3 F = Fresnel_Schlick1(F0, cosLo);

		// Get diffuse contribution factor (as with direct lighting).
        float3 kd = lerp(1.0 - F, 0.0, metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
        float3 diffuseIBL = kd * albedo * irradiance;

		// Sample pre-filtered specular reflection environment at correct mipmap level., roughness * specularTextureLevels
   
        //specularTexture.GetDimensions(0, width, height, specularTextureLevels);roughness * specularTextureLevels 
        uint specularTextureLevels=querySpecularTextureLevels(specularTexture);
        
        float3 specularIrradiance = specularTexture.SampleLevel(MetalicSampler, Lr, roughness * specularTextureLevels/2).rgb;
      
		// Split-sum approximation factors for Cook-Torrance specular BRDF.
        float3 specularBRDF = specularBRDF_LUT.Sample(spBRDF_Sampler, float2(cosLo, roughness)).rgb;

		// Total specular IBL contribution.
        float3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
        ambientLighting = diffuseIBL + specularIBL;
      
    
    }

    
    float3 totallighting = (ambientLighting + directLighting);
   
    float3 totallightingWithAo = 0;
    if (ambient > 0)
    {
        totallightingWithAo = lerp(totallighting, totallighting * ambient, 1.0f);
    }
    else
    {
        totallightingWithAo = totallighting;

    }

    float3 emissive = EmissiveMap.Sample(BaseColorSampler, input.Tex).rgb * emissive_factor;
  
    totallightingWithAo += emissive;
 
      
 



	
    return float4(totallightingWithAo, 1.0);*/



   
   


}
