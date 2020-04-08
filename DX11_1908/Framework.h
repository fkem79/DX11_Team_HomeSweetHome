#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN

#define VP Environment::Get()->GetVP()
#define CAMERA Environment::Get()->MainCamera()
#define LIGHT Environment::Get()->GetLight()

#include <windows.h>

#include <Mouse.h>
//#include <Keyboard.h>
using namespace DirectX;

extern std::unique_ptr<Mouse> g_mouse;
//extern unique_ptr<Keyboard> g_keyboard;

#include "../Framework/Framework.h"

#include "Camera/Camera.h"
#include "Camera/FreeCam.h"
#include "Camera/FollowCam.h"
#include "Camera/FirstPersonView.h"

#include "Environment/Environment.h"

#include "Objects/Landscape/Terrain.h"
#include "Objects/Landscape/TerrainEditor.h"
#include "Objects/Landscape/CubeSky.h"

#include "Objects/UI/Render2D.h"

#include "Objects/Model/Kaya.h"
#include "ADX11Resource/Model/Belle.h"
#include "ADX11Resource/Model/PlayerHead.h"
#include "ADX11Resource/Model/PlayerLHand.h"

#include "ADX11Resource/Object/DoorWall.h"

#include "Objects/Sphere.h"
#include "Objects/Shadow.h"

#include "AStar/Node.h"
#include "AStar/Heap.h"
#include "AStar/AStar.h"

#include "Scene/Scene.h"