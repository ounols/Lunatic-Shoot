#ifndef _SPRITEOBJECT_H_
#define _SPRITEOBJECT_H_

#include <d3dx9.h>


class SpriteObject
{
public:
	SpriteObject();
	SpriteObject(LPDIRECT3DTEXTURE9 t, D3DXVECTOR3 p, D3DXVECTOR3 c, float sx, float sy, float sz);
	SpriteObject(const SpriteObject&);
	~SpriteObject();

	void Shutdown();

	void StartAnimation(int s_index, int e_index, bool isLoop, float delay);
	void setAnimationFrame(int index);
	void setAnimationStop();
	int getAnimationFrame(){
		return c_index;
	}


public:
	LPD3DXSPRITE sprite;
	LPDIRECT3DTEXTURE9 texture;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 center;
	RECT size;

	D3DXVECTOR3 WalkSpeed;

private:
	//현재의 키 프레임을 표시
	int c_index;
	RECT realSize;
	float time;

};

#endif