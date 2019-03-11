#include"Mathematics.hlsli"
#include"Common.hlsli"
#include"BRDF.hlsli"



TextureCube inputTexture : register(t0);
RWTexture2DArray<float4> outputtexture : register(u0);

SamplerState defaultSampler : register(s0);


[numthreads(32, 32, 1)]
void main(uint3 ThreadID : SV_DispatchThreadID)
{









}



