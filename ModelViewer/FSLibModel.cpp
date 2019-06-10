#include "FSLibModel.h"
#include "FSLibFile.h"

MODEL::MODEL(void) {
	baseColorMap = nullptr;
	normalMap = nullptr;
	roughnessMap = nullptr;
	metallicMap = nullptr;
	heightMap = nullptr;

	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	worldMatrix = XMMatrixIdentity();
}

MODEL::MODEL(const VERTEX* vert, size_t vertNum, const WORD* index, size_t idxNum) {
	baseColorMap = nullptr;
	normalMap = nullptr;
	roughnessMap = nullptr;
	metallicMap = nullptr;
	heightMap = nullptr;

	extern ID3D11Device* Device;

	// VertexBuffer
	{
		vertexBuffer = nullptr;
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.ByteWidth = vertNum * sizeof(VERTEX);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subResourceData;
		subResourceData.pSysMem = &vert[0];
		subResourceData.SysMemPitch = 0;
		subResourceData.SysMemSlicePitch = 0;

		Device->CreateBuffer(&bufferDesc, &subResourceData, &vertexBuffer);
	}

	// IndexBuffer
	indexBuffer = nullptr;
	{
		indexNum = idxNum;
		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * indexNum;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = &index[0];
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		Device->CreateBuffer(&ibDesc, &irData, &indexBuffer);
	}

	vertexShader = nullptr;
	pixelShader = nullptr;

	worldMatrix = XMMatrixTranslation(0.f, 0.f, 0.f);
}

MODEL::MODEL(char* fileName) {
	baseColorMap = nullptr;
	normalMap = nullptr;
	roughnessMap = nullptr;
	metallicMap = nullptr;
	heightMap = nullptr;

	FILE_EXTENSION fileType;
	{
		FILE_MANAGER fm(fileName, "rb");
		fileType = fm.Type();
	}

	if (fileType == FILE_EXTENSION_FSM) {
		FsmLoad(fileName);
	}

	// fbxのロード
	if (fileType == FILE_EXTENSION_FBX) {
		extern FbxManager* fbxManager;
		FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxscene");
		FbxString FileName(fileName);
		FbxImporter *fbxImporter = FbxImporter::Create(fbxManager, "imp");
		fbxImporter->Initialize(FileName.Buffer(), -1, fbxManager->GetIOSettings());
		fbxImporter->Import(fbxScene);
		fbxImporter->Destroy();

		FbxGeometryConverter geometryConverter(fbxManager);
		geometryConverter.Triangulate(fbxScene, true);

		//FbxAxisSystem::DirectX.ConvertScene(fbxScene);

		// メッシュの取得
		FbxMesh* mesh = nullptr;
		for (int i = 0; i < fbxScene->GetRootNode()->GetChildCount(); i++) {
			if (fbxScene->GetRootNode()->GetChild(i)->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
				mesh = fbxScene->GetRootNode()->GetChild(i)->GetMesh();
				break;
			}
		}

		// インデックスの取得
		indexNum = mesh->GetPolygonVertexCount();
		vector<WORD> index(indexNum);
		for (int i = 0; i < indexNum; i++)
			index[i] = mesh->GetPolygonVertices()[i];

		// 頂点の取得
		size_t vertNum = mesh->GetControlPointsCount();
		//// 座標
		vector<VERTEX> vertices(vertNum);
		for (int i = 0; i < vertNum; i++) {
			vertices[i].pos[0] = (FLOAT)mesh->GetControlPointAt(i)[0];
			vertices[i].pos[1] = (FLOAT)mesh->GetControlPointAt(i)[2];
			vertices[i].pos[2] = (FLOAT)mesh->GetControlPointAt(i)[1];
		}

		//// 法線
		int normalLayerCount = mesh->GetElementNormalCount();	// 法線セット数
		for (int i = 0; i < normalLayerCount; i++) {
			// 法線セット
			FbxGeometryElementNormal* normal = mesh->GetElementNormal(i);

			// マッピングモード 法線がメッシュ表面にどのように定義されているか
			FbxGeometryElement::EMappingMode mappingMode = normal->GetMappingMode();
			// リファレンスモード データの格納方法
			FbxGeometryElement::EReferenceMode reference = normal->GetReferenceMode();

			switch (mappingMode) {
			case FbxGeometryElement::eByControlPoint:
			{
				switch (reference) {
				case FbxGeometryElement::eDirect: // データは順番に格納
				{
					// 法線数
					int normalCount = normal->GetDirectArray().GetCount();

					for (int a = 0; a < normalCount; a++) {
						vertices[a].norm[0] = (float)normal->GetDirectArray().GetAt(a)[0];
						vertices[a].norm[1] = (float)normal->GetDirectArray().GetAt(a)[1];
						vertices[a].norm[2] = (float)normal->GetDirectArray().GetAt(a)[2];
					}
				}break;

				case FbxGeometryElement::eIndex:
				{
					// TODO
				}break;

				case FbxGeometryElement::eIndexToDirect:
				{
					// TODO
				}break;

				default:break;
				}
			}break;

			case FbxGeometryElement::eByPolygonVertex:
			{
				switch (reference) {
				case FbxGeometryElement::eDirect: // データは順番に格納
				{
					// 法線数
					int normalCount = normal->GetDirectArray().GetCount();

					for (int a = 0; a < normalCount; a++) {
						vertices[index[a]].norm[0] = (float)normal->GetDirectArray().GetAt(a)[0];//-
						vertices[index[a]].norm[1] = (float)normal->GetDirectArray().GetAt(a)[2];
						vertices[index[a]].norm[2] = (float)normal->GetDirectArray().GetAt(a)[1];
					}
				}break;

				case FbxGeometryElement::eIndex:
				{
					// TODO
				}break;

				case FbxGeometryElement::eIndexToDirect:
				{
					// TODO
				}break;

				default:break;
				}
			}break;

			default:break;
			}
		}

		//// UV
		vector<FbxVector2> uvList(0);

		FbxStringList lUVSetNameList;
		mesh->GetUVSetNames(lUVSetNameList);
		for (int i = 0; i < lUVSetNameList.GetCount(); i++) {
			const char* lUVSetName = lUVSetNameList.GetStringAt(i);
			FbxGeometryElementUV* uv = mesh->GetElementUV(lUVSetName);

			if (!uv)continue;

			FbxLayerElement::EMappingMode mappingMode = uv->GetMappingMode();
			FbxLayerElement::EReferenceMode referenceMode = uv->GetReferenceMode();

			const bool lUseIndex = uv->GetReferenceMode() != FbxGeometryElement::eDirect;
			const int lIndexCount = (lUseIndex) ? uv->GetIndexArray().GetCount() : 0;

			switch (mappingMode) {
			case FbxLayerElement::eByPolygonVertex:
			{
				switch (referenceMode) {
				case FbxLayerElement::eDirect:
				{
					// UV数
					int uvCount = uv->GetDirectArray().GetCount();

					for (int a = 0; a < uvCount; a++) {	// TODO:おそらくは違う
						vertices[a].uv.pal[0] = (float)uv->GetDirectArray().GetAt(a)[0];
						vertices[a].uv.pal[1] = (float)uv->GetDirectArray().GetAt(a)[1];
					}
				}break;

				case FbxLayerElement::eIndexToDirect:
				{
					vector<VECTOR2D> uvset(0);

					int uvCount = uv->GetDirectArray().GetCount();
					for (int a = 0; a < uvCount; a++) {
						VECTOR2D uvBuf;

						uvBuf.pal[0] = (float)uv->GetDirectArray().GetAt(a)[0];
						uvBuf.pal[1] = 1.f - (float)uv->GetDirectArray().GetAt(a)[1];

						uvset.push_back(uvBuf);
					}

					int uvIndexCount = uv->GetIndexArray().GetCount();
					for (int a = 0; a < indexNum; a++) {
						int uvIndex = uv->GetIndexArray().GetAt(a);

						vertices[index[a]].uv = uvset[uvIndex];
					}
				}break;

				default:break;
				}
			}break;

			case FbxLayerElement::eByControlPoint:
			{
				switch (referenceMode) {
				case FbxLayerElement::eDirect:
				{
					// TODO:
				}break;

				case FbxLayerElement::eIndexToDirect:
				{
					// TODO:
				}break;

				default:break;
				}
			}break;

			default:break;
			}

		}

		// RH->LHの変換
		for (int i = 0; i < indexNum; i++) {
			if (i % 3 == 0) {
				WORD buf = index[i + 2];
				index[i + 2] = index[i];
				index[i] = buf;
			}
		}

		// タンジェント空間の算出
		vector<POLYGON> poly;
		vector<WORD> indexClone(index);
		VECTOR tangentBuf;
		VECTOR binormalBuf;
		POLYGON pBuf;
		for (int v = 0; v < vertNum; v++) {
			poly.clear();
			poly.shrink_to_fit();

			// 頂点が含まれるポリゴンを取得
			for (int i = 0; i < indexClone.size(); i += 3) {
				for (int a = 0; a < 3; a++) {
					if (v == indexClone[i + a]) {
						pBuf.vert[0] = vertices[indexClone[i + a]];
						pBuf.vert[1] = vertices[indexClone[i + (a + 1) % 3]];
						pBuf.vert[2] = vertices[indexClone[i + (a + 2) % 3]];

						poly.push_back(pBuf);

						// 不必要になったインデックスを削除
						if (indexClone[i] <= v && indexClone[i + 1] <= v && indexClone[i + 2] <= v) {
							indexClone.erase(indexClone.begin() + i, indexClone.begin() + (i + 3));
							i -= 3;
						}

						break;
					}
				}
			}

			tangentBuf = VGet(0.f, 0.f, 0.f);
			binormalBuf = VGet(0.f, 0.f, 0.f);
			for (auto itr = poly.begin(); itr != poly.end(); itr++) {
				CalcTangentAndBinormal(
					&itr->vert[0].pos, &itr->vert[0].uv,
					&itr->vert[1].pos, &itr->vert[1].uv,
					&itr->vert[2].pos, &itr->vert[2].uv,
					&vertices[v].tangent, &vertices[v].binorm);

				tangentBuf += vertices[v].tangent;
				binormalBuf += vertices[v].binorm;
			}
			vertices[v].tangent = VNorm(vertices[v].tangent);
			vertices[v].binorm = VNorm(vertices[v].binorm);
		}

		Save(fileName, &vertices[0], vertNum, &index[0], indexNum);
		Load(&vertices[0], vertNum, &index[0], indexNum);
	}
	
	/*
	int num;
	while (vertices.size() != 1) {
		num = vertices.size();
		for (int i = num - 1; i > 0; i -= 2) {
			vertices[i - 1].pos += vertices[i].pos;
			vertices.erase(vertices.begin() + i);
		}
		vertices.shrink_to_fit();
	}
	vertices[0].pos *= 1.f / (float)vertNum;
	vertices.clear();
	vertices.shrink_to_fit();
	*/
}

void MODEL::Load(const VERTEX* vert, size_t vertNum, const WORD* index, size_t idxNum) {
	extern ID3D11Device* Device;

	// VertexBuffer
	{
		vertexBuffer = nullptr;
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.ByteWidth = vertNum * sizeof(VERTEX);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subResourceData;
		subResourceData.pSysMem = &vert[0];
		subResourceData.SysMemPitch = 0;
		subResourceData.SysMemSlicePitch = 0;

		Device->CreateBuffer(&bufferDesc, &subResourceData, &vertexBuffer);
	}

	// IndexBuffer
	indexBuffer = nullptr;
	{
		indexNum = idxNum;
		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * indexNum;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = &index[0];
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		Device->CreateBuffer(&ibDesc, &irData, &indexBuffer);
	}

	vertexShader = nullptr;
	pixelShader = nullptr;

	worldMatrix = XMMatrixTranslation(0.f, 0.f, 0.f);
}

void MODEL::FsmLoad(char* fileName) {
	FILE_MANAGER fm(fileName, "rb");

	size_t vertNum = 0;
	fread(&vertNum, sizeof(vertNum), 1, fm.Get());

	vector<VERTEX> vertices(vertNum);
	fread(&vertices[0], sizeof(VERTEX), vertNum, fm.Get());

	size_t indexNum = 0;
	fread(&indexNum, sizeof(indexNum), 1, fm.Get());

	vector<WORD> index(indexNum);
	fread(&index[0], sizeof(WORD), indexNum, fm.Get());

	Load(&vertices[0], vertNum, &index[0], indexNum);
}

void MODEL::Save(char* fileName, const VERTEX* vert, size_t vertNum, const WORD* index, size_t indexNum) {
	vector<char> name;
	{
		int i = 0;
		while (fileName[i] != '.' && fileName[i] != '\0') {
			name.push_back(fileName[i]);

			++i;
		}

		char add[5] = ".fsm";
		for (int a = 0; a < 5; a++) {
			name.push_back(add[a]);
		}
	}

	FILE* fp;
	fopen_s(&fp, &name[0], "wb");
	
	// vertNum
	fwrite(&vertNum, sizeof(vertNum), 1, fp);

	// vert
	fwrite(vert, sizeof(VERTEX), vertNum, fp);

	// indexNum
	fwrite(&indexNum, sizeof(indexNum), 1, fp);

	// index
	fwrite(index, sizeof(WORD), indexNum, fp);

	fclose(fp);
}

void MODEL::SetBaseColorMap(TEXTURE* tex) { baseColorMap = tex; }
void MODEL::SetNormalMap(TEXTURE* tex) { normalMap = tex; }
void MODEL::SetRoughnessMap(TEXTURE* tex) { roughnessMap = tex; }
void MODEL::SetMetallicMap(TEXTURE* tex) { metallicMap = tex; }
void MODEL::SetHeightMap(TEXTURE* tex) { heightMap = tex; }

bool MODEL::Render(void) {
	extern ID3D11DeviceContext* DeviceContext;
	extern CAMERA Camera;

	UINT strides = sizeof(VERTEX);
	UINT offsets = 0;
	DeviceContext->IASetInputLayout(vertexShader->GetInputLayout());//
	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);		// 複数モデルに対応
	DeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の解釈の仕方
	DeviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);//
	DeviceContext->VSSetShader(vertexShader->Get(), NULL, 0);//

	DeviceContext->PSSetConstantBuffers(0, 1, &constantBuffer);
	DeviceContext->PSSetShader(pixelShader->Get(), NULL, 0);

	// テクスチャ
	ID3D11ShaderResourceView* psResourcesView[5] = { nullptr };
	if(baseColorMap != nullptr)psResourcesView[0] = baseColorMap->GetView();
	if(normalMap != nullptr)psResourcesView[1] = normalMap->GetView();
	if(roughnessMap != nullptr)psResourcesView[2] = roughnessMap->GetView();
	if(metallicMap !=  nullptr)psResourcesView[3] = metallicMap->GetView();
	if(heightMap != nullptr)psResourcesView[4] = heightMap->GetView();

	DeviceContext->PSSetShaderResources(0, 5, psResourcesView);

	DeviceContext->DrawIndexed(indexNum, 0, 0);

	return true;
}

void MODEL::SetPosition(VECTOR& pos) {
	position = (XMVECTOR)pos;
	XMMATRIX mPos = XMMatrixTranslationFromVector(position);
	XMMATRIX mRot = XMMatrixRotationQuaternion(rotation);
	worldMatrix = XMMatrixMultiply(mPos, mRot);
}

VECTOR MODEL::GetPosition(void) {
	return VGet(position);
}

void MODEL::SetRotation(VECTOR& rot) {
	rotation = (XMVECTOR)rot;
	XMMATRIX mPos = XMMatrixTranslationFromVector(position);
	XMMATRIX mRot = XMMatrixRotationQuaternion(rotation);
	worldMatrix = XMMatrixMultiply(mPos, mRot);
}

VECTOR MODEL::GetRotation(void) {
	return VGet(rotation);
}
