#include "CameraClass.h"
#include "ResourceClass.h"
#include <math.h>


CameraClass::CameraClass()
{
	vEyePt = D3DXVECTOR3(0.0f, 6.0f, -10.0f);
	vLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vUpVec = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}


CameraClass::CameraClass(const CameraClass& other){

}


CameraClass::~CameraClass()
{
}


void CameraClass::Shutdown(){


}


void CameraClass::SetupMatrix(){

	//RES->getDeivce()->SetTransform(D3DTS_WORLD, &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the 
	// origin, and define "up" to be in the y-direction.
	D3DXMATRIXA16 matView;

	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	RES->getDeivce()->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	RES->getDeivce()->SetTransform(D3DTS_PROJECTION, &matProj);

}

void CameraClass::CameraTranslation(float x, float y, float z){
	vEyePt.x += x;
	vEyePt.y += y;
	vEyePt.z += z;
	vLookatPt.x += x;
	vLookatPt.y += y;
	vLookatPt.z += z;
}

void CameraClass::CameraRotationX(float angle){


}