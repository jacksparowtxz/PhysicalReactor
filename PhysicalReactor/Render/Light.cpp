#include "stdafx.h"
#include "Light.h"


Light::Light():lightype(NONE), Intensity(0.0f)/*,Lightchannel(1),Intensity(0.0f),Temperature(0.0f),IndirectlightingIntensity(0.0f),volumetricScatteringIntensity(0.0f)
               ,MinRoughness(0.0f),shadowResolutionscale(0.0f),shadowbisa(0.0f),shadowflitersharpen(0.0f),contactshadowlength(0.0f)
	           ,Casttranslucentshadow(0.0f),UseTemperature(false),Castshadow(true),dyanmicindirectLighting(false),Forcecachedshadowingformoveable(false)
	           ,Caststaticshadow(true),Castdyanmicshadow(true),affecttranslucentLights(true),volumetricshadows(false),indirectLightingSaturation(0.0f)
	           ,shadowExponent(0.0f),useareashadowforstationarylight(false),maxdrawdistance(0.0f),maxdistancefaderange(0.0f),fadedistance(0.0f),disabledBrightness(0.0f)*/
{
	//lightfunctionscale = new XMFLOAT4;
	world = new XMFLOAT4X4;
	color = new XMFLOAT4;
}


Light::~Light()
{
	//delete lightfunctionscale;
	delete world;
	delete color;
}
