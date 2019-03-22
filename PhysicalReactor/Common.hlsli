#define PI 3.141592f
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


