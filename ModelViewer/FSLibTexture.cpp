#include "FSLibTexture.h"

TEXTURE::TEXTURE(void) {
	resource = nullptr;
	view = nullptr;
}

TEXTURE::TEXTURE(const wchar_t* fileName) {
	this->Load(fileName);
}

TEXTURE::~TEXTURE(void) {
	if (resource != nullptr)resource->Release();
	if (view != nullptr)view->Release();
}

ID3D11Resource* TEXTURE::GetResource(void) { return resource; }

ID3D11ShaderResourceView* TEXTURE::GetView(void) { return view; }

bool TEXTURE::Load(const wchar_t* fileName) {
	extern ID3D11Device* Device;

	HRESULT hr = CreateWICTextureFromFile(Device, fileName, &resource, &view);
	if (FAILED(hr))return false;
	return true;
}

bool TEXTURE::Draw(VECTOR2D min, VECTOR2D max) {
	if (!(min[0] < max[0] && min[1] < max[1]))return false;

	extern RENDER_INFO RenderInfo;
	extern ID3D11DeviceContext* DeviceContext;
	extern WINDOW_CONTROLLER wndCont;
	extern CONSTANT_MANAGER ConstantManager;

	DeviceContext->PSSetShaderResources(0, 1, &view);

	float wndW = wndCont.GetWidth(), wndH = wndCont.GetHeight();

	XMFLOAT4X4 constant;
	MATRIX trans = MGetTranslate(min[0], min[1], 0.f);
	MATRIX scale = MGetScale(max[0] - min[0], max[1] - min[1], 1.f);
	MATRIX mat = { {
		{ 2.f / wndW, 0.f, 0.f, 0.f , },
		{ 0.f, -2.f / wndH, 0.f, 0.f , },
		{ 0.f, 0.f, 1.f, 0.f , },
		{ -1.f, 1.f, 0.f, 1.f , },
		}, };
	DirectX::XMStoreFloat4x4(&constant, XMMatrixTranspose(XMMATRIX(scale*trans*mat)));

	CONSTANT_BUFFER cb = RenderInfo.GetTextureConstantBuffer();
	bool f = ConstantManager.SetConstant(&cb, &constant);

	ID3D11Buffer* texVertexBuffer = RenderInfo.GetTextureVertexBuffer();
	ID3D11Buffer* texIndexBuffer = RenderInfo.GetTextureIndexBuffer();

	ID3D11InputLayout* texInputLayout = RenderInfo.GetTextureVertexShader().GetInputLayout();
	ID3D11VertexShader* texVertexShader = RenderInfo.GetTextureVertexShader().Get();
	
	ID3D11PixelShader* texPixelShader = RenderInfo.GetTexturePixelShader().Get();
	
	UINT strides = sizeof(VERTEX2D);
	UINT offsets = 0;
	DeviceContext->IASetInputLayout(texInputLayout);//
	DeviceContext->IASetVertexBuffers(0, 1, &texVertexBuffer, &strides, &offsets);
	DeviceContext->IASetIndexBuffer(texIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// ’¸“_‚Ì‰ðŽß‚ÌŽd•û

	DeviceContext->VSSetConstantBuffers(0, 1, &cb.buffer);//
	DeviceContext->VSSetShader(texVertexShader, NULL, 0);//

	//DeviceContext->PSSetConstantBuffers(0, 1, &cb.buffer);
	DeviceContext->PSSetShader(texPixelShader, NULL, 0);

	RenderInfo.Rasterizer()->SetDepthOff();

	DeviceContext->DrawIndexed(6, 0, 0);

	return true;
}
