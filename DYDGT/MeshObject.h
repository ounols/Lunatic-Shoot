#ifndef _MESHOBJECT_H_
#define _MESHOBJECT_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <fbxsdk.h>

class MeshObject
{
public:
	MeshObject();
	MeshObject(const MeshObject&);
	MeshObject(char* filename);
	~MeshObject();

	HRESULT CreateFBX(char* FBXfilename);
	HRESULT CreateX(char* filename);
	void loadMeshFromFbxNodeAttribute(FbxNodeAttribute* attr);
	void generateBoundingBox()
	{
		void* firstPos;
		m_pMesh->LockVertexBuffer(0, &firstPos);
		D3DXComputeBoundingBox(
			(D3DXVECTOR3*)firstPos,
			m_pMesh->GetNumVertices(),
			D3DXGetFVFVertexSize(m_pMesh->GetFVF()),
			&minPoint,
			&maxPoint);

		m_pMesh->UnlockVertexBuffer();
	}
	virtual void getBoundingBox(D3DXVECTOR3& _minPoint, D3DXVECTOR3& _maxPoint)
	{
		_minPoint = this->minPoint;
		_maxPoint = this->maxPoint;
	}
	void Shutdown();

	void SetupMatrix();

	void Translation(float x, float y, float z, bool isStatic);
	void Scaling(float sx, float sy, float sz, bool isStatic);
	void RotationX(float angle, bool isStatic);
	void RotationY(float angle, bool isStatic);
	void RotationZ(float angle, bool isStatic);

	D3DXVECTOR3 getPosition();

	HRESULT Render();

	void addChild(MeshObject* child);
	void addChild(MeshObject* child, int tag);

	IDirect3DTexture9* createTextureFromFile(const char* fileName);



private:
	void identity();


private:
	char				m_strName[128];
	LPD3DXMESH			m_pMesh;
	D3DMATERIAL9*		m_pMaterials;
	vector<D3DMATERIAL9> m_pMeterials2;
	LPDIRECT3DTEXTURE9*	m_textures;
	vector<LPDIRECT3DTEXTURE9> m_textures2;
	DWORD				m_dwNumMaterials;
	bool				m_bUseMaterials;

	D3DXMATRIXA16 worldMatrix, recycleMatrix;

	vector<MeshObject*> m_child;

	D3DXVECTOR3 minPoint, maxPoint;
};



#endif