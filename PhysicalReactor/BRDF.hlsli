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

//////////////////////////////////////////////////Simple vis 
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
float Vis_Implict()
{
    return 0.25f;
}

///////////////////////////////////////////////////
///////////////////////////////
///////////////////////////////////////http://sirkan.iit.bme.hu/~szirmay/brdf6.pdf
float Vis_Neumann(float3 n,float3 v,float3 l)
{
    return 1 / (4 * max(dot(n, l), dot(n, l)));
}

/////////////////////////////////////////
//////////////////////////////////////////
////////////////////////////////////////////////http://sirkan.iit.bme.hu/~szirmay/scook.pdf
float Vis_Kelemen(float3 v,float3 h)
{
    return rcp(4 * dot(v, h) * dot(v, h) + 1e-5);
}


////////////////////////////////////////////
//////////////////////////////////////////
//////////////////////////////////////////////////////
float Vis_Simth(float a,float3 n,float3 l,float3 v)
{
    float Vis_SimthV = dot(n, v) + sqrt(dot(n, v) * (dot(n, v) - dot(n, v) * a * a) + a * a);
    float Vis_SimthL = dot(n, l) + sqrt(dot(n, l) * (dot(n, l) - dot(n, l) * a * a) + a * a);
    return rcp(Vis_SimthV * Vis_SimthL);
}


////////////////////////////////////
///////////////////////////////////
///////////////////////////////////////http://jcgt.org/published/0003/02/03/paper.pdf
float Vis_SmithJointApprox(float a,float3 n,float3 v,float3 l)
{
    float a = sqrt(a);
    float Vis_SimthV = dot(n, l) * (dot(n, v) * (1 - a) + a);
    float Vis_SimthL = dot(n, v) * (dot(n, l) * (1 - a) + a);
    return 0.5 * rcp(Vis_SimthV + Vis_SimthL);
}




//////////////////////////////simple Fresnel
float3 F_Simple(float SpecularColor)
{
    return SpecularColor;
}

//////////////////////////////////
///////////////////////http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.50.2297&rep=rep1&type=pdf
float3 Fresnel_Schlick(float3 specularcolor,float3 v,float3 h)
{
    float Fc = pow(1 - dot(v, h), 5);
    return saturate(50.0f * specularcolor.g) * Fc + (1 - Fc) * specularcolor;
}

float3 Fresnel(float3 SpecularColor, float3 V,float3 H)
{
    float3 SpecularColorSqrt = sqrt(clamp(float3(0, 0, 0), float3(0.99, 0.99, 0.99), SpecularColor));
    float3 n = (1 + SpecularColorSqrt) / (1 - SpecularColorSqrt);
    float3 g = sqrt(n * n + dot(V, H) * dot(V, H) - 1);
    float VOH = dot(V, H);
    return 0.5 * Square(((g - VOH) / (g + VOH)) * (1 + Square(g + VOH * VOH - 1) / (g - VOH * VOH + 1)));
}













//////////////////////Specular F
////////////////// Spherical Gaussian approximation
/////////////////////F(v,h)=F0+(1-F0)*EXP2(-5.55473(V¡¤H)-6.98316)*(V¡¤H)
/////////////////////////////////////F0= specular reflectance at normal incidence;


float GGX_SGA(float F0,float3 v,float3 n)
{
    return F0 + (1.0f - F0) * pow(1.0f - dot(v,n), 5.0f);
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////  H for the half-vector, N for the normal, \alpha for roughness, X for the tangent and Y for the bi-tangent (in my case these are simply aligned with the x and y axes respectively).
////////////////////////////////////////////\ax and \ay represent roughness in the corresponding directions
/////////////////////////////http://graphicrants.blogspot.com/2013/08/specular-brdf-reference.html
float GGXAniso_NDF(float ax,float ay,float3 n,float3 h,float3 x,float y)
{
    float XOH = dot(x, h);
    float YOH = dot(y, h);
    float d = XOH * XOH / (ax * ax) + YOH * YOH / (ay * ay) + dot(n, h) * dot(n, h);
    return 1 / (PI / ax * ay * d * d);
}

