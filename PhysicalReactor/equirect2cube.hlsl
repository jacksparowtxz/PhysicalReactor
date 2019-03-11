#include"Common.hlsli"
#include"Mathematics.hlsli"


Texture2D inputTexture : register(t0);
RWTexture2DArray<float4> outputtexture : register(u0);

SamplerState defaultSampler : register(s0);





[numthreads(32, 32, 1)]
void main(uint3 ThreadID:SV_DispatchThreadID)
{
    float3 v = getSamplingVector(ThreadID,outputtexture);

    float phi = atan2(v.z, v.x);
    float theta = acos(v.y);

    float4 color = inputTexture.SampleLevel(defaultSampler, float2(phi / 2 * PI, theta / PI), 0);

    outputtexture[ThreadID] = color;

}

