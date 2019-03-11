#include"Common.hlsli"

////////////spherical-harmonics Function
/////////////////////////////////////"Simplifying-Spherical-Harmonics-for-Lighting"Dr Graham Hazel

/*float3 Coefficients[9] =
{
 2.0952,    2.0944,    1.91702,
0.653221,	0.693606,	0.762548,
-0.0489448,	-0.0396268	,-0.0285173,
0.0441169	,0.0430626,	0.0677261,
0.03926,	0.0186167,	0.00929239,
0.183267,	0.158966	,0.1199,
-0.168802,	-0.157308	,-0.159969,
-0.0361812,	-0.0386558	,-0.0466892,
-0.199456,	-0.169909,	-0.153581,
};*/

float3 spherical_harmonics_Irrandice(float3 Coefficients[9],float3 n)
{

    float SHbasis[16];
    float x = n.x;
    float y = n.y;
    float z = n.z;
    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;

    SHbasis[0] = 1.0f / sqrt(1.0f / 4.0f*PI);
    SHbasis[1] = sqrt(3.0f / (4.0f * PI)) * y;
    SHbasis[2] = sqrt(3.0f / (4.0f * PI)) * z;
    SHbasis[3] = sqrt(3.0f / (4.0f * PI)) * x;
    SHbasis[4] = 1.f / 2.f * sqrt(15.f / PI) * x * y;
    SHbasis[5] = 1.f / 2.f * sqrt(15.f / PI) * y * z;
    SHbasis[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x * x - y * y + 2 * z * z);
    SHbasis[7] = 1.f / 2.f * sqrt(15.f / PI) * z * x;
    SHbasis[8] = 1.f / 4.f * sqrt(15.f / PI) * (x * x - y * y);

    float3 color = { 0.f, 0.f, 0.f };
    for (int i = 0; i < 9;i++)
    {
        color += Coefficients[i] * SHbasis[i];
    }

    return color;
}

float3x3 GetTangentBasis(float3 TangentZ)
{
    float3 UpVector = abs(TangentZ.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
    float3 TangentX = normalize(cross(UpVector, TangentZ));
    float3 TangentY = cross(TangentZ, TangentX);
    return float3x3(TangentX, TangentY, TangentZ);
}

float3 TangentToWorld(float3 Vec, float3 TangentZ)
{
    return mul(Vec, GetTangentBasis(TangentZ));
}
float3 TangentToWorld2(const float3 v, const float3 N, const float3 S, const float3 T)
{
    return S * v.x + T * v.y + N * v.z;
}
//////////////////////////////////////////ImprotanceSample
//////////////////////////////////////https://schuttejoe.github.io/post/ggximportancesamplingpart1/
////////////////////////////////////////https://www.tobias-franke.eu/log/2014/03/30/notes_on_importance_sampling.html
float4 ImportanceSampleGGX(float2 Xi, float Roughness)
{
    float a2 = Roughness * Roughness;
    float Phi = 2 * PI * Xi.x;
    float CosTheta = sqrt((1 - Xi.y) / (1 + (a2 * a2 - 1) * Xi.y));
    float SinTheta = sqrt(1 - CosTheta * CosTheta);

    float3 H;
    H.x = SinTheta * cos(Phi);
    H.y = SinTheta * sin(Phi);
    H.z = CosTheta;
	
    float d = (CosTheta * a2 - CosTheta) * CosTheta + 1;
    float D = a2 / (PI * d * d);
    float PDF = D * CosTheta;
    return float4(H, PDF);
}



/////////////////////////////////Van der Corput sequence
/////////////////////////////////http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
float VDC_radicalInverse(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

//////////////////
/////////////////////

float2 Hammersley(uint Index, uint NumSamples, uint2 Random)
{
    float E1 = frac((float) Index / NumSamples + float(Random.x & 0xffff) / (1 << 16));
    float E2 = VDC_radicalInverse(Index)* 2.3283064365386963e-10;
    return float2(E1, E2);
}

float2 sampleHammersley(uint i,float InvNumSample, uint2 n)
{
    return float2(i * InvNumSample, VDC_radicalInverse(n.y));
}

float3 sampleHemisphere(float u1, float u2)
{
    const float u1p = sqrt(max(0.0, 1.0 - u1 * u1));
    return float3(cos(2 * PI * u2) * u1p, sin(2 * PI * u2) * u1p, u1);
}

void ComputeBasisVector(const float3 N,out float3 S,out float3 T)
{
    T = cross(N, float3(0.0, 1.0, 0.0));
    T = lerp(cross(N, float3(1.0, 0.0, 0.0)), T, step(0.00001f, dot(T, T)));

    T = normalize(T);
    S = normalize(cross(N, T));
}

float3 getSamplingVector(uint3 ThreadID, RWTexture2DArray<float4> outputtextures)
{
    float outputWidth, outputHeight, outputDepth;
    outputtextures.GetDimensions(outputWidth, outputHeight, outputDepth);

    float2 st = ThreadID.xy / float2(outputWidth, outputHeight);
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