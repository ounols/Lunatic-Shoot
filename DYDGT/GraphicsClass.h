#ifndef _GRAPICSCLASS_H_
#define _GRAPICSCLASS_H_


//includes
#include <Windows.h>

#include"DXClass.h"
#include"SceneUnity.h"
#include "InputClass.h"


//globals
// 풀 스크린의 유무
const bool FULL_SCREEN = false;



class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();


	bool Initialize(int, int, HWND);
	void InitializeTexture();
	void Shutdown();
	bool Frame();
	bool setScene(SceneUnity*);
	bool Render();
	bool Update();

	void getIC(InputClass* input){
		m_Input = input;
	}




private:
	DXClass* m_DX;
	SceneUnity* m_Scene;
	InputClass* m_Input;
private:
};


#endif