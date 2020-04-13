#pragma once
#include "Pch.h"

#define WIN_START_X 0
#define WIN_START_Y 0
#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

#define MAX_MODEL_BONE 300
#define MAX_MODEL_KEY 600
#define MAX_MODEL_INSTANCE 500

#ifdef NDEBUG
	#define V(hr) hr
#else
	#define V(hr) assert(SUCCEEDED(hr))
#endif

#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetContext()

#define DELTA Timer::Get()->Delta()

#define KEYDOWN(k) Keyboard::Get()->Down(k)
#define KEYPRESS(k) Keyboard::Get()->Press(k)
#define KEYUP(k) Keyboard::Get()->Up(k)

#define MOUSEPOS Keyboard::Get()->GetMouse()

#define LERP(s, e, t) (s + (e - s)*t)

typedef XMFLOAT4 Float4;
typedef XMFLOAT3 Float3;
typedef XMFLOAT2 Float2;
typedef XMVECTOR Vector4;
typedef XMMATRIX Matrix;
typedef XMFLOAT4X4 Float4x4;
typedef XMCOLOR Color;

typedef function<void()> Event;

const XMVECTORF32 kForward = { 0, 0, 1 };
const XMVECTORF32 kUp = { 0, 1, 0 };
const XMVECTORF32 kRight = { 1, 0, 0 };

#include "Device/Device.h"

#include "Math/Vector3.h"
#include "Math/Transform.h"
#include "Math/Math.h"

#include "Utility/Path.h"
#include "Utility/Binary.h"
#include "Utility/Keyboard.h"
#include "Utility/Timer.h"
#include "Utility/Xml.h"

#include "Render/Shader.h"
#include "Render/ComputShader.h"
#include "Render/Buffer.h"
#include "Render/ConstBuffer.h"
#include "Render/GlobalBuffer.h"
#include "Render/VertexLayouts.h"
#include "Render/Texture.h"
#include "Render/Material.h"
#include "Render/CsResource.h"
#include "Render/CsBuffer.h"
#include "Render/DepthStencil.h"
#include "Render/RenderTarget.h"

#include "State/RasterizerState.h"
#include "State/BlendState.h"
#include "State/DepthStencilState.h"
#include "State/SamplerState.h"

#include "Mesh/Mesh.h"
#include "Mesh/MeshRender.h"

#include "Converter/Types.h"
#include "Converter/ModelReader.h"

#include "Model/ModelMeshPart.h"
#include "Model/ModelMesh.h"
#include "Model/ModelClip.h"
#include "Model/Model.h"
#include "Model/ModelSingle.h"
#include "Model/ModelRender.h"
#include "Model/ModelAnimator.h"

#include "Collision/Collider.h"
#include "Collision/BoxCollider.h"
#include "Collision/SphereCollider.h"

using namespace GameMath;