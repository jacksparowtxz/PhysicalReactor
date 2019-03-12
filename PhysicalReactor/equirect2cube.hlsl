#include"Common.hlsli"
#include"Mathematics.hlsli"


struct Data
{
    float3 coef[9];
};


Texture2D inputTexture : register(t0);
RWTexture2DArray<float4> outputtexture : register(u0);
RWStructuredBuffer<Data> outputdata:register(u1);
SamplerState defaultSampler : register(s0);





[numthreads(32, 32, 1)]
void main(uint3 ThreadID:SV_DispatchThreadID)
{
    float3 v = getSamplingVector(ThreadID,outputtexture);

    float phi = atan2(v.z, v.x);
    float theta = acos(v.y);

    float4 color = inputTexture.SampleLevel(defaultSampler, float2(phi / 2 * PI, theta / PI), 0);
    float basis[9] = { 0 };
    float r = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    HarmonicBasis(basis, v.x, v.y, v.z, r);
    float3 coef[9] = { 0, 0, 0 };
    for (uint i = 0; i < 9;i++)
    {
        coef[i].x += color.x * basis[i];
        coef[i].y += color.y * basis[i];
        coef[i].z += color.z * basis[i];
    }
   
    for (uint j = 0; j < 9;j++)
    {
        coef[j].x = coef[j].x * PI / 4096 * 2048;
        coef[j].y = coef[j].y * PI / 4096 * 2048;
        coef[j].z = coef[j].z * PI / 4096 * 2048;
    }
    outputtexture[ThreadID] = color;
    outputdata.coef = coef;
   
}

