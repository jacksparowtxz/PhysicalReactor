#include "stdafx.h"
#include "SkyLight.h"


SkyLight::SkyLight():skysourcetype(SLSSpecifiedCubemap),SourceCubemapangle(0.0f),cubemapResolution(0.0f),skyDistanceTHreadhold(0.0f),
                     CaptureEmissiveOnly(0.0f),lowerhemisphereissoildColor(0.0f),occlusionmaxdistance(0.0f),occlusioncontance(0.0f),
	                 occlusionexponent(0.0f),minocclusion(0.0f)
{
	occlusiontint = new XMFLOAT4;
	Lowerhemispherecolor = new XMFLOAT4;

	lightype = SKYLIGHT;

}


SkyLight::~SkyLight()
{
	delete occlusiontint;
	delete Lowerhemispherecolor;

}
