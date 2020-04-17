#pragma once

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <assert.h>
#include <functional>

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>
#include <wrl/client.h>
#include <DirectXCollision.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")

//ImGui
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
//ImGui Gizmo
#include "../ImGui/ImGuizmo.h"

//DirectXTex
#include "../DirectXTex/DirectXTex.h"

//Assimp
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

//FMOD
//#include "FMOD/inc/fmod.hpp"

//#pragma comment(lib, "Framework/FMOD/lib/x86/fmod_vc.lib")
#pragma comment(lib, "Assimp/assimp-vc142-mtd.lib")


using namespace DirectX;
using namespace std;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;
using namespace DirectX::TriangleTests;
