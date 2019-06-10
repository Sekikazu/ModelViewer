#include "RenderInfo.h"
#include "resources\Shader\textureVS.h"
#include "resources\Shader\texturePS.h"

RENDER_INFO::RENDER_INFO(void) {
	texVertexShader = VERTEX_SHADER();
	texPixelShader = PIXEL_SHADER();
}

RENDER_INFO::~RENDER_INFO(void) {
	//sampler->Release();
}

void RENDER_INFO::Init(void) {
	extern ID3D11Device* Device;
	extern ID3D11DeviceContext* DeviceContext;
	extern CONSTANT_MANAGER ConstantManager;

	// Rasterizer
	rasterizer.Init();

	// Sampler
	{
		sampler = nullptr;

		D3D11_SAMPLER_DESC smpDesc;
		ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
		smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;// CLAMP;
		smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;// CLAMP;
		smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;// CLAMP;
		Device->CreateSamplerState(&smpDesc, &sampler);
		DeviceContext->PSSetSamplers(0, 1, &sampler);
	}

	// Texture
	//// Shader
	{
		D3D11_INPUT_ELEMENT_DESC ied[2] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		texVertexShader = VERTEX_SHADER(g_vs_texture, sizeof(g_vs_texture), ied, 2);
		texPixelShader = PIXEL_SHADER(g_ps_texture, sizeof(g_ps_texture));
	}
	//// VertexBuffer
	{
		VERTEX2D texVert[4] = {
			{ { 0.f, 0.f, 0.5f } ,{ 0.f, 0.f } },
			{ { 1.f, 0.f, 0.5f },{ 1.f, 0.f } },
			{ { 0.f, 1.f, 0.5f },{ 0.f, 1.f } },
			{ { 1.f, 1.f, 0.5f },{ 1.f, 1.f } },
		};
		texVertexBuffer = nullptr;
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.ByteWidth = 4 * sizeof(VERTEX2D);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subResourceData;
		subResourceData.pSysMem = texVert;
		subResourceData.SysMemPitch = 0;
		subResourceData.SysMemSlicePitch = 0;

		Device->CreateBuffer(&bufferDesc, &subResourceData, &texVertexBuffer);
	}
	//// IndexBuffer
	{
		WORD index[6] = { 0, 1, 2, 3, 2, 1 };
		texIndexBuffer = nullptr;

		D3D11_BUFFER_DESC ibDesc;
		ibDesc.ByteWidth = sizeof(WORD) * 6;
		ibDesc.Usage = D3D11_USAGE_DEFAULT;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA irData;
		irData.pSysMem = &index[0];
		irData.SysMemPitch = 0;
		irData.SysMemSlicePitch = 0;

		Device->CreateBuffer(&ibDesc, &irData, &texIndexBuffer);
	}
	//// ConstantBuffer
	{
		//XMMATRIX mat;
		//ZeroMemory(&mat, sizeof(XMMATRIX));
		texConstantBuffer = ConstantManager.CreateConstantBuffer(sizeof(XMMATRIX));
	}
}

bool RENDER_INFO::SetSampler(D3D11_SAMPLER_DESC& smpDesc) {
	return false; // TODO
}