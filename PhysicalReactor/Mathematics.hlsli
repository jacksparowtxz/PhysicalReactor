#include"Common.hlsli"

////////////spherical-harmonics Function
/////////////////////////////////////"Simplifying-Spherical-Harmonics-for-Lighting"Dr Graham Hazel

/*float3 spherical_harmonics_Irrandice(coffies Coefficients[9], float3 n)
{

    float SHbasis[9];
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
        color.r += Coefficients[i].r * SHbasis[i];
        color.g += Coefficients[i].g * SHbasis[i];
        color.b += Coefficients[i].b * SHbasis[i];
    }

    return color;
}*/



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

float3 sampleGGX(float2 u, float roughness)
{
    float alpha = roughness * roughness;

    float cosTheta = sqrt((1.0 - u.y) / (1.0 + (alpha * alpha - 1.0) * u.y));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta); // Trig. identity
    float phi = 2*PI * u.x;

	// Convert to Cartesian upon return.
    return float3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
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

float2 sampleHammersley(uint i,float InvNumSample)
{
    return float2(i * InvNumSample, VDC_radicalInverse(i));
}

float3 sampleHemisphere(float u1, float u2)
{
    const float u1p = sqrt(max(0.0, 1.0 - u1 * u1));
    return float3(cos(2 * PI * u2) * u1p, sin(2 * PI * u2) * u1p, u1);
}

void ComputeBasisVector(const float3 N,out float3 S,out float3 T)
{
    T = cross(N, float3(0.0, 1.0, 0.0));
    T = lerp(cross(N, float3(1.0, 0.0, 0.0)), T, step(0.00001, dot(T, T)));

    T = normalize(T);
    S = normalize(cross(N, T));
}

float3 getSamplingVector(uint3 ThreadID, RWTexture2DArray<float4> outputtextures)
{
    float outputWidth, outputHeight, outputDepth;
    outputtextures.GetDimensions(outputWidth, outputHeight, outputDepth);

    float2 st = ThreadID.xy / float2(outputWidth, outputHeight);
    float2 uv = 2.0 * float2(st.x, 1.0 - st.y) - float2(1.0f, 1.0f);

    float3 ret = {0.0f,0.0f,0.0f};
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


void HarmonicBasis(float Basis[9],float x,float y,float z,float r)
{
    Basis[0] = 1.f / 2.f * sqrt(1.f / PI);
    Basis[1] = sqrt(3.f / (4.f * PI)) * y / r;
    Basis[2] = sqrt(3.f / (4.f * PI)) * z / r;
    Basis[3] = sqrt(3.f / (4.f * PI)) * x / r;
    float r2 = r * r;
    Basis[4] = 1.f / 2.f * sqrt(15.f / PI) * x * y / r2;
    Basis[5] = 1.f / 2.f * sqrt(15.f / PI) * y * z / r2;
    Basis[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x * x - y * y + 2 * z * z) / r2;
    Basis[7] = 1.f / 2.f * sqrt(15.f / PI) * z * x / r2;
    Basis[8] = 1.f / 4.f * sqrt(15.f / PI) * (x * x - y * y) / r2;
}

///////////Form UE4
float ClampedPow(float X, float Y)
{
    return pow(max(abs(X), 0.000001f), Y);
}
float2 ClampedPow(float2 X, float2 Y)
{
    return pow(max(abs(X), float2(0.000001f, 0.000001f)), Y);
}
float3 ClampedPow(float3 X, float3 Y)
{
    return pow(max(abs(X), float3(0.000001f, 0.000001f, 0.000001f)), Y);
}
float4 ClampedPow(float4 X, float4 Y)
{
    return pow(max(abs(X), float4(0.000001f, 0.000001f, 0.000001f, 0.000001f)), Y);
}

float PhongShadingPow(float X, float Y)
{

    return ClampedPow(X, Y);
}


///////////////////////

float Square(float x)
{
    return x * x;
}

float2 Square(float2 x)
{
    return x * x;
}

float3 Square(float3 x)
{
    return x * x;
}

float4 Square(float4 x)
{
    return x * x;
}

float Pow2(float x)
{
    return x * x;
}

float2 Pow2(float2 x)
{
    return x * x;
}

float3 Pow2(float3 x)
{
    return x * x;
}

float4 Pow2(float4 x)
{
    return x * x;
}

float Pow3(float x)
{
    return x * x * x;
}

float2 Pow3(float2 x)
{
    return x * x * x;
}

float3 Pow3(float3 x)
{
    return x * x * x;
}

float4 Pow3(float4 x)
{
    return x * x * x;
}

float Pow4(float x)
{
    float xx = x * x;
    return xx * xx;
}

float2 Pow4(float2 x)
{
    float2 xx = x * x;
    return xx * xx;
}

float3 Pow4(float3 x)
{
    float3 xx = x * x;
    return xx * xx;
}

float4 Pow4(float4 x)
{
    float4 xx = x * x;
    return xx * xx;
}

float Pow5(float x)
{
    float xx = x * x;
    return xx * xx * x;
}

float2 Pow5(float2 x)
{
    float2 xx = x * x;
    return xx * xx * x;
}

float3 Pow5(float3 x)
{
    float3 xx = x * x;
    return xx * xx * x;
}

float4 Pow5(float4 x)
{
    float4 xx = x * x;
    return xx * xx * x;
}

float Pow6(float x)
{
    float xx = x * x;
    return xx * xx * xx;
}

float2 Pow6(float2 x)
{
    float2 xx = x * x;
    return xx * xx * xx;
}

float3 Pow6(float3 x)
{
    float3 xx = x * x;
    return xx * xx * xx;
}

float4 Pow6(float4 x)
{
    float4 xx = x * x;
    return xx * xx * xx;
}

float3 NormalSampleToWorldSpace(float3 normalMapSample,float3 unitNormalW,float3 tangentW)
{
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    float3 B = normalize(cross(N, T));

    float3x3 TBN = float3x3(T, B, N);
    float3 bumpedNormalW = mul(normalT, TBN);
    return bumpedNormalW;
}

/////////////////////////////////////  MircoSoft Gltf viewer
float4 SRGBtoLINEAR(float4 srgbIn)
{
#ifdef MANUAL_SRGB
     #ifdef SRGB_FAST_APPROXIMATION
        float3 S2L=float3(2.2, 2.2, 2.2);
        float3 linOUT = pow(srgbIn.xyz, S2L);
     #else
        float3 bless=step(float3(0.04045),srgbIn.xyz);
        float3 linOUT=mix(srgbIn.xyz/float3(12.92),pow(srgbIn.xyz+float3(0.055),float3(2.4)),bless);
    #endif
    return float4(linOUT,srgbIn.w);
#else
    return srgbIn;
#endif



}


uint querySpecularTextureLevels(TextureCube specularTexture)
{
    uint width, height, levels;
    specularTexture.GetDimensions(0, width, height, levels);
    return levels;
}