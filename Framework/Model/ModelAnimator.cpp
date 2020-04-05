#include "Framework.h"

ModelAnimator::ModelAnimator(wstring shaderFile)
	: texture(nullptr), srv(nullptr), shader(shaderFile)
{
	model = new Model();	

	frameBuffer = new FrameBuffer();

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		worlds[i] = XMMatrixIdentity();

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE, true);
}

ModelAnimator::~ModelAnimator()
{
	delete model;
	delete frameBuffer;
	delete[] clipTransform;

	texture->Release();
	srv->Release();

	for (Transform* transform : transforms)
		delete transform;

	delete instanceBuffer;
}

void ModelAnimator::Update()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		TweenDesc& tweenDesc = frameBuffer->data.tweenDesc[i];		

		{//현재 애니메이션	
			KeyFrameDesc& desc = tweenDesc.cur;
			ModelClip* clip = model->ClipByIndex(desc.clip);

			float time = 1.0f / clip->FrameRate() / desc.speed;
			desc.runningTime += DELTA;

			if (desc.time >= 1.0f)
			{
				desc.runningTime = 0.0f;

				if (desc.curFrame + desc.time >= clip->FrameCount())
				{
					if (EndEvent.count(desc.clip) != 0)
						EndEvent[desc.clip]();
				}

				desc.curFrame = (desc.curFrame + (UINT)desc.time) % clip->FrameCount();
				desc.nextFrame = (desc.curFrame + 1) % clip->FrameCount();				
			}
			desc.time = desc.runningTime / time;
		}

		{//다음 애니메이션
			KeyFrameDesc& desc = tweenDesc.next;

			if (desc.clip > -1)
			{
				ModelClip* clip = model->ClipByIndex(desc.clip);

				tweenDesc.runningTime += DELTA;
				tweenDesc.tweenTime = tweenDesc.runningTime / tweenDesc.takeTime;

				if (tweenDesc.tweenTime >= 1.0f)
				{
					tweenDesc.cur = desc;
					tweenDesc.runningTime = 0.0f;
					tweenDesc.tweenTime = 0.0f;

					desc.runningTime = 0.0f;
					desc.curFrame = 0;
					desc.nextFrame = 0;
					desc.time = 0.0f;
					desc.clip = -1;					
				}
				else
				{
					float time = 1.0f / clip->FrameRate() / desc.speed;
					desc.runningTime += DELTA;

					if (desc.time >= 1.0f)
					{
						desc.runningTime = 0.0f;

						desc.curFrame = (desc.curFrame + 1) % clip->FrameCount();
						desc.nextFrame = (desc.curFrame + 1) % clip->FrameCount();						
					}
					desc.time = desc.runningTime / time;
				}
			}
		}
	}

	UpdateTransforms();

	for (ModelMesh* mesh : *model->GetMeshes())
		mesh->Update();
}

void ModelAnimator::Render()
{
	if (texture == nullptr)
		CreateTexture();

	instanceBuffer->Set(1);

	worldBuffer->SetVSBuffer(1);
	frameBuffer->SetVSBuffer(3);		

	for (ModelMesh* mesh : *model->GetMeshes())
		mesh->Render(transforms.size());
}

void ModelAnimator::ReadMaterial(string file)
{
	model->ReadMaterial(file);
}

void ModelAnimator::ReadMesh(string file)
{
	model->ReadMesh(file);

	for (ModelMesh* mesh : *model->GetMeshes())
		mesh->SetShader(shader);
}

void ModelAnimator::ReadClip(string file)
{
	model->ReadClip(file);
}

void ModelAnimator::PlayClip(UINT instance, UINT clip, float speed, float takeTime)
{
	frameBuffer->data.tweenDesc[instance].takeTime = takeTime;
	frameBuffer->data.tweenDesc[instance].next.clip = clip;
	frameBuffer->data.tweenDesc[instance].next.speed = speed;
}

Transform* ModelAnimator::AddTransform()
{
	Transform* transform = new Transform();
	transforms.push_back(transform);

	return transform;
}

void ModelAnimator::UpdateTransforms()
{
	for (UINT i = 0; i < transforms.size(); i++)
	{
		transforms[i]->UpdateWorld();
		//memcpy(&worlds[i], &transforms[i]->GetWorld(), sizeof(Matrix));
		memcpy(&worlds[i], &XMMatrixTranspose(transforms[i]->GetWorld()), sizeof(Matrix));
	}

	D3D11_MAPPED_SUBRESOURCE subResource;
	DC->Map(instanceBuffer->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, worlds, MAX_MODEL_INSTANCE * sizeof(Matrix));
	}
	DC->Unmap(instanceBuffer->GetBuffer(), 0);
}

void ModelAnimator::CreateTexture()
{
	UINT clipCount = model->GetClips()->size();

	clipTransform = new ClipTransform[clipCount];
	for (UINT i = 0; i < clipCount; i++)
		CreateClipTransform(i);

	{//Create Texture
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = MAX_MODEL_BONE * 4;
		desc.Height = MAX_MODEL_KEY;
		desc.ArraySize = clipCount;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		UINT pageSize = MAX_MODEL_BONE * 4 * 16 * MAX_MODEL_KEY;

		void* p = VirtualAlloc(nullptr, pageSize * clipCount, MEM_RESERVE, PAGE_READWRITE);

		for (UINT c = 0; c < clipCount; c++)
		{
			UINT start = c * pageSize;

			for (UINT y = 0; y < MAX_MODEL_KEY; y++)
			{
				void* temp = (BYTE*)p + MAX_MODEL_BONE * y * sizeof(Matrix) + start;

				VirtualAlloc(temp, MAX_MODEL_BONE * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
				memcpy(temp, clipTransform[c].transform[y], MAX_MODEL_BONE * sizeof(Matrix));
			}
		}

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];
		for (UINT c = 0; c < clipCount; c++)
		{
			void* temp = (BYTE*)p + c * pageSize;

			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_BONE * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
		}
		V(DEVICE->CreateTexture2D(&desc, subResource, &texture));

		delete[] subResource;
		VirtualFree(p, 0, MEM_RELEASE);
	}

	{//Create SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = clipCount;

		V(DEVICE->CreateShaderResourceView(texture, &desc, &srv));
	}

	for (ModelMesh* mesh : *model->GetMeshes())
		mesh->SetTransformsSRV(srv);
}

void ModelAnimator::CreateClipTransform(UINT index)
{
	Matrix* bones = new Matrix[MAX_MODEL_BONE];

	ModelClip* clip = model->ClipByIndex(index);
	for (UINT f = 0; f < clip->FrameCount(); f++)
	{
		for (UINT b = 0; b < model->GetBones()->size(); b++)
		{
			ModelBone* bone = model->GetBones()->at(b);

			Matrix parent;
			Matrix invGlobal = XMMatrixInverse(nullptr, bone->transform);
			
			int parentIndex = bone->parentIndex;
			if (parentIndex < 0)
				parent = XMMatrixIdentity();
			else
				parent = bones[parentIndex];

			Matrix animation;
			KeyFrame* frame = clip->GetKeyFrame(bone->name);
			if (frame != nullptr)
			{
				KeyTransform& transform = frame->transforms[f];

				Matrix S = XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
				Matrix R = XMMatrixRotationQuaternion(XMLoadFloat4(&transform.rotation));
				Matrix T = XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);

				animation = S * R * T;
			}
			else
			{
				animation = XMMatrixIdentity();
			}

			bones[b] = animation * parent;
			clipTransform[index].transform[f][b] = invGlobal * bones[b];
		}
	}
}
