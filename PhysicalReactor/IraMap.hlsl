#include"BRDF.hlsli"


static const uint NumSample = 64 * 1024;
static const float InvNumSample = 1.0 / float(NumSample);

TextureCube inputTexture : register(t0);
RWTexture2DArray<float4> outputTexture : register(u0);

SamplerState defaultSampler : register(s0);

[numthreads(32, 32, 1)]
void main(uint3 ThreadID : SV_DispatchThreadID)
{
    float3 N = getSamplingVector(ThreadID,outputTexture);

    float3 S, T;
    ComputeBasisVector(N, S, T);
    float3 irradiacne = 0.0;
    for (uint i = 0; i < NumSample;++i)
    {
        float2 u = sampleHammersley(i, InvNumSample);
        float3 Li = TangentToWorld2(sampleHemisphere(u.x,u.y),N,S,T);
        float cosTheta = max(0.0, dot(Li,N));

        irradiacne += 2.0 * inputTexture.SampleLevel(defaultSampler, Li, 0).rgb * cosTheta;

    }
    
    irradiacne /= float(NumSample);

    outputTexture[ThreadID] = float4(irradiacne, 1.0);

}