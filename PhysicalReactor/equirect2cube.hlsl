#include"Common.hlsli"

Texture2D inputTexture : register(t0);
RWTexture2DArray<float4> outputtexture : register(u0);

SamplerState defaultSampler : register(s0);


float3 getSamplingVector(uint3 ThreadID)
{
    float outputWidth, outputHeight, outputDepth;
    outputtexture.GetDimensions(outputWidth, outputHeight, outputDepth);

    float2 st = ThreadID.xy / float2(outputWidth,outputHeight);
    float2 uv = 2.0 * float2(st.x, 1.0 - st.y) - float2(1.0f, 1.0f);

    float3 ret;
    switch (ThreadID.z)
    {
        case 0:
            ret = float3(1.0, uv.y, -uv.x);
            break;
        case 1:
            ret = float3(-1.0, uv.y, uv.x);
            break;
        case 2:
            ret = float3(uv.x, 1.0, -uv.y);
            break;
        case 3:
            ret = float3(uv.x, -1.0, uv.y);
            break;
        case 4:
            ret = float3(uv.x, uv.y, 1.0);
            break;
        case 5:
            ret = float3(-uv.x, uv.y, -1.0);
            break;
    }
    return normalize(ret);

}


[numthreads(32, 32, 1)]
void main(uint3 ThreadID:SV_DispatchThreadID)
{
    float3 v = getSamplingVector(ThreadID);

    float phi = atan2(v.z, v.x);
    float theta = acos(v.y);

    float4 color = inputTexture.SampleLevel(defaultSampler, float2(phi / 2 * PI, theta / PI), 0);

    outputtexture[ThreadID] = color;

}

