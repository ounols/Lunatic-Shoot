#include "SpriteObject.h"
#include "ResourceClass.h"


SpriteObject::SpriteObject()
{
}


SpriteObject::SpriteObject(LPDIRECT3DTEXTURE9 t, D3DXVECTOR3 p, D3DXVECTOR3 c, float sx, float sy, float sz){
	sprite = NULL;
	c_index = 0;
	WalkSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCreateSprite(RES->getDeivce(), &sprite);
	texture = t;
	pos = p;
	center = c;
	size.left = 0.0f;
	size.top = 0.0f;
	size.right = sx;
	size.bottom = sy;
	realSize = size;
	time = 0;
}


SpriteObject::SpriteObject(const SpriteObject& other){

}


SpriteObject::~SpriteObject()
{
}


void SpriteObject::Shutdown(){
	sprite->Release();
	texture->Release();
}


void SpriteObject::StartAnimation(int s_index, int e_index, bool isLoop, float delay){
	
	time++;
	
	if (c_index == -1){
		return;
	}

	if (c_index == 0){
		c_index = s_index;
	}
	else if (c_index > e_index){
		if (isLoop)
			c_index = s_index;
		else{
			c_index = -1;
			return;
		}

	}

	size.left = c_index * realSize.right;
	size.top = 0.0f;
	size.right = (c_index + 1) * realSize.right;
	size.bottom = realSize.bottom;

	if (delay < time){
		c_index++;
		time = 0;
	}

}

void SpriteObject::setAnimationFrame(int index){

	c_index = index;

	size.left = c_index * realSize.right;
	size.top = 0.0f;
	size.right = (c_index + 1) * realSize.right;
	size.bottom = realSize.bottom;

}


void SpriteObject::setAnimationStop(){
	c_index = -1;


}