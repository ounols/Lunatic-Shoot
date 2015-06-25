#include "MeshObject.h"
#include "ResourceClass.h"
//#include "MeshLoader.h"


MeshObject::MeshObject()
{
	strcpy_s(m_strName, "");
	m_pMesh = NULL;
	m_pMaterials = NULL;
	m_textures = NULL;
	m_dwNumMaterials = NULL;
	m_bUseMaterials = true;

	identity();


}

MeshObject::MeshObject(const MeshObject& other){

}

MeshObject::MeshObject(char* filename){

	strcpy_s(m_strName, "");
	m_pMesh = NULL;
	m_pMaterials = NULL;
	m_textures = NULL;
	m_dwNumMaterials = NULL;
	m_bUseMaterials = true;

	identity();

	CreateX(filename);
}


MeshObject::~MeshObject()
{
}


void MeshObject::identity(){

	D3DXMatrixIdentity(&worldMatrix);
	D3DXMatrixIdentity(&recycleMatrix);


}


HRESULT MeshObject::CreateFBX(char* FBXfilename)
{
	////LPD3DXBUFFER pD3DXMtrlBuffer;


	//// Load the mesh from the specified file
	//MeshLoader* FBXLoader = new MeshLoader();

	//FBXLoader->Initialize();
	//FBXLoader->LoadScene(FBXfilename);
	//FBXLoader->AnalyzeFBX();

	//m_pMesh = FBXLoader->getMesh();


	//
	//// We need to extract the material properties and texture names from the 
	//// pD3DXMtrlBuffer
	////D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	//m_pMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	//if (m_pMaterials == NULL)
	//	return E_OUTOFMEMORY;
	//m_textures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
	//if (m_textures == NULL)
	//	return E_OUTOFMEMORY;

	//for (DWORD i = 0; i < m_dwNumMaterials; i++)
	//{
	//	// Copy the material
	//	//m_pMaterials[i] = d3dxMaterials[i].MatD3D;

	//	// Set the ambient color for the material (D3DX does not do this)
	//	//m_pMaterials[i].Ambient = m_pMaterials[i].Diffuse;

	//	m_textures[i] = NULL;
	//	//if (d3dxMaterials[i].pTextureFilename != NULL &&
	//	//	lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
	//	//{
	//		// Create the texture
	//		if (FAILED(D3DXCreateTextureFromFileA(RES->getDeivce(),
	//			FBXLoader->getTextureName(i),
	//			&m_textures[i])))
	//		{
	//			MessageBox(NULL, L"텍스쳐를 찾을 수 없슴당", L"Error", MB_OK);
	//		}
	//	//}
	//}

	//// Done with the material buffer
	////pD3DXMtrlBuffer->Release();
	//FBXLoader->CleanupFbxManager();

	return S_OK;
}

HRESULT MeshObject::CreateX(char* filename)
{
	LPD3DXBUFFER pD3DXMtrlBuffer;


	// Load the mesh from the specified file
	if (FAILED(D3DXLoadMeshFromXA(filename, D3DXMESH_SYSTEMMEM,
		RES->getDeivce(), NULL,
		&pD3DXMtrlBuffer, NULL, &m_dwNumMaterials,
		&m_pMesh)))
	{
		MessageBox(NULL, L"메시를 찾을 수 없습당", L"Error", MB_OK);
		return E_FAIL;
	}

	// We need to extract the material properties and texture names from the 
	// pD3DXMtrlBuffer
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMaterials = new D3DMATERIAL9[m_dwNumMaterials];
	if (m_pMaterials == NULL)
		return E_OUTOFMEMORY;
	m_textures = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];
	if (m_textures == NULL)
		return E_OUTOFMEMORY;

	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		// Copy the material
		m_pMaterials[i] = d3dxMaterials[i].MatD3D;

		// Set the ambient color for the material (D3DX does not do this)
		m_pMaterials[i].Ambient = m_pMaterials[i].Diffuse;

		m_textures[i] = NULL;
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			// Create the texture
			if (FAILED(D3DXCreateTextureFromFileA(RES->getDeivce(),
				d3dxMaterials[i].pTextureFilename,
				&m_textures[i])))
			{
				MessageBox(NULL, L"텍스쳐를 찾을 수 없슴당", L"Error", MB_OK);
			}
		}
	}

	// Done with the material buffer
	pD3DXMtrlBuffer->Release();

	return S_OK;
}


void MeshObject::Shutdown(){

	if (m_pMaterials != NULL)
		delete[] m_pMaterials;

	if (m_textures)
	{
		for (DWORD i = 0; i < m_dwNumMaterials; i++)
		{
			if (m_textures[i])
				m_textures[i]->Release();
		}
		delete[] m_textures;
	}
	if (m_pMesh != NULL)
		m_pMesh->Release();
}

void MeshObject::SetupMatrix(){
	D3DXMATRIXA16 Setup;
	D3DXMatrixIdentity(&Setup);

	Setup *= worldMatrix * recycleMatrix;

	RES->getDeivce()->SetTransform(D3DTS_WORLD, &Setup);

}


void MeshObject::Translation(float x, float y, float z, bool isStatic){
	D3DXMATRIXA16 moveMatrix;
	D3DXMatrixIdentity(&moveMatrix);

	D3DXMatrixTranslation(&moveMatrix, x, y, z);
	if (isStatic)
		worldMatrix *= moveMatrix;
	else
		recycleMatrix *= moveMatrix;

}

void MeshObject::Scaling(float sx, float sy, float sz, bool isStatic){
	D3DXMATRIXA16 scaleMatrix;
	D3DXMatrixIdentity(&scaleMatrix);

	D3DXMatrixScaling(&scaleMatrix, sx, sy, sz);
	if (isStatic)
		worldMatrix *= scaleMatrix;
	else
		recycleMatrix *= scaleMatrix;

}

void MeshObject::RotationX(float angle, bool isStatic){
	D3DXMATRIXA16 rotationMatrix;
	D3DXMatrixIdentity(&rotationMatrix);

	D3DXMatrixRotationX(&rotationMatrix, angle);
	if (isStatic)
		worldMatrix *= rotationMatrix;
	else
		recycleMatrix *= rotationMatrix;

}

void MeshObject::RotationY(float angle, bool isStatic){
	D3DXMATRIXA16 rotationMatrix;
	D3DXMatrixIdentity(&rotationMatrix);

	D3DXMatrixRotationY(&rotationMatrix, angle);
	if (isStatic)
		worldMatrix *= rotationMatrix;
	else
		recycleMatrix *= rotationMatrix;

}

void MeshObject::RotationZ(float angle, bool isStatic){
	D3DXMATRIXA16 rotationMatrix;
	D3DXMatrixIdentity(&rotationMatrix);

	D3DXMatrixRotationZ(&rotationMatrix, angle);
	if (isStatic)
		worldMatrix *= rotationMatrix;
	else
		recycleMatrix *= rotationMatrix;

}

HRESULT MeshObject::Render(){

	for (DWORD i = 0; i < m_dwNumMaterials; i++)
	{
		// Set the material and texture for this subset
		RES->getDeivce()->SetMaterial(&m_pMaterials[i]);
		RES->getDeivce()->SetTexture(0, m_textures[i]);

		// Draw the mesh subset
		m_pMesh->DrawSubset(i);
	}

	D3DXMatrixIdentity(&recycleMatrix);

	return S_OK;

}