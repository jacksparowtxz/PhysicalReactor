#define PI 3.141592653589793f
#define Epsilon=0.00001f
#define MAX_LIGHTS 4
#define MANUAL_SRGB 1
#define SRGB_FAST_APPROXIMATION 1


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
    float pad; //lightAngleScale=1.0f/max(0.001f,(cosInner-cosOuter));
    float pad1; //lightAngleOffset=-cosOuter*angleScale;
    float pad2;
};



struct PBRInfo
{

    float NdotL; // cos angle between normal and light direction
    float NdotV; // cos angle between normal and view direction
    float NdotH; // cos angle between normal and half vector
    float LdotH; // cos angle between light direction and half vector
    float VdotH; // cos angle between view direction and half vector
    float perceptualRoughness; // roughness value, as authored by the model creator (input to shader)
    float metalness; // metallic value at the surface
    float3 reflectance0; // full reflectance color (normal incidence angle)
    float3 reflectance90; // reflectance color at grazing angle
    float alphaRoughness; // roughness mapped to a more linear change in the roughness (proposed by [2])
    float3 diffuseColor; // color contribution from diffuse lighting
    float3 specularColor; // color contribution from specular lighting

};