#include "pch.h"
#include "CLevel.h"

#include "CObj.h"
#include "Cfloor.h"
#include "CPlatform.h"
#include "Monster_Wall.h"

CLevel::CLevel()
	: m_CurPlayer(nullptr)
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		m_arrLayer[i].clear();
	}
}

CLevel::~CLevel()
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		Release_Vector(m_arrLayer[i]);
	}	
}

void CLevel::Tick()
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrLayer[i].size(); ++j)
		{
			m_arrLayer[i][j]->Tick();
		}
	}
}


void CLevel::FinalTick()
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrLayer[i].size(); ++j)
		{
			m_arrLayer[i][j]->FinalTick();
		}
	}
}

void CLevel::Render()
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		vector<CObj*>& vecObj = m_arrLayer[i];

		vector<CObj*>::iterator iter = vecObj.begin();
		for (; iter != vecObj.end(); )
		{
			if ((*iter)->IsDead())
			{
				iter = vecObj.erase(iter);
			}
			else
			{
				(*iter)->Render();
				++iter;
			}		
		}
	}
}

void CLevel::AddObject(CObj* _Obj, LAYER_TYPE _Type)
{
	m_arrLayer[(int)_Type].push_back(_Obj);
	_Obj->m_LayerType = (int)_Type;
}

void CLevel::DeleteObjects(LAYER_TYPE _Type)
{
	vector<CObj*>& vecObj = m_arrLayer[(int)_Type];
	Release_Vector(vecObj);
}

void CLevel::DeleteAllObjects()
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		DeleteObjects((LAYER_TYPE)i);
	}
}

void CLevel::LoadPlatformData(const wstring& filePath)
{
	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, filePath.c_str(), L"r");
	if (err != 0 || !pFile)
	{
		// 에러 처리
		return;
	}

	m_PlatformData.clear();

	wchar_t buffer[256];
	PlatformData data;
	int platformIndex;

	while (fwscanf_s(pFile, L"%s %d", buffer, (unsigned)_countof(buffer), &platformIndex) != EOF)
	{
		fwscanf_s(pFile, L"%s %d", buffer, (unsigned)_countof(buffer), &data.type);
		fwscanf_s(pFile, L"%s %f %f", buffer, (unsigned)_countof(buffer), &data.position.x, &data.position.y);
		fwscanf_s(pFile, L"%s %f %f", buffer, (unsigned)_countof(buffer), &data.scale.x, &data.scale.y);

		m_PlatformData.push_back(data);
	}

	fclose(pFile);
}

void CLevel::CreatePlatformsFromData()
{
	for (const auto& data : m_PlatformData)
	{
		CObj* pObj = nullptr;
		LAYER_TYPE layerType;

		if (data.type == 1) // 플랫폼
		{
			pObj = new CPlatform;
			pObj->SetName(L"Platform");
			layerType = LAYER_TYPE::PLATFORM;
		}
		else if (data.type == 2) // 플로어
		{
			pObj = new Cfloor;
			pObj->SetName(L"Floor");
			layerType = LAYER_TYPE::FLOOR;
		}
		else if (data.type == 3) // 몬스터 벽
		{
			pObj = new Monster_Wall;
			pObj->SetName(L"Wall");
			layerType = LAYER_TYPE::FLOOR;
		}

		if (pObj)
		{
			pObj->SetPos(data.position);
			pObj->SetScale(data.scale);
			AddObject(pObj, layerType);
		}
	}
}