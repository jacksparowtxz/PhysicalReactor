#include"Common.hlsli"

float3 LambertDiffuse(float3 diffusecolor)
{
    return diffusecolor * (1 / PI);
}

////////////////////////////////https://blog.selfshadow.com/publications/s2012-shading-course/burley/s2012_pbs_disney_brdf_notes_v3.pdf
float3 BurleyDiffuse(float3 diffusecolor,float3 roughness,float3 v,float3 h,float3 n)
{
    float VoN = dot(v, n);
    float LoN = dot(h, n);
    float d = VoN - LoN;

    float FD90 = 0.5 + 2 * d * d * roughness;
    float FdV = 1 + (FD90 - 1) * pow(1 - VoN,5);
    float FdL = 1 + (FD90 - 1) * pow(1 - LoN,5);
    return diffusecolor * ((1 / PI) * FdV * FdL);
}

//////////////////////////http://www1.cs.columbia.edu/~bosun/images/research/egnp06/download/appendix.pdf
float OrenNayarDiffuseColor(float3 diffusecolor, float3 roughness, float3 v, float3 l,float3 n)
{
    float LdotV = dot(l, v);
    float NdotL = dot(l, n);
    float NdotV = dot(n, v);

    float s = LdotV - NdotL * NdotV;
    float t = lerp(1.0, max(NdotL, NdotV), step(0.0, s));

    float sigma2 = roughness * roughness;
    float A = 1.0 + sigma2 * (diffusecolor / (sigma2 + 0.13) + 0.5 / (sigma2 + 0.33));
    float B = 0.45 * sigma2 / (sigma2 + 0.09);

    return diffusecolor * max(0.0, NdotL) * (A + B * s / t) / PI;

}

//////////////////////////////////////////////http://research.tri-ace.com/Data/DesignReflectanceModel_notes.pdf
//////////////////////////////////////////////
/////////////////////////
float3 Diffuse_Gotanda(float3 diffuseColor, float roughness, float3 V, float3 L, float3 H,float3 N)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float F0 = 0.04;
    float VoL = 2 * dot(V, H) * dot(V, H) - 1; // double angle identity
    float Cosri = VoL - dot(N, V) * dot(N,L);
#if 1
    float a2_13 = a2 + 1.36053;
    float Fr = (1 - (0.542026 * a2 + 0.303573 * a) / a2_13) * (1 - pow(1 - dot(N, V), 5 - 4 * a2) / a2_13) * ((-0.733996 * a2 * a + 1.50912 * a2 - 1.16402 * a) * pow(1 - dot(N,V), 1 + rcp(39 * a2 * a2 + 1))+1);
	//float Fr = ( 1 - 0.36 * a ) * ( 1 - pow( 1 - NoV, 5 - 4*a2 ) / a2_13 ) * ( -2.5 * Roughness * ( 1 - NoV ) + 1 );
    float Lm = (max(1 - 2 * a, 0) * (1 - pow(1 - dot(N, L), 5)) + min(2 * a, 1)) * (1 - 0.5 * a * (dot(N, L) - 1)) * dot(N, L);
    float Vd = (a2 / ((a2 + 0.09) * (1.31072 + 0.995584 * dot(N, V)))) * (1 - pow(1 - dot(N, L), (1 - 0.3726732 * dot(N, V) * dot(N, V)) / (0.188566 + 0.38841 * dot(N, V))));
    float Bp = Cosri < 0 ? 1.4 * dot(N, V) * dot(N, L) * Cosri : Cosri;
    float Lr = (21.0 / 20.0) * (1 - F0) * (Fr * Lm + Vd + Bp);
    return diffuseColor / PI * Lr;
#else
	float a2_13 = a2 + 1.36053;
	float Fr = ( 1 - ( 0.542026*a2 + 0.303573*a ) / a2_13 ) * ( 1 - pow( 1 -  dot(N,V), 5 - 4*a2 ) / a2_13 ) * ( ( -0.733996*a2*a + 1.50912*a2 - 1.16402*a ) * pow( 1 -  dot(NoV), 1 + rcp(39*a2*a2+1) ) + 1 );
	float Lm = ( max( 1 - 2*a, 0 ) * ( 1 - Pow5( 1 -  dot(N,L))) + min( 2*a, 1 ) ) * ( 1 - 0.5*a + 0.5*a *  dot(N,L) );
	float Vd = ( a2 / ( (a2 + 0.09) * (1.31072 + 0.995584 *  dot(N,V)))) * ( 1 - pow( 1 -  dot(N,L), ( 1 - 0.3726732 *  dot(NoV) *  dot(NoV) ) / ( 0.188566 + 0.38841 *  dot(NoV) ) ) );
	float Bp = Cosri < 0 ? 1.4 *  dot(N,V) * Cosri : Cosri / max(  dot(N,L), 1e-8 );
	float Lr = (21.0 / 20.0) * (1 - F0) * ( Fr * Lm + Vd + Bp );
	return DiffuseColor / PI * Lr;
#endif
}

//////////////////////Diffuse BRDF
///////////////https://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf
/////////////// 
///////////////
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

