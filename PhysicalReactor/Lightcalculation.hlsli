#include"BRDF.hlsli"


///https://seblagarde.files.wordpress.com/2015/07/course_notes_moving_frostbite_to_pbr_v32.pdf

///Punctual lights

float smoothDistanceAtt(float squareDistance,float invSqrAttRadius)
{
    float factor = squareDistance * invSqrAttRadius;
    float smoothFactor = saturate(1.0f-factor*factor);
    return smoothFactor * smoothFactor;
}

float getDistanceAtt(float3 unormalizedLightVector,float invSqrAttRadius)
{
    float sqrDist = dot(unormalizedLightVector, unormalizedLightVector);
    float attenuation = 1.0 / (max(sqrDist, 0.01 * 0.01));
    attenuation *= smoothDistanceAtt(sqrDist, invSqrAttRadius);
    return attenuation;
}

float getAngleAtt(float3 normalizedLightVector,float3 lightDir,float lightAngleScale,float lightAngleOffset)
{
    float cd = dot(lightDir, normalizedLightVector);
    float attenuation = saturate(cd * lightAngleScale + lightAngleOffset);
    attenuation *= attenuation;
    return attenuation;
}

float3 CalculationDirectionalLightSpecular(DirectionalLight dl, float3 V, float roughness, float NdotV,float3 F0, float3 N)
{
    float3 R = reflect(-V, N);
    float3 D = -dl.direction;
    float sunAngularRadius = 0.017f;
    float r = sin(sunAngularRadius);
    float d = cos(sunAngularRadius);
  
    float DdotR = dot(D,R);
    float3 S = R - DdotR * D;
    float3 L = DdotR < d ? normalize(d * D + normalize(S) * r) : R;

    float NdotL = saturate(dot(N, L));
    
    float3 H = normalize(V + L);
    float LdotH = saturate(dot(L, H));
    float NdotH =clamp(dot(N, H), 0.0, 0.99999995);


    float3 fresnel = Fresnel_Schlick1(F0, LdotH);
    float geo = gaSchlickGGX_IBL(NdotL, NdotV, roughness*roughness);
    float ndf = GGX_NDF1(NdotH, roughness);
    float3 specularBRDF1 = fresnel * ndf * geo / PI;
    return specularBRDF1;

}

float3 CalculationDirectionalLightDiffuse(DirectionalLight dl, float3 V, float3 f0, float NdotV, float3 N,float metalness)
{
   
   
    float3 D = -dl.direction;
    float NdotD = saturate(dot(N, D));
    float3 H = normalize(V + D);
    float LdotH = saturate(dot(D, H));

    float3 fresnel = Fresnel_Schlick1(f0, LdotH); 
   
    float3 kd=lerp(float3(1, 1, 1) - fresnel, float3(0, 0, 0), metalness);
    return fresnel;
}


float3 CalculationSpotlight(SpotLight spotlight, float3 WorldPos,float3 N,float3 V,float NdotV,float3 F0,float roughness,float3 diffusecolor,float metalness)
{
    float3 unnormalizedLightVector = spotlight.position - WorldPos;
    float3 L = normalize(unnormalizedLightVector);

   
    const float3 H = normalize(V + L);
    const float LdotH = saturate(dot(L, H));
    const float NdotH = saturate(dot(N, H));
    const float NdotL = saturate(dot(N, L));

    float3 fresnel = Fresnel_Schlick1(F0, LdotH);
    float geo = gaSchlickGGX_IBL(NdotL, NdotV, roughness*roughness);
    float ndf = GGX_NDF1(NdotH, roughness * roughness);
    
   
    float3 kd = lerp(float3(1, 1, 1) - fresnel, float3(0, 0, 0), metalness);

    float3 BRDF = (fresnel * geo * ndf / PI) + LambertDiffuse1(diffusecolor)*kd;

    float att = 1;
    float lightinvsqrattradius = 1.0f / (spotlight.attenuationradius * spotlight.attenuationradius);
    att *= getDistanceAtt(unnormalizedLightVector, lightinvsqrattradius);
    att *= getAngleAtt(L, spotlight.rotation, spotlight.lightAngleScale, spotlight.lightAngleOffset);

    


    float3 luminace = (BRDF * spotlight.colorAndIntensity.w * att * NdotL) * spotlight.colorAndIntensity.xyz / (PI);
    return luminace;

}


float3 CalculationPointlight(PointLight pointlight, float3 WorldPos, float3 N, float3 V, float NdotV,float3 F0, float roughness, float3 diffusecolor, float metalness)
{
    float3 unnormalizedLightVector = pointlight.position - WorldPos;
    float3 L = normalize(unnormalizedLightVector);

  
    const float3 H = normalize(V + L);
    const float LdotH = saturate(dot(L, H));
    const float NdotH = saturate(dot(N, H));
    const float NdotL = saturate(dot(N, L));

    float3 fresnel = Fresnel_Schlick1(F0, LdotH);
    float geo = gaSchlickGGX_IBL(NdotL, NdotV, roughness);
    float ndf = GGX_NDF1(NdotH, roughness);

    float3 kd = lerp(float3(1, 1, 1) - fresnel, float3(0, 0, 0), metalness);
    float3 BRDF = (fresnel * geo * ndf / PI) + LambertDiffuse1(diffusecolor) * kd;

    float att = 1;
    float lightinvsqrattradius = 1.0f / (pointlight.attenuationraduis * pointlight.attenuationraduis);
    att *= getDistanceAtt(unnormalizedLightVector, lightinvsqrattradius);
    att *= getAngleAtt(L, pointlight.rotation, 1, 0);
    float3 luminace = (BRDF * pointlight.Intensity * att * NdotL) * pointlight.color.xyz / (4 * PI);
    return luminace;
}