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
	//화면의 너비, 높이
	int screenWidth, screenHeight;
	//그래픽의 초기화 유무
	bool result;


	//높이와 너비를 초기화
	screenHeight = 720;
	screenWidth = 1280;


	//윈도우아피로 윈도우 초기화
	InitializeWindows(screenWidth, screenHeight);



	//input 객체를 생성 (키보드 입력을 처리)
	m_Input = new InputClass;
	if (!m_Input) return false;

	m_Input->Initialize();


	//grapic객제를 생성 (그래픽을 처리)
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
	//Grapic객체를 반환
	if (m_Graphics){
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = NULL;
	}

	//input객체를 반환
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

	//창을 디스트로이 에일리언
	ShutdownWindows();

	return;


}

void SystemClass::Run(){
	//Windows Message
	MSG msg;
	//루프를 끝내는가?
	bool done;
	//프레임 정상 작동 유무
	bool result;


	//메세지 구조체 초기화
	ZeroMemory(&msg, sizeof(MSG));

	//ㅁㄴ! 메세지 루프 실행
	done = false;
	while (!done){

		//메세지를 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//루프 빠져나오는 조건
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
	//grapic객체의 프레임 정상 작동 유무
	bool result;

	//ESC키를 눌러 종료
	if (m_Input->IsKeyDown(VK_ESCAPE)){
		return false;
	}


	//그래픽의 작업을 처리
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
	//윈도우 화면 세팅
	DEVMODE dmScreenSettings;
	//윈도우 화면 포지션
	int posX, posY;


	// 외부 포인터를 이 객체로 설정
	ApplicationHandle = this;

	// 이 어플리케이션의 인스턴스
	m_hinstance = GetModuleHandle(NULL);

	// 어플리케이션 이름 설정
	m_applicationName = L"Engine";

	// 클래스를 기본 설정으로 맞춤
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

	// 윈도우 클래스 등록
	RegisterClassEx(&wc);






	// 모니터 화면 해상도
	//screenWidth = GetSystemMetrics(SM_CXSCREEN);
	//screenHeight = GetSystemMetrics(SM_CYSCREEN);

	//풀 스크린이 작동 시
	if (FULL_SCREEN){
		//색상을 32비트에 맞추고, 화면을 데스크톱 크기에 맞춤
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//풀스크린으로 디스플레이 설정
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;

		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	}
	else{
		//창모드로 맞춤
		//screenWidth = 800;
		//screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;

		m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, /*WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP*/NULL, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);
	}







	


	// 윈도우를 화면에 표시하고 포커스를 줌
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 마우스 커서를 숨김
	ShowCursor(false);

	return;
}


void SystemClass::ShutdownWindows(){

	//커서를 표시
	ShowCursor(true);

	if (FULL_SCREEN){
		//풀스크린이면 해제
		ChangeDisplaySettings(NULL, 0);
	}

	//윈도우를 제거
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	//어플리케이션 제거
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	//외부 포이터 참조도 제거
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