#include "ResourceClass.h"


ResourceClass::ResourceClass()
{
}


ResourceClass::ResourceClass(const ResourceClass& other){


}


ResourceClass::~ResourceClass()
{
}


bool ResourceClass::Initalize(LPDIRECT3DDEVICE9 device){

	g_pD3dDevice = device;


	return true;
}


void ResourceClass::ShutDown(){

	texture.clear();
	return;
}


bool ResourceClass::insertTexture(LPCSTR key, LPCSTR file){
	HRESULT result;
	LPDIRECT3DTEXTURE9 temp = NULL;

	result = D3DXCreateTextureFromFileExA(g_pD3dDevice, file, D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, D3DX_FROM_FILE, NULL, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE
		, NULL, NULL, NULL, &temp);

	if (FAILED(result)) return false;



	texture.insert(make_pair(key,temp));

	return true;
}


bool ResourceClass::deleteTexture(LPCSTR key){
	texture[key]->Release();
	texture.erase(key);

	return true;
}