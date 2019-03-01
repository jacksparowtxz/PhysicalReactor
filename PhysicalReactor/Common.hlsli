

#define PI 3.1415

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float Intensity;
};

struct PointLight
{
    float4 color;
    float3 position;
    float Intensity;
    float3 rotation;
    float attenuationraduis;
    float sourceraduis;
    float sourcelength;
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