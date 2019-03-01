#include"Common.hlsli"
//////////////////////Diffuse BRDF
///////////////https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
/////////////// 
///////////////
float3 LambertDiffuse(float3 diffusecolor)
{
    return diffusecolor * (1 / PI);
}

//////////////////////Microfacet Specular BRDF
/////////////////////              D(h)*F(v, h)*G(l, v, h)
////////////////////// f(l, v) = --------------------------
///////////////////////////        4*(n ¡¤ l)*(n ¡¤ v)



////////////////////Specular D
///////////////////                    ¦Á*¦Á
//////////////////  D(h)=------------------------------------------------------------------------
///////////////////            ¦Ð*((n ¡¤ h)*(n ¡¤ h)*(¦Á*¦Á-1)+1)*((n ¡¤ h)*(n ¡¤ h)*(¦Á*¦Á-1)+1)

float GGX_NDF(float a,float NoH)
{
    float d = (NoH * a * a - NoH) * NoH + 1;
    return (a * a) / (PI * d * d);
}

////////////////////////Specular G
////////////////////GGC_Schlick
////////////////////                n¡¤v
////////////////////        G(V)=-----------------
////////////////////////////////  (n¡¤v)*(1-K)+K
////////////////////////
//////////////////// G(l,v,h)=G(l)*G(v);
//////////////////////////        (Rougness+1)*(Roughness+1)
//////////////////////          k=------------------------------
/////////////////////////                  8

float GGX_Schilck(float r,float3 l,float3 v,float3 n)
{
    float k = sqrt(r * r) * 0.5;
    float SchlickV = dot(l, n) * (1 - k) + k;
    float SchlickL = dot(v, n) * (1 - k) + k;
    return 0.25 / (SchlickV * SchlickL);
}

//////////////////////Specular F
////////////////// Spherical Gaussian approximation
/////////////////////F(v,h)=F0+(1-F0)*EXP2(-5.55473(V¡¤H)-6.98316)*(V¡¤H)
/////////////////////////////////////F0= specular reflectance at normal incidence;


float GGX_SGA(float F0,float3 v,float3 n)
{
    return F0 + (1.0f - F0) * pow(1.0f - dot(v,n), 5.0f);
}