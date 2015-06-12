#pragma once

#include "InputClass.h"
#include "SpriteObject.h"
#include "MeshObject.h"
#include "CameraClass.h"
#include <d3dx9.h>
#include <vector>

using namespace std;




class SceneUnity{

public:
	SceneUnity(){
		m_Camera = NULL;
		m_Camera = new CameraClass();

		Sprite.clear();
		Object.clear();
	}

	virtual ~SceneUnity(){

		//delete 2d sprite
		for (int i = 0; i < Sprite.size(); i++){
			Sprite[i]->Shutdown();
		}

		Sprite.clear();

		//delete 3d object
		for (int i = 0; i < Object.size(); i++){
			Object[i]->Shutdown();
		}

		Object.clear();

		//delete camera
		m_Camera->Shutdown();
		delete m_Camera;
	}

	virtual bool InitailzeScene(InputClass* input){
		m_Input = input;
		if (!Initailze()){
			return false;
		}
		return true;
	}

	virtual bool Initailze(){ return true; };
	virtual void Shutdown() = 0;
	virtual void Update() = 0;
	virtual void Render(){
		//default
		for (int i = 0; i < Sprite.size(); i++){
			Sprite[i]->sprite->Begin(D3DXSPRITE_ALPHABLEND);
			Sprite[i]->sprite->Draw(Sprite[i]->texture, &Sprite[i]->size, &Sprite[i]->center, &Sprite[i]->pos, 0xffffffff);
			Sprite[i]->sprite->End();
		}

		for (int i = 0; i < Object.size(); i++){
			Object[i]->SetupMatrix();
			m_Camera->SetupMatrix();
			Object[i]->Render();
		}

	}

protected:
	CameraClass *m_Camera;
	InputClass* m_Input;

	vector<SpriteObject*> Sprite;
	vector<MeshObject*> Object;

};