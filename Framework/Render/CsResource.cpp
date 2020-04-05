#include "Framework.h"

CsResource::CsResource()
	: result(nullptr)
{
}

CsResource::~CsResource()
{
	input->Release();
	srv->Release();

	output->Release();
	uav->Release();

	if(result != nullptr)
		result->Release();
}

void CsResource::CreateBuffer()
{
	CreateInput();
	CreateSRV();

	CreateOutput();
	CreateUAV();

	CreateResult();
}
