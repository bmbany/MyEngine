#pragma once

// 여기에 미리 컴파일하려는 헤더 추가
#include <iostream>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <thread>
#include <mutex>
#include <random>
#include <iomanip>
#include <type_traits>
#include <chrono>
#include <utility>
#include <typeindex>
#include <filesystem>
#include <dwrite.h>
#include <d3d11.h>
#include <directxtk/SimpleMath.h>
#include <comdef.h>
#include <d3dcompiler.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Xinput.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "fmod_vc.lib")

using namespace DirectX;
using namespace DirectX::SimpleMath;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "SpinLock.h"

#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Utility.h"
#include "Engine_Enum.h"
#include "Engine_Extern.h"