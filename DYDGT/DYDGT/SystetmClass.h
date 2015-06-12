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

	//�����츦 �ʱ�ȭ
	bool Initialize();
	//�����츦 ����
	void Shutdown();
	//������ �޼��� ������ ����
	void Run();
	//������ �޼��� �ڵ鷯
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);


private:
	//ESC Ȯ�� & graphics��ü�� ������ Ȯ��
	bool Frame();
	//ȭ���� �ʱ�ȭ
	void InitializeWindows(int&, int&);
	//������ ��Ʈ���� ���ϸ��� ��ũ�����
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
