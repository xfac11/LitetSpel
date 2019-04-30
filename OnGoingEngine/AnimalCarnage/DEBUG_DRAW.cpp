#include "DEBUG_DRAW.h"

DEBUG_DRAW::DEBUG_DRAW()
{
	//filip fråga
	this->renderTargetView = nullptr;
	this->depthStencilView = nullptr;

	shader = new SimpleShader();
	if (shader->InitializeShaders())
		return;
	matrixBuffer.initialize(System::getDevice());
}


DEBUG_DRAW::~DEBUG_DRAW()
{
}

void DEBUG_DRAW::Draw(Primitives* shape)
{
	if (shape == nullptr)
		return;
	//else go on draw my primitives! :D
	float colour[4] = { 1,1,1,1 };
	System::getDeviceContext()->ClearRenderTargetView(renderTargetView, colour);
	System::getDeviceContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	const UINT offset = 0;
	const UINT stride = sizeof(Vertex);

	// Set the vertex buffer
	System::getDeviceContext()->IASetVertexBuffers(0, 1, shape->GetVerticesBuffer(), &stride, &offset);
	System::getDeviceContext()->IASetIndexBuffer(shape->GetIndicesBuffer(), DXGI_FORMAT_R32_UINT, 0);
	//lines
	System::getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//matrixBuffer.data.projection = camera.projection;
	//matrixBuffer.data.view = System::
	matrixBuffer.data.world = shape->getWorld();
	matrixBuffer.applyChanges(System::getDevice(), System::getDeviceContext());
	//Set the constant buffer
	System::getDeviceContext()->VSSetConstantBuffers(0, 1, matrixBuffer.getAddressOfBuffer());

	System::getDeviceContext()->IASetInputLayout(this->shader->GetVertexShader()->GetInputLayout());
	// Set the vertex and pixel shaders 
	System::getDeviceContext()->VSSetShader(this->shader->GetVertexShader()->GetShader(), NULL, 0);
	System::getDeviceContext()->PSSetShader(this->shader->GetPixelShader()->GetShader(), NULL, 0);
	// Render the Lines
	System::getDeviceContext()->DrawIndexed(shape->GetIndices(), 0, 0);


}
