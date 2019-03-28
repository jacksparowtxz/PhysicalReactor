#include"BRDF.hlsli"


static const uint NumSamples = 1024;
static const float InvNumSamples = 1.0 / float(NumSamples);


cbuffer SpecularMapFilterSetting : register(b0)
{
    float roughness;
};

TextureCube inputTexture : register(t0);
RWTexture2DArray<float4> outputTexture : register(u0);

SamplerState defaulteSampler : register(s0);



[numthreads(32, 32, 1)]
void main(uint3 ThreadID:SV_DispatchThreadID)
{
    uint outputWidth, outputHeight, outputdepth;
    outputTexture.GetDimensions(outputWidth,outputHeight,outputdepth);
    if (ThreadID.x>=outputWidth||ThreadID.y>=outputHeight)
    {
        return;
    }

    float inputWidth, inputHeight, inputlevel;
    inputTexture.GetDimensions(0,inputWidth, inputHeight, inputlevel);

    float wt = 4.0 * PI / (6 * inputWidth * inputHeight);

    float3 N = getSamplingVector(ThreadID,outputTexture);
    float3 Lo = N;

    float3 S, T;
    ComputeBasisVector(N, S, T);

    float3 color = 0;
    float weight = 0;

    for (uint i = 0; i < NumSamples;++i)
    {
        float2 u = sampleHammersley(i, InvNumSamples);
        float3 Lh = TangentToWorld2(ImportanceSampleGGX(u,roughness).xyz,N,S,T);

        float3 Li = 2.0 * dot(Lo, Lh) * Lh - Lo;
        float cosLi = dot(N,Li);
        if(cosLi>0.0)
        {
           float cosLh = max(dot(N, Lh), 0.0);
           float pdf = GGX_NDF(cosLh, roughness) * 0.25;
           float ws = 1.0 / (NumSamples * pdf);
           float miplevel = max(0.5 * log2(ws / wt) + 1.0, 0.0);
           color += inputTexture.SampleLevel(defaulteSampler, Li, miplevel).rgb * cosLi;
           weight += cosLi;
        }

    }
    color /= weight;
    outputTexture[ThreadID] = float4(color, 1.0f);
}