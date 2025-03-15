#pragma once

class CObj;

void CreateObject(CObj* _Object, LAYER_TYPE _LayerType);
void DeleteObject(CObj* _Object);
void ChangeLevel(LEVEL_TYPE _Type);


void DrawDebugShape(DEBUG_SHAPE _Shape, PEN_TYPE _Pen, BRUSH_TYPE _Brush, Vec2 _vPos, Vec2 _vScale, float _Duration = 0.f);
void DrawDebugLine(PEN_TYPE _Type, Vec2 _Start, Vec2 _End, float Duration);


template<typename T>
bool IsValid(T*& _Object)
{
	if (nullptr == _Object)
		return false;

	if (_Object->IsDead())
	{
		_Object = nullptr;
		return false;
	}

	return true;	
}


template<typename T>
void Release_Vector(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (nullptr == _vec[i])
			continue;

		delete _vec[i];		
	}

	_vec.clear();
}

template<typename T1, typename T2>
void Release_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter)
	{
		if (nullptr == iter->second)
			continue;

		delete iter->second;
	}

	_map.clear();
}

Vec2 RotateDir(Vec2 _vDir, float _Degree);

void SaveWString(const wstring& _Str, FILE* _File);
void LoadWString(wstring& _Str, FILE* _File);


void SaveAssetRef(class CAsset* _Asset, FILE* _File);
class CTexture* LoadTextureRef(FILE* _File);


// Vec2를 위한 선형 보간 함수
Vec2 Lerp(const Vec2& a, const Vec2& b, float t);

// float를 위한 선형 보간 함수 (필요한 경우)
float Lerp(float a, float b, float t);

// 값을 주어진 범위 내로 제한하는 함수
float Clamp(float value, float min, float max);




