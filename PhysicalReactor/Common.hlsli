

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

