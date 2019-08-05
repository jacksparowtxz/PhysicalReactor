#include"Common.hlsli"


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

float CalculationDirectionalLight(DirectionalLight dl,float3 WorldPos,float3 BRDF,float ndotl,float3 camerapos,float3 N)
{
    float3 V = normalize(camerapos - WorldPos);
    float3 R = reflect(-V, N);
    float3 D = dl.direction;
    float sunAngularRadius = 0.017f;
    float r = sin(sunAngularRadius);
    float d = cos(sunAngularRadius);
  
    float DdotR = dot(D,R);
    float3 S = R - DdotR * D;
    float3 L = DdotR < d ? normalize(d * D + normalize(S) * r) : R;



}

float3 CalculationSpotlight(SpotLight spotlight, float3 WorldPos, float3 BRDF,float ndotl)
{
    float3 unnormalizedLightVector = spotlight.position - WorldPos;
    float3 L = normalize(unnormalizedLightVector);
    float att = 1;
    float lightinvsqrattradius = 1.0f / (spotlight.attenuationradius * spotlight.attenuationradius);
    att *= getDistanceAtt(unnormalizedLightVector, lightinvsqrattradius);
    att *= getAngleAtt(L, spotlight.rotation, spotlight.pad, spotlight.pad1);
    float3 luminace = (BRDF * spotlight.Intensity * att * ndotl) * spotlight.color.xyz/(PI);


}


float3 CalculationPointlight(PointLight pointlight, float3 WorldPos, float3 BRDF, float ndotl)
{
    float3 unnormalizedLightVector = pointlight.position - WorldPos;
    float3 L = normalize(unnormalizedLightVector);
    float att = 1;
    float lightinvsqrattradius = 1.0f / (pointlight.attenuationraduis * pointlight.attenuationraduis);
    att *= getDistanceAtt(unnormalizedLightVector, lightinvsqrattradius);
    att *= getAngleAtt(L, pointlight.rotation, 1, 0);
    float3 luminace = (BRDF * pointlight.Intensity * att * ndotl) * pointlight.color.xyz / (4*PI);
}