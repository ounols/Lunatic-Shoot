#include "LightClass.h"
#include "ResourceClass.h"


LightClass::LightClass()
{
	initialize();
}

LightClass::LightClass(const LightClass& other){
}


LightClass::~LightClass()
{
}


void LightClass::initialize(){

	ZeroMemory(&light, sizeof(light));
	ZeroMemory(&material, sizeof(D3DMATERIAL9));

}


void LightClass::Shutdown(){

}

HRESULT LightClass::CreatePointLight(D3DXCOLOR diffuse, float range, float* x, float* y, float* z){

	light.Type = D3DLIGHT_POINT;
	light.Diffuse = diffuse;
	light.Position = D3DXVECTOR3(*x, *y, *z);
	light.Range = range;
	light.Attenuation0 = 0.0f;    // no constant inverse attenuation
	light.Attenuation1 = 0.125f;    // only .125 inverse attenuation
	light.Attenuation2 = 0.0f;    // no square inverse attenuation

	

	
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	
	return S_OK;

}

HRESULT LightClass::LightEnable(bool isEnable){

	if (!isEnable){
		RES->getDeivce()->SetLight(0, &light);
		RES->getDeivce()->LightEnable(0, FALSE);
		return S_OK;
	}

	RES->getDeivce()->SetLight(0, &light);
	RES->getDeivce()->LightEnable(0, TRUE);

	RES->getDeivce()->SetMaterial(&material);

	return S_OK;

}