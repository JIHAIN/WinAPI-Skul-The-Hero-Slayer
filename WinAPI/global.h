#pragma once

// bmp 파일을 사용하기 위해 필요한 헤더와 라이브러리
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;

// 사운드 관련 헤더
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")


#include <assert.h>

#include "define.h"
#include "enum.h"
#include "struct.h"
#include "func.h"

#include "CSelectObject.h"