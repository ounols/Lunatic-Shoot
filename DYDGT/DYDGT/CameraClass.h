#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


#include <d3d9.h>
#include <d3dx9.h>


class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();
	void Shutdown();

	void SetupMatrix();

	void CameraTranslation(float x, float y, float z);
	void CameraRotationX(float angle);
	


private:
	D3DXVECTOR3 vEyePt;
	D3DXVECTOR3 vLookatPt;
	D3DXVECTOR3 vUpVec;
};


#endif