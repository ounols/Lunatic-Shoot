#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


#include <d3d9.h>
#include <d3dx9.h>

//enum LIGHT {LIGHT_NULL, LIGHT_AMBIENT, LIGHT_SPOT, LIGHT_POINT};

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void initialize();
	void Shutdown();

	HRESULT CreatePointLight(D3DXCOLOR diffuse, float range, float* x, float* y, float* z);
	HRESULT LightEnable(bool isEnable);

private:
	D3DLIGHT9 light;
	D3DMATERIAL9 material;
};


#endif