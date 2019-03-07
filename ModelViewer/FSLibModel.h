#pragma once
#include "FSLibStructure.h"
#include "FSLibShader.h"
#include "FSLibCamera.h"
#include "FSLibTexture.h"

class MODEL {
private:
	// �\�z���
	size_t indexNum;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constantBuffer;
	UINT constantBufferSize;
	VERTEX_SHADER* vertexShader;
	PIXEL_SHADER* pixelShader;

	TEXTURE* baseColorMap;
	TEXTURE* normalMap;
	TEXTURE* roughnessMap;
	TEXTURE* metallicMap;
	TEXTURE* heightMap;

	// ��� �O����z+
	XMVECTOR rotation, position; // rotation:�N�I�[�^�j�I��
	XMMATRIX worldMatrix;

public:
	/* �\�z */
	MODEL(void);
	MODEL(char* fileName);
	MODEL(const VERTEX* vert, size_t vertNum, const WORD* index, size_t indexNum);

	void Load(const VERTEX* vert, size_t vertNum, const WORD* index, size_t indexNum);
	void FsmLoad(char* fileName);
	void Save(char* fileName, const VERTEX* vert, size_t vertNum, const WORD* index, size_t indexNum);
	size_t GetIndexNum(void) { return indexNum; }
	ID3D11Buffer* GetVertexBuffer(void) { return vertexBuffer; }
	ID3D11Buffer* GetIndexBuffer(void) { return indexBuffer; }
	ID3D11Buffer* GetConstantBuffer(void) { return constantBuffer; }
	void SetConstantBuffer(ID3D11Buffer* constBuffer, size_t bufferSize) {
		constantBuffer = constBuffer;
		constantBufferSize = bufferSize;
	}
	UINT GetConstantBufferSize(void) { return constantBufferSize; }

	void SetVertexShader(VERTEX_SHADER* vertShader){ vertexShader = vertShader; }
	VERTEX_SHADER* GetVertexShader(void) { return vertexShader; }
	void SetPixelShader(PIXEL_SHADER* pixShader){ pixelShader = pixShader; }
	PIXEL_SHADER* GetPixelShader(void) { return pixelShader; }

	XMMATRIX GetWorldMatrix_XM(void) { return worldMatrix; }
	MATRIX GetWorldMatrix(void) { return XMMatToMat(worldMatrix); }

	void SetBaseColorMap(TEXTURE* tex);
	void SetNormalMap(TEXTURE* tex);
	void SetRoughnessMap(TEXTURE* tex);
	void SetMetallicMap(TEXTURE* tex);
	void SetHeightMap(TEXTURE* tex);

	/* ��� */
	void SetPosition(VECTOR&);
	VECTOR GetPosition(void);
	void SetRotation(VECTOR&); // ��]������ZXY ���[�s�b�`���[
	VECTOR GetRotation(void);

	/* ���� */
	bool Render(void);
};

