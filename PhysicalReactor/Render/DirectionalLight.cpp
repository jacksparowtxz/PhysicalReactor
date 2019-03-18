#include "stdafx.h"
#include "DirectionalLight.h"


DirectionalLight::DirectionalLight():color({ 1.0f,1.0f,1.0f,1.0f }), Direction({ 0.0f,0.0f,0.0f }), Intensity(10.f)/*:lightsourceangle(0.0f),occlusionmaskdrakness(0.0f),occlusiondepthrange(0.0f),DistanceFieldShadowDistance(0.0f),
                                    DistanceFieldTraceDistance(0.0f),LightSourceAngle(0.0f),dynamicshadowdistancemoveablelight(0.0f),
	                                dynamicshadowdistancestationarylight(0.0f), NumDynamicShadowcascaded(0.0f),DistrbutionExponent(0.0f),
	                                TransitionFraction(0.0f),Distancefadeoutfraction(0.0f),castModulatedShadows(false),AtmosphereFogsunlight(false), 
	                                Lightshaftsocclusion(false),lightshaftbloom(false),bloomthreashold(false)*/
{
	/*ModulatedShadowscolor = new XMFLOAT4;
	bloomtint = new XMFLOAT4;*/
}


DirectionalLight::~DirectionalLight()
{
	/*delete ModulatedShadowscolor;
	delete bloomtint;*/

}

DirectionalLight::DirectionalLight(XMFLOAT3 direction):Direction(direction)
{
	color = { 1.0f,1.0f,1.0f,1.0f };
    Intensity=1.0f;
	pading={ 0.0f,0.0f,0.0f,0.0f };
	pading1={0.0f,0.0f,0.0f,0.0f };
}

void DirectionalLight::SetPostion(XMFLOAT3 direction)
{
	Direction = direction;
}


