#include "pch.h"
#include "CAnim.h"

#include "CObj.h"
#include "CEngine.h"
#include "CTexture.h"
#include "CTimeMgr.h"

#include "CAssetMgr.h"

CAnim::CAnim()
	: m_Owner(nullptr)
	, m_Atlas(nullptr)
	, m_CurIdx(0)
	, m_AccTime(0.f)
	, m_Finish(false)
{
}

CAnim::CAnim(const CAnim& _Other)
	: CBase(_Other)
	, m_Owner(nullptr)
	, m_vecFrm(_Other.m_vecFrm)
	, m_Atlas(_Other.m_Atlas)
	, m_CurIdx(_Other.m_CurIdx)
	, m_AccTime(_Other.m_AccTime)
	, m_Finish(_Other.m_Finish)
{
}

CAnim::~CAnim()
{
}

void CAnim::FinalTick()
{
	if (m_Finish)
		return;

	// 시간의 흐름을 체크
	m_AccTime += DT;

	// 진행된 시간이 현재 프레임의 Duration 을 초과하면
	if (m_vecFrm[m_CurIdx].Duration < m_AccTime)
	{
		m_AccTime -= m_vecFrm[m_CurIdx].Duration;
		++m_CurIdx;

		if (m_vecFrm.size() <= m_CurIdx)
		{
			m_CurIdx = (int)(m_vecFrm.size() - 1.f);
			m_Finish = true;
		}
	}
}

void CAnim::Render()
{
	assert(m_Atlas);

	HDC dc = CEngine::Get()->GetBackDC();

	// Animation 을 소유한 오브젝트의 위치
	Vec2 vPos = m_Owner->GetOwner()->GetRenderPos();

	BLENDFUNCTION BD = {};
	BD.BlendFlags = 0; // 그냥 0줌
	BD.SourceConstantAlpha = 255; // 색상의 진하기 이것도 수가 줄어들면 투명해짐
	BD.BlendOp = AC_SRC_OVER; // 이거는 두개뿐 이거랑 0 둘중하나 넣으면됌 0,1
	BD.AlphaFormat = AC_SRC_ALPHA; // 이거는 본인이 들고있는 알파값을 쓰겠다는 뜻
	// 알파설정이 없는 텍스쳐라면 0을 주면 됌.

	AlphaBlend(BackDC, (int)vPos.x - m_vecFrm[m_CurIdx].vSlice.x / 2.f + (int)m_vecFrm[m_CurIdx].vOffset.x,
		(int)vPos.y - m_vecFrm[m_CurIdx].vSlice.y / 2.f + (int)m_vecFrm[m_CurIdx].vOffset.y,
		(int)m_vecFrm[m_CurIdx].vSlice.x, (int)m_vecFrm[m_CurIdx].vSlice.y,
		m_Atlas->GetDC(),
		(int)m_vecFrm[m_CurIdx].vLeftTop.x, (int)m_vecFrm[m_CurIdx].vLeftTop.y,
		(int)m_vecFrm[m_CurIdx].vSlice.x, (int)m_vecFrm[m_CurIdx].vSlice.y, BD);
}

void CAnim::UI_Render()
{
	assert(m_Atlas);

	HDC dc = CEngine::Get()->GetBackDC();

	// Animation 을 소유한 오브젝트의 위치
	Vec2 vPos = m_Owner->GetOwner()->GetPos();

	BLENDFUNCTION BD = {};
	BD.BlendFlags = 0; // 그냥 0줌
	BD.SourceConstantAlpha = 255; // 색상의 진하기 이것도 수가 줄어들면 투명해짐
	BD.BlendOp = AC_SRC_OVER; // 이거는 두개뿐 이거랑 0 둘중하나 넣으면됌 0,1
	BD.AlphaFormat = AC_SRC_ALPHA; // 이거는 본인이 들고있는 알파값을 쓰겠다는 뜻
	// 알파설정이 없는 텍스쳐라면 0을 주면 됌.

	AlphaBlend(BackDC, (int)vPos.x - m_vecFrm[m_CurIdx].vSlice.x / 2.f + (int)m_vecFrm[m_CurIdx].vOffset.x,
		(int)vPos.y - m_vecFrm[m_CurIdx].vSlice.y / 2.f + (int)m_vecFrm[m_CurIdx].vOffset.y,
		(int)m_vecFrm[m_CurIdx].vSlice.x, (int)m_vecFrm[m_CurIdx].vSlice.y,
		m_Atlas->GetDC(),
		(int)m_vecFrm[m_CurIdx].vLeftTop.x, (int)m_vecFrm[m_CurIdx].vLeftTop.y,
		(int)m_vecFrm[m_CurIdx].vSlice.x, (int)m_vecFrm[m_CurIdx].vSlice.y, BD);
}


void CAnim::Create(const tAnimDesc& _Info)
{
	SetName(_Info.AnimName);
	m_Atlas = _Info.pAtlas;

	for (int i = 0; i < _Info.FrmCount; ++i)
	{
		tAnimFrm frm = {};

		frm.vLeftTop.x = _Info.StartLeftTop.x + _Info.SliceSize.x * (float)i;
		frm.vLeftTop.y = _Info.StartLeftTop.y;

		frm.vSlice = _Info.SliceSize;

		frm.Duration = 1.f / (float)_Info.FPS;

		m_vecFrm.push_back(frm);
	}
}

/// <summary>
/// 내가 만든 에디터 버전 Create
/// </summary>
/// <param name="_Info"></param>
void CAnim::CreateEditPlus(const tAnimDesc& _Info)
{
	SetName(_Info.AnimName);
	m_Atlas = _Info.pAtlas;

	tAnimFrm frm = {};
	frm.vLeftTop.x = _Info.StartLeftTop.x + _Info.SliceSize.x;
	frm.vLeftTop.y = _Info.StartLeftTop.y;
	frm.vSlice = _Info.SliceSize;
	frm.Duration = 1.f / (float)_Info.FPS;
	m_vecFrm.push_back(frm);
}

void CAnim::Save(const wstring& _FolderPath)
{
	wstring strName = GetName();
	wstring FilePath = _FolderPath + strName + L".anim";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, FilePath.c_str(), L"w");

	if (!pFile)
	{
		// 파일 열기에 실패한 경우
		MessageBox(NULL, L"Failed to open file for writing!", L"Error", MB_ICONERROR);
		return; // 함수 종료
	}

	// 애니메이션 이름
	fwprintf(pFile, L"[ANIMATION_NAME]\n");
	fwprintf(pFile, L"%s\n\n", strName.c_str());

	// 참조하던 아틀라스 텍스쳐 정보
	fwprintf(pFile, L"[ATLAS_TEXTURE]\n");

	if (m_Atlas)
	{
		fwprintf(pFile, L"%s\n", m_Atlas->GetKey().c_str());
		fwprintf(pFile, L"%s\n", m_Atlas->GetRelativePath().c_str());
	}
	else
	{
		fwprintf(pFile, L"None\n");
		fwprintf(pFile, L"None\n");
	}
	fwprintf(pFile, L"\n");

	// 각 프레임별 데이터
	fwprintf(pFile, L"[FRAME_DATA]\n");
	fwprintf(pFile, L"Frame_Count %d\n\n", static_cast<int>(m_vecFrm.size()));

	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fwprintf(pFile, L"Frame_Index %d\n", static_cast<int>(i));
		fwprintf(pFile, L"Left_Top    %f %f\n", m_vecFrm[i].vLeftTop.x, m_vecFrm[i].vLeftTop.y);
		fwprintf(pFile, L"Offset      %f %f\n", m_vecFrm[i].vOffset.x, m_vecFrm[i].vOffset.y);
		fwprintf(pFile, L"Slice       %f %f\n", m_vecFrm[i].vSlice.x, m_vecFrm[i].vSlice.y);
		fwprintf(pFile, L"Duration    %f\n\n", m_vecFrm[i].Duration);
	}

	fclose(pFile);

}

void CAnim::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"r");

	wchar_t szRead[255] = {};

	if (!pFile)
	{
		// 파일 열기에 실패한 경우
		MessageBox(NULL, L"Failed to open file for writing!", L"Error", MB_ICONERROR);
		return; // 함수 종료
	}

	while (true)
	{
		if (EOF == fwscanf_s(pFile, L"%s", szRead, 255))
			break;

		wstring str = szRead;

		if (str == L"[ANIMATION_NAME]")
		{
			fwscanf_s(pFile, L"%s", szRead, 255);
			SetName(szRead);
		}
		else if (str == L"[ATLAS_TEXTURE]")
		{
			wstring strKey, strRelativePath;
			fwscanf_s(pFile, L"%s", szRead, 255);
			strKey = szRead;

			fwscanf_s(pFile, L"%s", szRead, 255);
			strRelativePath = szRead;

			if (strKey != L"None")
			{
				m_Atlas = CAssetMgr::Get()->LoadTexture(strKey, strRelativePath);
			}
		}
		else if (str == L"[FRAME_DATA]")
		{
			wchar_t FrameBuff[255] = {};

			tAnimFrm frm = {};

			while (true)
			{
				if (EOF == fwscanf_s(pFile, L"%s", FrameBuff, 255))
				{
					break;
				}

				if (!wcscmp(L"Left_Top", FrameBuff))
				{
					fwscanf_s(pFile, L"%f %f", &frm.vLeftTop.x, &frm.vLeftTop.y);
				}
				else if (!wcscmp(L"Offset", FrameBuff))
				{
					fwscanf_s(pFile, L"%f %f", &frm.vOffset.x, &frm.vOffset.y);
				}
				else if (!wcscmp(L"Slice", FrameBuff))
				{
					fwscanf_s(pFile, L"%f %f", &frm.vSlice.x, &frm.vSlice.y);
				}
				else if (!wcscmp(L"Duration", FrameBuff))
				{
					fwscanf_s(pFile, L"%f", &frm.Duration);
					m_vecFrm.push_back(frm);
				}
			}
		}
	}

	fclose(pFile);
}

//void CAnim::Save(const wstring& _FolderPath)
//{
//	wstring strName = GetName();
//	wstring FilePath = _FolderPath + strName + L".anim";
//
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, FilePath.c_str(), L"wb");
//
//	// 애니메이션 이름
//	SaveWString(strName, pFile);
//	
//	// 각 프레임별 데이터
//	size_t size = m_vecFrm.size();
//	fwrite(&size, sizeof(size_t), 1, pFile);
//	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), size, pFile);
//
//	// 참조하던 아틀라스 텍스쳐 정보
//	SaveAssetRef(m_Atlas, pFile);
//	
//
//	fclose(pFile);
//}
//
//void CAnim::Load(const wstring& _FilePath)
//{
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
//
//	// Animation 이름
//	wstring strName;
//	LoadWString(strName, pFile);
//	SetName(strName);
//
//	// 프레임 데이터
//	size_t size = 0;
//	fread(&size, sizeof(size_t), 1, pFile);
//
//	m_vecFrm.resize(size);
//	fread(m_vecFrm.data(), sizeof(tAnimFrm), size, pFile);
//
//	// Atlas 텍스쳐
//	m_Atlas = LoadTextureRef(pFile);
//
//	fclose(pFile);
//}