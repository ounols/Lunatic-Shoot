#ifndef _RESOURCECLASS_H_
#define _RESOURCECLASS_H_


#define RES ResourceClass::getInstance()


#include <d3dx9.h>
#include "SingleTon.h"
#include <map>



using namespace std;


class ResourceClass : public Singleton<ResourceClass>
{
public:
	ResourceClass();
	ResourceClass(const ResourceClass&);
	~ResourceClass();

	bool Initalize(LPDIRECT3DDEVICE9);
	void ShutDown();
	bool insertTexture(LPCSTR key, LPCSTR file);
	bool deleteTexture(LPCSTR);
	LPDIRECT3DTEXTURE9 getTexture(LPCSTR key){
		return texture[key];
	}
	LPDIRECT3DDEVICE9 getDeivce(){
		return g_pD3dDevice;
	}


private:
	//map<LPCSTR, LPD3DXSPRITE> Sprite;

	map<LPCSTR, LPDIRECT3DTEXTURE9> texture;
	LPDIRECT3DDEVICE9 g_pD3dDevice;
};

#endif