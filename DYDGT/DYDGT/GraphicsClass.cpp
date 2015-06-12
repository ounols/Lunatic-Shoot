#include "GraphicsClass.h"
#include "ResourceClass.h"


GraphicsClass::GraphicsClass()
{
	m_DX = NULL;
	m_Scene = NULL;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other){

}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int ScreenHeight, HWND hwnd){

	bool result;


	m_DX = new DXClass();
	if (!m_DX)	return false;

	result = m_DX->Initailze(hwnd);
	if (!result)	return false;


	InitializeTexture();


	return true;
}

void GraphicsClass::InitializeTexture(){
	RES->insertTexture("player", "./player.png");
	RES->insertTexture("player_head", "./player_head.png");

}


void GraphicsClass::Shutdown(){

	if (m_DX != NULL){
		m_DX->Shutdown();
		delete m_DX;
		m_DX = NULL;
	}

	if(m_Scene != NULL){
		m_Scene->Shutdown();
		delete m_Scene;
		m_Scene = NULL;
	}

	return;
}


bool GraphicsClass::Frame(){
	bool result;

	result = Update();
	if (!result) return false;
	
	
	result = Render();
	if (!result) return false;


	return true;
}

bool GraphicsClass::setScene(SceneUnity* scene){

	if (m_Scene != NULL){
		m_Scene->Shutdown();
		delete m_Scene;
		m_Scene = NULL;
	}

	m_Scene = scene;

	if (!m_Scene->InitailzeScene(m_Input)){
		return false;
	}

	return true;

}


bool GraphicsClass::Render(){


	if (m_DX->DrawStart()){

		//render

		m_Scene->Render();

		//======

		m_DX->DrawEnd();
	}
	else{
		return false;
	}


	return true;

}

bool GraphicsClass::Update(){

	m_Scene->Update();

	return true;
}