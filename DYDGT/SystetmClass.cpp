#include "SystetmClass.h"
#include "ResourceClass.h"

#include "SceneGame.h"


SystemClass::SystemClass()
{
	m_Input = NULL;
	m_Graphics = NULL;
	m_Timer = NULL;
}


SystemClass::SystemClass(const SystemClass& other){

}


SystemClass::~SystemClass()
{
}

bool SystemClass::Initialize(){
	//ȭ���� �ʺ�, ����
	int screenWidth, screenHeight;
	//�׷����� �ʱ�ȭ ����
	bool result;


	//���̿� �ʺ� �ʱ�ȭ
	screenHeight = 720;
	screenWidth = 1280;


	//��������Ƿ� ������ �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);



	//input ��ü�� ���� (Ű���� �Է��� ó��)
	m_Input = new InputClass;
	if (!m_Input) return false;

	m_Input->Initialize();


	//grapic������ ���� (�׷����� ó��)
	m_Graphics = new GraphicsClass;
	if (!m_Graphics) return false;

	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result) return false;

	m_Graphics->getIC(m_Input);

	m_Graphics->setScene(new SceneGame());

	m_Timer = new TimerClass();
	if (!m_Timer) return false;

	result = m_Timer->Initialize();
	if (!result){
		return false;
	}


	return true;


}

void SystemClass::Shutdown(){
	//Grapic��ü�� ��ȯ
	if (m_Graphics){
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	//input��ü�� ��ȯ
	if (m_Input){
		delete m_Input;
		m_Input = NULL;
	}

	if (m_Timer){
		delete m_Timer;
		m_Timer = 0;
	}

	if (RES != NULL){
		RES->delInstance();
	}

	//â�� ��Ʈ���� ���ϸ���
	ShutdownWindows();

	return;


}

void SystemClass::Run(){
	//Windows Message
	MSG msg;
	//������ �����°�?
	bool done;
	//������ ���� �۵� ����
	bool result;


	//�޼��� ����ü �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));

	//����! �޼��� ���� ����
	done = false;
	while (!done){

		//�޼����� ó��
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���� ���������� ����
		if (msg.message == WM_QUIT) done = true;
		else{
			result = Frame();
			if (!result){
				done = true;
			}

		}


	}

	return;
}


bool SystemClass::Frame(){
	//grapic��ü�� ������ ���� �۵� ����
	bool result;

	//ESCŰ�� ���� ����
	if (m_Input->IsKeyDown(VK_ESCAPE)){
		return false;
	}


	//�׷����� �۾��� ó��
	result = m_Graphics->Frame();
	if (!result){
		return false;
	}

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam){
	switch (umsg){
	case WM_KEYDOWN:
		m_Input->KeyDown((unsigned int)wparam);
		return 0;

	case WM_KEYUP:
		m_Input->KeyUp((unsigned int)wparam);
		return 0;

	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight){
	WNDCLASSEX wc;
	//������ ȭ�� ����
	DEVMODE dmScreenSettings;
	//������ ȭ�� ������
	int posX, posY;


	// �ܺ� �����͸� �� ��ü�� ����
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ�
	m_hinstance = GetModuleHandle(NULL);

	// ���ø����̼� �̸� ����
	m_applicationName = L"Engine";

	// Ŭ������ �⺻ �������� ����
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ������ Ŭ���� ���
	RegisterClassEx(&wc);






	// ����� ȭ�� �ػ�
	//screenWidth = GetSystemMetrics(SM_CXSCREEN);
	//screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//Ǯ ��ũ���� �۵� ��
	if (FULL_SCREEN){
		//������ 32��Ʈ�� ���߰�, ȭ���� ����ũ�� ũ�⿡ ����
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//Ǯ��ũ������ ���÷��� ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;

		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	}
	else{
		//â���� ����
		//screenWidth = 800;
		//screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, /*WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP*/NULL, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	}







	


	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ���� ��
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ���콺 Ŀ���� ����
	ShowCursor(false);

	return;
}


void SystemClass::ShutdownWindows(){

	//Ŀ���� ǥ��
	ShowCursor(true);

	if (FULL_SCREEN){
		//Ǯ��ũ���̸� ����
		ChangeDisplaySettings(NULL, 0);
	}

	//�����츦 ����
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//���ø����̼� ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//�ܺ� ������ ������ ����
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam){
	switch (umessage)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	default:
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
}