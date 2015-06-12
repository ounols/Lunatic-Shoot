#pragma once


//#include <vector>
#include "SceneUnity.h"
#include "LightClass.h"





class SceneGame : public SceneUnity
{
public:
	SceneGame();
	SceneGame(const SceneGame&);
	virtual ~SceneGame();

	bool Initailze();
	void Shutdown();
	void Update();
	//void Render();

	void keyEvent();


private:
	
	LightClass* m_pointlight;
	D3DXVECTOR3 m_pointlightPos;

	
};

