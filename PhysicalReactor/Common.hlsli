#ifndef __COMMON_HLSL__
#define __COMMON_HLSL__

#define PI 3.1415f
#define Epsilon=0.00001f
#define MAX_LIGHTS 4

struct coffies
{
    float r;
    float g;
    float b;
    float a;
};


struct DirectionalLight
{
    float4 color;
    float3 direction;
    float Intensity;
    float4 pad;
    float4 pad1;
};

struct PointLight
{
    float4 color;
    float3 position;
    float Intensity;
    float3 rotation;
    float attenuationraduis;
    float sourceraduis;
    float softsourceradius;
    float sourcelength;
    float pad;
};

struct SpotLight
{
    float4 color;
    float3 position;
    float Intensity;
    float3 rotation;
    float InnerConeAngle;
    float OuterConeAngle;
    float attenuationradius;
    float sourceradius;
    float softsourceradius;
    float sourcelengh;
    float pad;
    float pad1;
    float pad2;
};

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
#endif 