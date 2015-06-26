#ifndef _MESHOBJECT_H_
#define _MESHOBJECT_H_

#include <d3d9.h>
#include <d3dx9.h>

class MeshObject
{
public:
	MeshObject();
	MeshObject(const MeshObject&);
	MeshObject(char* filename);
	~MeshObject();

	HRESULT CreateFBX(char* FBXfilename);
	HRESULT CreateX(char* filename);
	void Shutdown();

	void SetupMatrix();

	void Translation(float x, float y, float z, bool isStatic);
	void Scaling(float sx, float sy, float sz, bool isStatic);
	void RotationX(float angle, bool isStatic);
	void RotationY(float angle, bool isStatic);
	void RotationZ(float angle, bool isStatic);

	HRESULT Render();



private:
	void identity();


private:
	char				m_strName[128];
	LPD3DXMESH			m_pMesh;
	D3DMATERIAL9*		m_pMaterials;
	LPDIRECT3DTEXTURE9*	m_textures;
	DWORD				m_dwNumMaterials;
	bool				m_bUseMaterials;

	D3DXMATRIXA16 worldMatrix, recycleMatrix;

public:
	D3DXVECTOR3 pos;
	
};



#endif