#include "SceneGame.h"
#include "ResourceClass.h"


SceneGame::SceneGame()
{
	m_Input = NULL;
	m_pointlight = NULL;
}


SceneGame::SceneGame(const SceneGame& other){

}


SceneGame::~SceneGame()
{
}


bool SceneGame::Initailze(){

	m_pointlight = new LightClass();
	m_pointlightPos = D3DXVECTOR3(0, 2.0, 0);
	m_pointlight->CreatePointLight(D3DXCOLOR(0.5, 0.5, 0.5, 1.0), 100.0f, &m_pointlightPos.x, &m_pointlightPos.y, &m_pointlightPos.z);
	m_pointlight->LightEnable(true);

	Sprite.push_back(new SpriteObject(RES->getTexture("player"), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 55, 65, 0));
	Sprite.push_back(new SpriteObject(RES->getTexture("player_head"), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 64, 52, 0));
	Sprite[1]->setAnimationFrame(0);

	Object.push_back(new MeshObject("./ss/tiger.x"));
	Object[0]->Scaling(1.2,1.2,1.2, true);
	Object.push_back(new MeshObject("./ss/tiger.x"));
	Object[1]->Scaling(1.2, 1.2, 1.2, true);
	Object.push_back(new MeshObject("./ss/tiger.x"));
	Object[2]->Scaling(1.2, 1.2, 1.2, true);

	Object.push_back(new MeshObject("./ss/ground.x"));
	//Object[3]->Scaling(5, 0, 5, true);
	Object[3]->Translation(0, -1, 0, true);
	Object.push_back(new MeshObject("./ss/tiger.x"));
	
	return true;
}

void SceneGame::Shutdown(){

	//for (int i = 0; i < Sprite.size(); i++){
	//	Sprite[i]->Shutdown();
	//}

	//Sprite.clear();

	//for (int i = 0; i < Object.size(); i++){
	//	Object[i]->Shutdown();
	//}

	//Object.clear();

}

//void SceneGame::Render(){
//
//	for (int i = 0; i < Sprite.size(); i++){
//		Sprite[i]->sprite->Begin(D3DXSPRITE_ALPHABLEND);
//		Sprite[i]->sprite->Draw(Sprite[i]->texture, &Sprite[i]->size, &Sprite[i]->center, &Sprite[i]->pos, 0xffffffff);
//		Sprite[i]->sprite->End();
//	}
//
//}

void SceneGame::Update(){

	keyEvent();

	Sprite[1]->pos = Sprite[0]->pos;
	Sprite[1]->pos.x -= 5;

	Object[0]->RotationY(0.1, true);
	Object[1]->RotationX(0.1, true);
	Object[2]->RotationZ(0.1, true);

	Object[1]->Translation(2, 0, 0, false);
	Object[2]->Translation(-2, 0, 0, false);


}

void SceneGame::keyEvent(){
	if (m_Input->IsKeyDown(VK_DOWN)){
		Sprite[1]->StartAnimation(0, 1, true, 5);
		Sprite[0]->pos.y += 3;
		m_Camera->CameraTranslation(0, 0, -0.5);
		//m_Camera->CameraRotationX(1);
	}
	else if (m_Input->IsKeyDown(VK_UP)){
		Sprite[1]->StartAnimation(4, 5, true, 5);
		Sprite[0]->pos.y -= 3;
		m_Camera->CameraTranslation(0, 0, 0.5);
	}

	if (m_Input->IsKeyDown(VK_LEFT)){
		Sprite[1]->StartAnimation(6,7,true, 5);
		if (Sprite[0]->WalkSpeed.x >= -5){
			Sprite[0]->WalkSpeed.x--;
		}
		m_Camera->CameraTranslation(-0.5, 0, 0);
	}
	else if (!m_Input->IsKeyDown(VK_LEFT)){
		if (Sprite[0]->WalkSpeed.x < 0){
			Sprite[0]->WalkSpeed.x++;
		}
	}

	if (m_Input->IsKeyDown(VK_RIGHT)){
		Sprite[1]->StartAnimation(2, 3, true, 5);
		if (Sprite[0]->WalkSpeed.x <= 5){
			Sprite[0]->WalkSpeed.x++;
		}
		m_Camera->CameraTranslation(0.5, 0, 0);
	}
	else if (!m_Input->IsKeyDown(VK_RIGHT)){
		if (Sprite[0]->WalkSpeed.x > 0){
			Sprite[0]->WalkSpeed.x--;
		}
	}

	if (!m_Input->IsKeyDown(VK_DOWN) &&
		!m_Input->IsKeyDown(VK_UP) &&
		!m_Input->IsKeyDown(VK_LEFT) &&
		!m_Input->IsKeyDown(VK_RIGHT)){
		Sprite[1]->setAnimationFrame(0);
	}

	Sprite[0]->pos.x += Sprite[0]->WalkSpeed.x;

}