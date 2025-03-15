#include "pch.h"
#include "func.h"

#include "CTaskMgr.h"
#include "CObj.h"



void CreateObject(CObj* _Object, LAYER_TYPE _LayerType)
{
	tTask task = {};
	task.TaskType = TASK_TYPE::CREATE_OBJECT;
	task.Param_0 = (DWORD_PTR)_Object;
	task.Param_1 = (DWORD_PTR)_LayerType;

	CTaskMgr::Get()->AddTask(task);
}

void DeleteObject(CObj* _Object)
{
	tTask task = {};
	task.TaskType = TASK_TYPE::DELETE_OBJECT;
	task.Param_0 = (DWORD_PTR)_Object;

	CTaskMgr::Get()->AddTask(task);
}

void ChangeLevel(LEVEL_TYPE _Type)
{
	tTask task = {};
	task.TaskType = TASK_TYPE::CHANGE_LEVEL;
	task.Param_0 = (DWORD_PTR)_Type;
	CTaskMgr::Get()->AddTask(task);
}

#include "CDebugMgr.h"

void DrawDebugShape(DEBUG_SHAPE _Shape, PEN_TYPE _Pen, BRUSH_TYPE _Brush
			      , Vec2 _vPos, Vec2 _vScale, float _Duration)
{
	tDebugShapeInfo info = {};

	info.Shape = _Shape;	
	info.Pen = _Pen;
	info.BRUSH = _Brush;
	info.Position = _vPos;
	info.Scale = _vScale;	
	info.Duration = _Duration;

	CDebugMgr::Get()->AddDebugShapeInfo(info);
}

void DrawDebugLine(PEN_TYPE _Pen, Vec2 _Start, Vec2 _End, float Duration)
{
	tDebugShapeInfo info = {};

	info.Shape = DEBUG_SHAPE::LINE;
	info.Pen = _Pen;	
	info.Position = _Start;
	info.Scale = _End;
	info.Duration = Duration;

	CDebugMgr::Get()->AddDebugShapeInfo(info);
}

Vec2 RotateDir(Vec2 _vDir, float _Degree)
{
	// cos(0), sin(0)		
	// cos(0 + a), sin(0 + a) 
	// cos(0) * cos(a) - sin(0) * sin(a),  sin(0) * cos(a) + cos(0) * sin(a)

	// x, y
	// cos(0 + a), sin(0 + a) 
	// cos(a) * x  - sin(a) * y,  x * sin(a) + y * cos(a)
	float angle = (_Degree / 180.f) * PI;

	_vDir.Normalize();
	Vec2 vRotateDir = Vec2(cosf(angle) * _vDir.x - sinf(angle) * _vDir.y
		                 , sinf(angle) * _vDir.x + cosf(angle) * _vDir.y);

	return vRotateDir;
}

void SaveWString(const wstring& _Str, FILE* _File)
{
	BYTE len = (BYTE)_Str.length();
	fwrite(&len, sizeof(BYTE), 1, _File);
	fwrite(_Str.c_str(), sizeof(wchar_t), _Str.length(), _File);
}

void LoadWString(wstring& _Str, FILE* _File)
{
	BYTE len = 0;
	fread(&len, 1, 1, _File);

	wchar_t szBuff[255] = {};
	fread(szBuff, sizeof(wchar_t), len, _File);

	_Str = szBuff;
}


#include "CAssetMgr.h"
#include "CAsset.h"


void SaveAssetRef(CAsset* _Asset, FILE* _File) //내가 사용한 에셋의 정보 저장
{
	// Asset 이 nullptr 인지 아닌지 저장
	bool b = _Asset;
	fwrite(&b, 1, 1, _File);

	if (b)
	{
		SaveWString(_Asset->GetKey(), _File);
		SaveWString(_Asset->GetRelativePath(), _File);
	}
}

class CTexture* LoadTextureRef(FILE* _File) //내가 사용한 텍스쳐 정보 받기
{
	bool b = false;
	fread(&b, 1, 1, _File);

	if (b)
	{
		wstring strKey, strRelativePath;
		LoadWString(strKey, _File);
		LoadWString(strRelativePath, _File);

		return CAssetMgr::Get()->LoadTexture(strKey, strRelativePath);
	}
	else
	{
		return nullptr;
	}
}

// Vec2를 위한 선형 보간 함수
Vec2 Lerp(const Vec2& a, const Vec2& b, float t)
{
	return Vec2(
		a.x + (b.x - a.x) * t,
		a.y + (b.y - a.y) * t
	);
}

// float를 위한 선형 보간 함수 (필요한 경우)
float Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

// 값을 주어진 범위 내로 제한하는 함수
float Clamp(float value, float min, float max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}
