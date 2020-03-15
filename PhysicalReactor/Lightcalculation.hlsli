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
    float ndf = GGX_NDF1(NdotH, roughness * roughness,1.0);
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
    float ndf = GGX_NDF1(NdotH, roughness * roughness,1.0);
    
    float3 kS = fresnel;
    float3 kd = 1.0 - kS;
    kd *= 1.0 - metalness;
    //float3 kd = lerp(float3(1, 1, 1) - fresnel, float3(0, 0, 0), metalness);

    float3 BRDF = (fresnel * geo * ndf / PI) + LambertDiffuse1(diffusecolor)*kd;

    float att = 1;
    float lightinvsqrattradius = 1.0f / (spotlight.attenuationradius * spotlight.attenuationradius);
    att *= getDistanceAtt(unnormalizedLightVector, lightinvsqrattradius);
    att *= getAngleAtt(L, -spotlight.rotation, spotlight.lightAngleScale, spotlight.lightAngleOffset);

    
   

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
    float geo = gaSchlickGGX_IBL(NdotL, NdotV, roughness * roughness);
    float ndf = GGX_NDF1(NdotH, roughness * roughness,1.0);
    float3 kS = fresnel;
    float3 kd = 1.0 - kS;
    kd *= 1.0 - metalness;
   // float3 kd = lerp(float3(1, 1, 1) - fresnel, float3(0, 0, 0), metalness);
    float3 BRDF = (fresnel * geo * ndf / PI) + LambertDiffuse1(diffusecolor) * kd;

    float att = 1;
    float lightinvsqrattradius = 1.0f / (pointlight.attenuationraduis * pointlight.attenuationraduis);
    att *= getDistanceAtt(unnormalizedLightVector, lightinvsqrattradius);
   // att *= getAngleAtt(L, pointlight.rotation, 1, 0);
    float3 luminace = (BRDF * pointlight.Intensity * att * NdotL) * pointlight.color.xyz / (4 * PI);
    return luminace;
}


float illuminanceSphereOrDisk(float cosTheta,float sinSigmaSqr)
{
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

    float illuminance = 0.0f;


    if (cosTheta*cosTheta>sinSigmaSqr)
    {
        illuminance = PI * sinSigmaSqr * saturate(cosTheta);
    }
    else
    {
        float x = sqrt(1.0f / sinSigmaSqr - 1.0f);
        float y = -x * (cosTheta / sinTheta);
        float sinThetaSqrtY = sinTheta * sqrt(1.0f - y * y);
        illuminance = (cosTheta * acos(y) - x * sinThetaSqrtY) * sinSigmaSqr + atan(sinThetaSqrtY / x);

    }
    return max(illuminance, 0.0f);

}

float3 getSpecularDominantDirArea(float3 N,float3 R,float roughness)
{
    float lerpFactor = (1 - roughness);

    return normalize(lerp(N, R, lerpFactor));

}

float Trace_plane(float3 o, float3 d, float3 planeOrigin, float3 planeNormal)
{
    return dot(planeNormal, (planeOrigin - o) / dot(planeNormal, d));
}

float3 ClosestPointOnSegment(float3 a, float3 b, float3 c)
{
    float3 ab = b - a;
    float t = dot(c - a, ab) / dot(ab, ab);
    return a + saturate(t) * ab;
}

float RectangleSolidAngle(float3 worldPos,
	float3 p0, float3 p1,
	float3 p2, float3 p3)
{
    float3 v0 = p0 - worldPos;
    float3 v1 = p1 - worldPos;
    float3 v2 = p2 - worldPos;
    float3 v3 = p3 - worldPos;

    float3 n0 = normalize(cross(v0, v1));
    float3 n1 = normalize(cross(v1, v2));
    float3 n2 = normalize(cross(v2, v3));
    float3 n3 = normalize(cross(v3, v0));


    float g0 = acos(dot(-n0, n1));
    float g1 = acos(dot(-n1, n2));
    float g2 = acos(dot(-n2, n3));
    float g3 = acos(dot(-n3, n0));

    return g0 + g1 + g2 + g3 - 2 * PI;
}

float3 ClosestPointOnLine(float3 a, float3 b, float3 c)
{
    float3 ab = b - a;
    float t = dot(c - a, ab) / dot(ab, ab);
    return a + t * ab;
}

float Trace_triangle(float3 o, float3 d, float3 A, float3 B, float3 C)
{
    float3 planeNormal = normalize(cross(B - A, C - B));
    float t = Trace_plane(o, d, A, planeNormal);
    float3 p = o + d * t;

    float3 N1 = normalize(cross(B - A, p - B));
    float3 N2 = normalize(cross(C - B, p - C));
    float3 N3 = normalize(cross(A - C, p - A));

    float d0 = dot(N1, N2);
    float d1 = dot(N2, N3);

    float threshold = 1.0f - 0.001f;
    return (d0 > threshold && d1 > threshold) ? 1.0f : 0.0f;
}

float Trace_rectangle(float3 o, float3 d, float3 A, float3 B, float3 C, float3 D)
{
    return max(Trace_triangle(o, d, A, B, C), Trace_triangle(o, d, C, D, A));
}

float3 CalculationSphereArealight(SphereAreaLight sal,float3 WorldPos, float3 N, float3 V, float NdotV, float3 F0, float roughness, float3 diffusecolor, float metalness)
{
    float3 Lunormalized = sal.Position.xyz - WorldPos;
    float dist = length(Lunormalized);
    float3 L = Lunormalized / dist;
    float lumanic=1;

    float sqrDist = dot(Lunormalized,Lunormalized);
    float cosTheta = clamp(dot(N, L), -0.999f, 0.999f);

    float sqrLightRadius = sal.attenuationradius * sal.attenuationradius;
    float sinSigmaSqr = min(sqrLightRadius / sqrDist, 0.9999f);
    lumanic *= illuminanceSphereOrDisk(cosTheta,sinSigmaSqr);
    float3 lightcolor;
    [branch]
    if (lumanic>0)
    {
        {
            float3 R = reflect(-V, N);
            R = getSpecularDominantDirArea(N, R, roughness);
            float3 centerToRay = dot(Lunormalized, R) * R - Lunormalized;
            float3 closestPoint = Lunormalized + centerToRay * saturate(sal.attenuationradius / length(centerToRay));
            L = normalize(closestPoint);
        }
       lightcolor = sal.color.xyz * sal.Intensity * lumanic;

        
        float3 H = normalize(L + V);
        float NdotL = saturate(dot(L, N));
        float HdotV = saturate(dot(H, V));
        float NdotH = saturate(dot(N, H));

        float3 fresnel = Fresnel_Schlick1(F0, NdotL);
        float geo = gaSchlickGGX_IBL(NdotL, NdotV, roughness * roughness);

        float alpha = roughness*roughness;
        float widenedCone = saturate(alpha + sal.attenuationradius / (2 * dist));
        float sphereNormalizedzation = (alpha / widenedCone) * (alpha / widenedCone);
        float ndf = GGX_NDF1(NdotH, roughness * roughness, sphereNormalizedzation);
        float3 kS = fresnel;
        float3 kd = 1.0 - kS;
        kd *= 1.0 - metalness;
        float3 BRDF = (fresnel * geo * ndf / PI) + LambertDiffuse1(diffusecolor) * kd;
        lightcolor *= BRDF;

    }

    return lightcolor;
}

float3 CalculationDiscLight(DiscLight dl, float3 WorldPos, float3 N, float3 V, float NdotV, float3 F0, float roughness, float3 diffusecolor, float metalness)
{

    float3 Lunormalized = dl.Position - WorldPos;
    float dist = length(Lunormalized);
    float3 L = Lunormalized / dist;
    float lumanic = 1;

    float sqrDist = dot(Lunormalized, Lunormalized);
    float3 lightPlaneNormal = dl.Front;
    float cosTheta = clamp(dot(N, L), -0.999, 0.999);
    float sqrLightRadius = dl.attenuationradius * dl.attenuationradius;
    float sinSigmaSqr = sqrLightRadius / (sqrLightRadius + max(sqrLightRadius, sqrDist));
    lumanic *= illuminanceSphereOrDisk(cosTheta, sinSigmaSqr) * saturate(dot(lightPlaneNormal, -L));
    float3 lightcolor;
    [branch]
    if (lumanic>0)
    {
        float3 R = reflect(-V, N);
        R = getSpecularDominantDirArea(N, R, roughness);
        float specularAttenuation = saturate(abs(dot(lightPlaneNormal, R)));


        [branch]
        if (specularAttenuation>0)
        {
            float t = Trace_plane(WorldPos,R,dl.Position,lightPlaneNormal);
            float3 p = WorldPos + R * t;
            float3 centerToRay = p - dl.Position;
            float3 closestPoint = Lunormalized + centerToRay * saturate(dl.attenuationradius / length(centerToRay));
            L = normalize(closestPoint);
        }

        lightcolor = dl.color.rgb * dl.Intensity * lumanic;

        float3 H = normalize(L + V);
        float NdotL = saturate(dot(L, N));
        float HdotV = saturate(dot(H, V));
        float NdotH = saturate(dot(N, H));

        float3 fresnel = Fresnel_Schlick1(F0, NdotL);
        float geo = gaSchlickGGX_IBL(NdotL, NdotV, roughness * roughness);

        //float alpha = roughness * roughness;
       // float widenedCone = saturate(alpha + dl.attenuationradius / (2 * dist));
       // float sphereNormalizedzation = (alpha / widenedCone) * (alpha / widenedCone);
        float ndf = GGX_NDF1(NdotH, roughness * roughness, 1.0);
        float3 kS = fresnel;
        float3 kd = 1.0 - kS;
        kd *= 1.0 - metalness;
        float3 BRDF = (fresnel * geo * ndf / PI) + LambertDiffuse1(diffusecolor) * kd;
        lightcolor *= BRDF;

    }

    return lightcolor;
}

float3 CalculationRectangleLight(RectangleLight rl, float3 WorldPos, float3 N, float3 V, float NdotV, float3 F0, float roughness, float3 diffusecolor, float metalness)
{
    float3 L = rl.Position - WorldPos;
    float dist = length(L);
    L /= dist;
    float lumanic = 1;
    float3 lightcolor = 1;
    [branch]
    if(lumanic>0)
    {
        float3 lightPlaneNormal = rl.LightFront;
        float3 lightLeft = rl.LightRight;
        float3 lightUp = rl.direction;
        float lightwidth = rl.LightWidth;
        float lightheight = rl.LightHeight;
       
        float halfWidth = lightwidth * 0.5;
        float halfHeight = lightheight * 0.5;

        float3 p0 = rl.Position + lightLeft * -halfWidth + lightUp * halfHeight;
        float3 p1 = rl.Position + lightLeft * -halfWidth + lightUp * -halfHeight;

        float3 p2 = rl.Position + lightLeft * halfWidth + lightUp * -halfHeight;
        float3 p3 = rl.Position + lightLeft * halfWidth + lightUp * halfHeight;

        float solidAngle = RectangleSolidAngle(WorldPos, p0, p1, p2, p3);
       
        if (dot(WorldPos-rl.Position,lightPlaneNormal)>0)
        {
            lumanic *= solidAngle * 0.2 * (saturate(dot(normalize(p0 - WorldPos), N)) +
				saturate(dot(normalize(p1 - WorldPos), N)) +
				saturate(dot(normalize(p2 - WorldPos), N)) +
				saturate(dot(normalize(p3 - WorldPos), N)) +
				saturate(dot(normalize(rl.Position - WorldPos), N)));

        }
        else
        {
            lumanic = 0;
        }

        [branch]
        if (lumanic>0)
        {
            float3 R = reflect(-V, N);
            R = getSpecularDominantDirArea(N,R,roughness);
            float specularAttenuation = saturate(abs(dot(lightPlaneNormal, R)));

            [branch]
            if (specularAttenuation>0)
            {
                float traced = Trace_rectangle(WorldPos, R, p0, p1, p2, p3);

                [branch]
                if (traced>0)
                {
                    L = R;
                }
                else
                {
                    float3 tracedPlane = WorldPos + R * Trace_plane(WorldPos, R, rl.Position, lightPlaneNormal);

                    float3 PC[4] =
                    {
                        ClosestPointOnSegment(p0, p1, tracedPlane),
						ClosestPointOnSegment(p1, p2, tracedPlane),
						ClosestPointOnSegment(p2, p3, tracedPlane),
						ClosestPointOnSegment(p3, p0, tracedPlane),
                    };
                    float dist[4] =
                    {
                        distance(PC[0], tracedPlane),
						distance(PC[1], tracedPlane),
						distance(PC[2], tracedPlane),
						distance(PC[3], tracedPlane),
                    };

                    float3 min = PC[0];
                    float minDist = dist[0];
                    [unroll]
                    for (uint i = 1; i < 4;i++)
                    {
                        if (dist[i]<minDist)
                        {
                            minDist = dist[i];
                            min = PC[i];
                        }

                    }
                    L = min - WorldPos;
                }
                L = normalize(L);

            }


            lightcolor = rl.color.rgb * rl.Intensity * lumanic;
            float3 H = normalize(L + V);
            float NdotL = saturate(dot(L, N));
            float HdotV = saturate(dot(H, V));
            float NdotH = saturate(dot(N, H));

            float3 fresnel = Fresnel_Schlick1(F0, NdotL);
            float geo = gaSchlickGGX_IBL(NdotL, NdotV, roughness * roughness);

            float ndf = GGX_NDF1(NdotH, roughness * roughness, 1.0);
            float3 kS = fresnel;
            float3 kd = 1.0 - kS;
            kd *= 1.0 - metalness;
            float3 BRDF = (fresnel * geo * ndf / PI) + LambertDiffuse1(diffusecolor) * kd;
            lightcolor *= BRDF;
        }

    }
    return lightcolor;
}

float3 CalculationTubeLight(TubeLight Tl,float3 WorldPos, float3 N, float3 V, float NdotV, float3 F0, float roughness, float3 diffusecolor, float metalness)
{
    float3 Lunormalized = Tl.Position - WorldPos;
    float dist = length(Lunormalized);
    float3 L = Lunormalized / dist;
    float lumanic = 1;
    float3 lightcolor = 1;
    [branch]
    if (lumanic > 0)
    {
        float sqrDist = dot(Lunormalized,Lunormalized);

        float3 lightright = Tl.LightRight;
        float lightWidth = Tl.sourcewidth;

        float3 P0 = Tl.Position - lightright * lightWidth * 0.5f;
        float3 P1 = Tl.Position + lightright * lightWidth * 0.5f;

        float3 forward = normalize(ClosestPointOnLine(P0, P1, WorldPos) - WorldPos);
        float3 left = lightright;
        float3 up = cross(lightright, forward);

        float3 p0 = Tl.Position - left * (0.5 * lightWidth) + Tl.attenuationradius * up;
        float3 p1 = Tl.Position - left * (0.5 * lightWidth) - Tl.attenuationradius * up;
        float3 p2 = Tl.Position + left * (0.5 * lightWidth) - Tl.attenuationradius * up;
        float3 p3 = Tl.Position + left * (0.5 * lightWidth) + Tl.attenuationradius * up;

        float solidAngle = RectangleSolidAngle(WorldPos, p0, p1, p2, p3);

        lumanic *= solidAngle * 0.2 * (
			saturate(dot(normalize(p0 - WorldPos), N)) +
			saturate(dot(normalize(p1 - WorldPos), N)) +
			saturate(dot(normalize(p2 - WorldPos), N)) +
			saturate(dot(normalize(p3 - WorldPos), N)) +
			saturate(dot(normalize(Tl.Position - WorldPos), N)));


        float3 spherePosition = ClosestPointOnSegment(P0, P1, WorldPos);
        float3 sphereUnormL = spherePosition - WorldPos;
        float3 sphereL = normalize(sphereUnormL);
        float sqrSphereDistance = dot(sphereUnormL, sphereUnormL);

        float LightSphere = PI * saturate(dot(sphereL, N)) * (Tl.attenuationradius * Tl.attenuationradius) / sqrSphereDistance;
        lumanic += LightSphere;

        [branch]
        if (lumanic > 0)
        {
            {
                float3 R = reflect(-V, N);
                R = getSpecularDominantDirArea(N, R, roughness);
                float3 L0 = P0 - WorldPos;
                float3 L1 = P1 - WorldPos;
                float3 Ld = L1 - L0;

                float t = dot(R, L0) * dot(R, Ld) - dot(L0, Ld);
                t /= dot(Ld, Ld) - dot(R, Ld) * dot(R, Ld);
                L = (L0 + saturate(t) * Ld);

                float3 centerToRay = dot(L, R) * R - L;
                float3 closetPoint = L + centerToRay * saturate(Tl.attenuationradius / length(centerToRay));
                L = normalize(closetPoint);

            }
            lightcolor = Tl.color.rgb * Tl.Intensity * lumanic;

            float3 H = normalize(L + V);
            float NdotL = saturate(dot(L, N));
            float HdotV = saturate(dot(H, V));
            float NdotH = saturate(dot(N, H));

            float3 fresnel = Fresnel_Schlick1(F0, NdotL);
            float geo = gaSchlickGGX_IBL(NdotL, NdotV, roughness * roughness);

            float ndf = GGX_NDF1(NdotH, roughness * roughness, 1.0);
            float3 kS = fresnel;
            float3 kd = 1.0 - kS;
            kd *= 1.0 - metalness;
            float3 BRDF = (fresnel * geo * ndf / PI) + LambertDiffuse1(diffusecolor) * kd;
            lightcolor *= BRDF;

        }

    }

    return lightcolor;
}