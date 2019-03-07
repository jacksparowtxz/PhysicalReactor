#include"Mathematics.hlsli"
#include"BRDF.hlsli"


/////////////////////////////////
////////////////////////////////////////////Basic Theory of  Physcially Based Rendering 
/////////////////////////////But Use Unreal Engine 4 Version
/*float3 SpecularIBL(uint2 Random,float3 SpecularColor,float rougness,float3 N,float3 V)
{
    float3 SpeclarLighting = 0;
    const uint NumSamples = 1024;
    for (uint i = 0; i < NumSamples;i++)
    {
        float2 Xi = Hammersley(i, 1 / NumSamples, Random);
        float3 H = TangentToWorld(ImprotanceSampleGGX(Xi, Pow4(rougness)).xyz, N);
        float3 L = 2 * dot(V, N) * H - V;

        float NoV = saturate(dot(N, V));
        float NoL = saturate(dot(N, L));
        float NoH = saturate(dot(N, H));
        float VoH = saturate(dot(V, H));

        if (NoL>0)
        {
            float3 SampleColor = AmbientCubemap.SampleLevel(AmbientCubeSampler, L, 0).rgb;

            float Vis = Vis_SmithJointApprox(Pow4(rougness), NoV, NoL);
            float Fc=pow(1-VoH,5);
            float3 F=(1-Fc)*SpecularColor+Fc;
            SpecularLighting+=SampleColor*F*(NoL*Vis*(4*VoH/NoH));

        }

    }
    return SpeclarLighting / NumSamples;

}*/
TextureCube AmbientCubemap;
SamplerState AmbientCubemapSampler;


float3 prefilterEnvMap(uint2 Random,float roughness,float3 R)
{
    float3 FilteredColor = 0;
    float Weight = 0;
		
    const uint NumSamples = 64;
    for (uint i = 0; i < NumSamples; i++)
    {
        float2 E = Hammersley(i, NumSamples, Random);
        float3 H = TangentToWorld(ImportanceSampleGGX(E, Pow4(roughness)).xyz, R);
        float3 L = 2 * dot(R, H) * H - R;

        float NoL = saturate(dot(R, L));
        if (NoL > 0)
        {
            FilteredColor += AmbientCubemap.SampleLevel(AmbientCubemapSampler, L, 0).rgb * NoL;
            Weight += NoL;
        }
    }

    return FilteredColor / max(Weight, 0.001);
}



float2 IntegrateBRDF(uint2 Random,float Roughness, float3 N,float3 V)
{
    float3 V;
    V.x = sqrt(1.0f - dot(N, V) * dot(N, V)); // sin
    V.y = 0;
    V.z = dot(N, V); // cos
    float A = 0;
    float B = 0;
    const uint NumSamples = 1024;
    for (uint i = 0; i < NumSamples; i++)
    {
        float2 Xi = Hammersley(i, NumSamples, Random);
        float3 H = ImportanceSampleGGX(Xi, Pow4(Roughness)).xyz;
        float3 L = 2 * dot(V, H) * H - V;
        float NoL = saturate(L.z);
        float NoH = saturate(H.z);
        float VoH = saturate(dot(V, H));
        if (NoL > 0)
        {
            float G = Vis_SmithJointApprox(Roughness, N, V, L);
            float G_Vis = G * dot(V, H) / (dot(N, H) * dot(N, V));
            float Fc = pow(1 - dot(V, H), 5);
            A += (1 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    return float2(A, B) / NumSamples;
}

/*float3 ApproximateSpecularIBL(float3 SpecularColor, float Roughness, float3 N, float3 V)
{
    float NoV = saturate(dot(N, V));
    float3 R = 2 * dot(V, N) * N - V;
    float3 PrefilteredColor = PrefilterEnvMap(Roughness, R);
    float2 EnvBRDF = IntegrateBRDF(Roughness, NoV);
    return PrefilteredColor * (SpecularColor * EnvBRDF.x + EnvBRDF.y);
}*/