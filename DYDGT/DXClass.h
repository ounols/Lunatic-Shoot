#ifndef _DXCLASS_H_
#define _DXCLASS_H_

//includes
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 


class DXClass
{
public:
	DXClass();
	DXClass(const DXClass&);
	~DXClass();

	bool Initailze(HWND);
	void Shutdown();
	bool DrawStart();
	void DrawEnd();


private:
	LPDIRECT3D9 g_pD3D;
	LPDIRECT3DDEVICE9 g_pD3dDevice;
};

#endif