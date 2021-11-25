#include "rtepch.h"
#include "PrimitivesBatcher.h"
#include "DebugDraw.h"


std::unique_ptr<DirectX::CommonStates> m_states;
std::unique_ptr<DirectX::BasicEffect> m_effect;
std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;
Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
bool initFlag = false;

void RTE::PrimitivesBatcher::Init(ID3D11Device* device, ID3D11DeviceContext* context, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection)
{
	using namespace D3DUtils;
	RTE_CORE_ASSERT(!initFlag, "PrimitivesBatcher already inited!");
	m_states = std::make_unique<CommonStates>(device);
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

	m_effect = std::make_unique<BasicEffect>(device);
	m_effect->SetVertexColorEnabled(true);
	m_effect->SetView(DirectX::XMLoadFloat4x4(&view));
	m_effect->SetProjection(DirectX::XMLoadFloat4x4(&projection));

	{
		void const* shaderByteCode;
		size_t byteCodeLength;

		m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

		ThrowIfFailed(
			device->CreateInputLayout(
				VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
				shaderByteCode, byteCodeLength,
				m_inputLayout.ReleaseAndGetAddressOf()));
	}
	initFlag = true;
}


void RTE::PrimitivesBatcher::DrawPrimitive(ID3D11DeviceContext* context, BoundingSphere sphere)
{
	/*
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	context->RSSetState(m_states->CullNone());
	*/
	m_effect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	m_batch->Begin();
	Draw(m_batch.get(), sphere, Colors::Blue); // BoundingSphere
	m_batch->End();
}

void RTE::PrimitivesBatcher::DrawGrid(ID3D11DeviceContext* context)
{
	/*
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	context->RSSetState(m_states->CullNone());
	*/
	m_effect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	m_batch->Begin();
	RTE::DrawGrid(m_batch.get(), XMVectorSet(100, 0, 0, 0), XMVectorSet(0, 0, 100, 0), XMVectorSet(0, 0, 0, 0), 100, 100);
	m_batch->End();

}

void RTE::PrimitivesBatcher::DrawPrimitive(ID3D11DeviceContext* context, BoundingBox box)
{
	/*
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthDefault() , 0);
	context->RSSetState(m_states->CullCounterClockwise() );
	*/
	m_effect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	m_batch->Begin();
	Draw(m_batch.get(), box, Colors::Blue); // BoundingOrientedBox
	m_batch->End();
}

void RTE::PrimitivesBatcher::DrawPrimitive(ID3D11DeviceContext* context, BoundingOrientedBox orientedBox)
{
	/*
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	context->RSSetState(m_states->CullNone());
	*/
	m_effect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	m_batch->Begin();
	Draw(m_batch.get(), orientedBox, Colors::Blue); // BoundingOrientedBox
	m_batch->End();
}

void RTE::PrimitivesBatcher::DrawPrimitive(ID3D11DeviceContext* context, BoundingFrustum frustum)
{
	/*
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthNone(), 0);
	context->RSSetState(m_states->CullNone());
	*/
	m_effect->Apply(context);
	context->IASetInputLayout(m_inputLayout.Get());
	m_batch->Begin();
	Draw(m_batch.get(), frustum, Colors::Blue); // BoundingFrustum
	m_batch->End();
}

void RTE::PrimitivesBatcher::SetViewProjection(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 projection)
{
	using namespace D3DUtils;
	m_effect->SetView(DirectX::XMLoadFloat4x4(&view));
	m_effect->SetProjection(DirectX::XMLoadFloat4x4(&projection));
}


