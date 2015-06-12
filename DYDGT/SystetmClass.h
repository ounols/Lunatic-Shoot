#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


//Pre-Progressing Dirctives
#define WIN32_LEAN_AND_MEAN

//Includes
#include <Windows.h>

//Custom Includes
#include "InputClass.h"
#include "GraphicsClass.h"
#include "TimerClass.h"


//Class
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	//윈도우를 초기화
	bool Initialize();
	//윈도우를 종료
	void Shutdown();
	//윈도우 메세지 루프를 시작
	void Run();
	//윈도우 메세지 핸들러
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);


private:
	//ESC 확인 & graphics객체의 프레임 확인
	bool Frame();
	//화면을 초기화
	void InitializeWindows(int&, int&);
	//윈도우 디스트로이 에일리언 테크놀로지
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	TimerClass* m_Timer;
};


//prototypes
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//globals
static SystemClass* ApplicationHandle = NULL;


#endif
