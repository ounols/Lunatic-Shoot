#include "DXClass.h"
#include "ResourceClass.h"


DXClass::DXClass()
{
	g_pD3D = NULL;
	g_pD3dDevice = NULL;
}

DXClass::DXClass(const DXClass& other){

}


DXClass::~DXClass()
{
}


bool DXClass::Initailze(HWND hWnd){

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);


	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the D3DDevice
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3dDevice)))
	{
		return E_FAIL;
	}
	// Turn on the zbuffer
	g_pD3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Turn on ambient lighting 
	g_pD3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00f5f5f5);
	g_pD3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	g_pD3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	g_pD3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);

	RES->Initalize(g_pD3dDevice);


	return true;
}


void DXClass::Shutdown(){

	if (g_pD3D != NULL){
		g_pD3D->Release();
		g_pD3D = NULL;
	}


	if (g_pD3dDevice != NULL){
		g_pD3dDevice->Release();
		g_pD3dDevice = NULL;
	}

	return;
}



bool DXClass::DrawStart(){

	// white색으로 뒷버퍼를 초기화
	g_pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100, 100, 100), 1.0f, 0);

	// 그리기 시작
	if (SUCCEEDED(g_pD3dDevice->BeginScene())){
		return true;
	}
	else{
		return false;
	}

}

void DXClass::DrawEnd(){
	g_pD3dDevice->EndScene();

	g_pD3dDevice->Present(NULL, NULL, NULL, NULL);
}