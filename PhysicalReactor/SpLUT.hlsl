#include"BRDF.hlsli"
static const uint NumSamples = 1024;
static const float InvNumSamples = 1.0 / float(NumSamples);

RWTexture2D<float2> LUT : register(u0);


[numthreads(32, 32, 1)]
void main(uint2 ThreadID:SV_DispatchThreadID)
{
    float outputWidth, outputheight;
    LUT.GetDimensions(outputWidth, outputheight);

    float cosLo = ThreadID.x / outputWidth;
    float roughness = ThreadID.y / outputheight;

    cosLo = max(cosLo, 0.001f);
    float3 Lo = float3(sqrt(1.0 - cosLo * cosLo), 0.0, cosLo);


    float DFG1 = 0;
    float DFG2 = 0;

   
  
    for (uint i = 0; i < NumSamples; ++i)
    {

        float2 u = sampleHammersley(i, (float)1/NumSamples);
        float3 Lh = sampleGGX(u, roughness);

        float3 Li = 2.0 * dot(Lo, Lh) * Lh - Lo;

        float cosLi = Li.z;
        float cosLh = Lh.z;
        float cosLoLh = max(dot(Lo, Lh), 0.0);

        if (cosLi>0.0)
        {
            float G = gaSchlickGGX_IBL(cosLi, cosLo, roughness);
            float Gv = G * cosLoLh / (cosLh * cosLo);
            float Fc = pow(1.0 - cosLoLh, 5);

            DFG1 += (1 - Fc) * Gv;
            DFG2 += Fc * Gv;

        }

    }
    LUT[ThreadID] = float2(DFG1, DFG2) * InvNumSamples;
}