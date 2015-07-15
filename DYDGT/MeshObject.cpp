#include "MeshObject.h"
#include "ResourceClass.h"
//#include "MeshLoader.h"

struct meshVertex{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

MeshObject::MeshObject()
{
	strcpy_s(m_strName, "");
	m_pMesh = NULL;
	m_pMaterials = NULL;
	m_textures = NULL;
	m_dwNumMaterials = NULL;
	m_bUseMaterials = true;

	minPoint = maxPoint = D3DXVECTOR3(0, 0, 0);

	identity();


}

FbxNodeAttribute* findNode(FbxNode* node)
{
	for (int i = 0; i != node->GetNodeAttributeCount(); ++i)
	{
		if (node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			return node->GetNodeAttributeByIndex(i);
		}
	}
	for (int i = 0; i != node->GetChildCount(); ++i)
	{
		return findNode(node->GetChild(i));
	}
	return 0;
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
	FbxManager* fbxMgr = 0;
	FbxScene*	fbxScene = 0;
	{
		fbxMgr = FbxManager::Create();
		if (!fbxMgr)
			throw runtime_error("Unable to create the FBX SDK manager");

		FbxIOSettings* fbxIOSetting = FbxIOSettings::Create(fbxMgr, IOSROOT);
		fbxMgr->SetIOSettings(fbxIOSetting);

		//FbxString path = FbxGetApplicationDirectory();
		//fbxMgr->LoadPluginsDirectory( path.Buffer() );

		fbxScene = FbxScene::Create(fbxMgr, "MyScene");

		FbxImporter* importer = FbxImporter::Create(fbxMgr, "");

		importer->Initialize(FBXfilename, -1, fbxMgr->GetIOSettings());
		importer->Import(fbxScene);
		importer->Destroy();
		fbxIOSetting->Destroy();
	}

	FbxNode* rootNode = fbxScene->GetRootNode();

	FbxGeometryConverter converter(fbxMgr);

	converter.TriangulateInPlace(rootNode);

	FbxNodeAttribute* attr = findNode(rootNode);

	this->loadMeshFromFbxNodeAttribute(attr);
	rootNode->Destroy();


	fbxScene->Destroy();
	fbxMgr->Destroy();

	this->generateBoundingBox();

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

void MeshObject::loadMeshFromFbxNodeAttribute(FbxNodeAttribute* attr)
{
	FbxMesh* mesh = (FbxMesh*)attr;

	//mesh->get
	//
	{
		DWORD numVertices = mesh->GetPolygonVertexCount();//mesh->GetControlPointsCount();
		DWORD numFaces = mesh->GetPolygonVertexCount() / 3;

		D3DVERTEXELEMENT9 decl[] =
		{
			{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },

			{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },

			{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		D3DXCreateMesh(numFaces, numVertices, D3DXMESH_MANAGED, decl, RES->getDeivce(), &this->m_pMesh);

		//FbxVector4* vertexBuffer = 0;

		WORD* indices;

		m_pMesh->LockIndexBuffer(0, (void**)&indices);

		for (int i = 0; i != numVertices; ++i)
		{
			indices[i] = i;
		}

		m_pMesh->UnlockIndexBuffer();

		meshVertex* vertexBuffer = 0;

		this->m_pMesh->LockVertexBuffer(0, (void**)&vertexBuffer);

		int* vert = mesh->GetPolygonVertices();
		for (unsigned int i = 0; i != mesh->GetPolygonVertexCount(); ++i)	//
		{
			//
			vertexBuffer[i].x = mesh->GetControlPointAt(vert[i]).mData[0];
			vertexBuffer[i].y = mesh->GetControlPointAt(vert[i]).mData[2];
			vertexBuffer[i].z = mesh->GetControlPointAt(vert[i]).mData[1];

			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(i / 3, i % 3, normal);
			vertexBuffer[i].nx = normal.mData[0];
			vertexBuffer[i].ny = normal.mData[2];
			vertexBuffer[i].nz = normal.mData[1];

			FbxStringList uvsetNames;
			mesh->GetUVSetNames(uvsetNames);
			const char* name = uvsetNames[0];

			FbxVector2 uv;
			mesh->GetPolygonVertexUV(i / 3, i % 3, name, uv);

			vertexBuffer[i].u = uv.mData[0];
			vertexBuffer[i].v = uv.mData[1];
		}

		this->m_pMesh->UnlockVertexBuffer();

		DWORD* attributeBuffer;

		m_pMesh->LockAttributeBuffer(0, &attributeBuffer);

		attributeBuffer[0] = 0;
		attributeBuffer[1] = 0;

		m_pMesh->UnlockAttributeBuffer();
	}

	FbxNode* mNode = attr->GetNode();

	FbxSurfaceMaterial* mat = mNode->GetMaterial(0);

	D3DMATERIAL9 mtrl = { 0.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 0.0, 1.0,
		0.0, 0.0, 0.0, 1.0 };


	if (mat->GetClassId() == FbxSurfacePhong::ClassId)
	{
		FbxSurfacePhong* phong = static_cast< FbxSurfacePhong* >(mat);
		FbxDouble3 specular = phong->Specular.Get();
		mtrl.Specular.r = specular.mData[0];
		mtrl.Specular.g = specular.mData[1];
		mtrl.Specular.b = specular.mData[2];
		mtrl.Specular.a = 1.0;

		FbxDouble3 diffuse = phong->Diffuse.Get();
		mtrl.Diffuse.r = diffuse.mData[0];
		mtrl.Diffuse.g = diffuse.mData[1];
		mtrl.Diffuse.b = diffuse.mData[2];
		mtrl.Diffuse.a = 1.0;

		FbxDouble3 ambient = phong->Ambient.Get();
		mtrl.Ambient.r = ambient.mData[0];
		mtrl.Ambient.g = ambient.mData[1];
		mtrl.Ambient.b = ambient.mData[2];
		mtrl.Ambient.a = 1.0;

		//::MessageBoxA( 0, phong->Diffuse.GetDstObject( )->GetClassId().GetName(), 0, 0 );
	}
	else if (mat->GetClassId() == FbxSurfaceLambert::ClassId)
	{
		FbxSurfaceLambert* lambert = static_cast< FbxSurfaceLambert * >(mat);
		FbxDouble3 diffuse = lambert->Diffuse.Get();
		mtrl.Diffuse.r = diffuse.mData[0];
		mtrl.Diffuse.g = diffuse.mData[1];
		mtrl.Diffuse.b = diffuse.mData[2];
		mtrl.Diffuse.a = 1.0;

		FbxDouble3 ambient = lambert->Ambient.Get();
		mtrl.Ambient.r = ambient.mData[0];
		mtrl.Ambient.g = ambient.mData[1];
		mtrl.Ambient.b = ambient.mData[2];
		mtrl.Ambient.a = 1.0;

	}
	else
	{
		throw runtime_error("mesh！");
	}

	this->m_pMeterials2.push_back(mtrl);

	FbxProperty prop = mat->FindProperty(FbxSurfaceMaterial::sDiffuse);

	if (prop.GetSrcObjectCount(FbxLayeredTexture::ClassId) > 0)		//
	{	//
		//prop.
		FbxLayeredTexture *lLayeredTexture = FbxCast <FbxLayeredTexture>(prop.GetSrcObject(FbxLayeredTexture::ClassId));
		int lNbTextures = lLayeredTexture->GetSrcObjectCount(FbxTexture::ClassId);
		FbxTexture* fbxTex = static_cast< FbxTexture* >(lLayeredTexture->GetSrcObject(FbxTexture::ClassId));
		//Textures.push_back(createTextureFromFile( fbxTex->GetFileName() ));
	}
	else if (prop.GetSrcObjectCount(FbxFileTexture::ClassId) > 0)
	{
		FbxFileTexture *fileTexture = FbxCast <FbxFileTexture>(prop.GetSrcObject(FbxFileTexture::ClassId));
		//MessageBoxA( 0, fileTexture->GetFileName(), 0, 0 );
		m_textures2.push_back(createTextureFromFile(fileTexture->GetFileName()));
	}
	else if (prop.GetSrcObjectCount(FbxProceduralTexture::ClassId) > 0)
	{//
		FbxProceduralTexture *lLayeredTexture = FbxCast <FbxProceduralTexture>(prop.GetSrcObject(FbxProceduralTexture::ClassId));
		int lNbTextures = lLayeredTexture->GetSrcObjectCount(FbxTexture::ClassId);
		FbxTexture* fbxTex = static_cast< FbxTexture* >(lLayeredTexture->GetSrcObject(FbxTexture::ClassId));
		MessageBoxA(0, fbxTex->GetName(), 0, 0);

	}
	else if (prop.GetSrcObjectCount(FbxTexture::ClassId) > 0)
	{//
		FbxTexture *lLayeredTexture = FbxCast <FbxTexture>(prop.GetSrcObject(FbxTexture::ClassId));
		int lNbTextures = lLayeredTexture->GetSrcObjectCount(FbxTexture::ClassId);
		FbxTexture* fbxTex = static_cast< FbxTexture* >(lLayeredTexture->GetSrcObject(FbxTexture::ClassId));
		MessageBoxA(0, fbxTex->GetName(), 0, 0);
	}
	else
		m_textures2.push_back(0);

	mesh->Destroy();


	this->generateBoundingBox();
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

D3DXVECTOR3 MeshObject::getPosition(){
	return D3DXVECTOR3(worldMatrix._41, worldMatrix._42, worldMatrix._43);
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

void MeshObject::addChild(MeshObject* child){
	m_child.push_back(child);
}

void MeshObject::addChild(MeshObject* child, int tag){

}

IDirect3DTexture9* MeshObject::createTextureFromFile(const char* fileName)
{
	IDirect3DTexture9* texture;
	D3DXCreateTextureFromFileA(RES->getDeivce(),
		fileName,
		&texture);
	return texture;
}