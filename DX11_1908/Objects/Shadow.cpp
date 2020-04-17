#include "Framework.h"

Shadow::Shadow()
	: width(Device::Get()->GetWidth()), height(Device::Get()->GetHeight()), radius(50)
{
	depthTarget = new RenderTarget(width, height);
	depthStencil = new DepthStencil(width, height);

	depthRender = new Render2D();
	depthRender->position = Vector3(200, 200, 0);
	depthRender->scale = Vector3(400, 400, 1);

	//depthRender->SetSRV(depthTarget->GetSRV());	
	depthRender->SetSRV(depthStencil->GetSRV());

	vpBuffer = new ViewProjectionBuffer();
	shadowBuffer = new ShadowBuffer();
	shadowBuffer->data.mapSize = Float2(width, height);

	depthSampler = new SamplerState();
	depthSampler->ComparisonFunc(D3D11_COMPARISON_LESS_EQUAL);
}

Shadow::~Shadow()
{
	delete depthTarget;
	delete depthStencil;

	delete depthRender;	
	delete vpBuffer;
	delete shadowBuffer;

	delete depthSampler;
}

void Shadow::PreRender()
{
	depthTarget->Set(depthStencil);
	SetVP();		
}

void Shadow::PostRender()
{
	depthRender->Update();
	depthRender->Render();

	ImGui::SliderFloat("ShadowBias", &shadowBuffer->data.bias, -0.001f, 0.001f, "%.4f");
	ImGui::SliderInt("ShadowQuality", &shadowBuffer->data.quality, 0, 2);
	ImGui::SliderFloat("ShadowRadius", &radius, 0.1f, 200);
}

void Shadow::SetShadowBuffer()
{	
	DC->PSSetShaderResources(4, 1, &depthTarget->GetSRV());
	//DC->PSSetShaderResources(4, 1, &depthStencil->GetSRV());
	shadowBuffer->SetPSBuffer(2);
	depthSampler->Set(10);
}

void Shadow::SetVP()
{	
	Vector3 lightDir = LIGHT->data.direction;
	lightDir.Normalize();
	Vector3 lightPos = lightDir * -radius;		
	
	float rotX = atan2(lightDir.GetX(), lightDir.GetY());
	float rotY = atan2(lightDir.GetZ(), lightDir.GetX());

	Matrix mat = XMMatrixRotationRollPitchYaw(rotX, rotY, 0.0f) * XMMatrixRotationX(-XM_PIDIV2);

	Vector4 up = XMVector3TransformNormal(kUp, mat);	

	Matrix V, P;
	V = XMMatrixLookAtLH(lightPos.data, XMVectorZero(), up);			
	//P = XMMatrixPerspectiveFovLH(XM_PIDIV2, 1, 0.1f, 1000.0f);	
	P = XMMatrixOrthographicLH(radius * 2, radius * 2, -radius, radius);
	
	vpBuffer->SetView(V);
	vpBuffer->SetProjection(P);
	vpBuffer->SetVSBuffer(0);
	vpBuffer->SetVSBuffer(4);
}
