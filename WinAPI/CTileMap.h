#pragma once
#include "CComponent.h"

struct tTileInfo
{
    int ImgIdx;

    tTileInfo()
        : ImgIdx(-1)
    {}
};


class CTileMap :
    public CComponent
{
private:
    UINT                m_Row;
    UINT                m_Col;
    Vec2                m_TileSize;
    int                 Tilecode;

    CTexture*           m_Atlas;
    

    Vec2                m_AtlasTileSize;
    Vec2                m_AtlasResolution;
    UINT                m_AtlasMaxRow;
    UINT                m_AtlasMaxCol;

    vector<tTileInfo>   m_vecTileInfo;
    
    bool                m_DbgOnOff; // 디버그 on off 용

public:
    void SetRowCol(UINT _Row, UINT _Col);
    UINT GetRow() { return m_Row; }
    UINT GetCol() { return m_Col; }

    void TileSelect(int _numb) { Tilecode = _numb; }
    void SetTileSize(Vec2 _TileSize) { m_TileSize = _TileSize; }
    Vec2 GetTileSize() { return m_TileSize; }

    void SetAtlas(CTexture* _Atlas);
    CTexture* GetAtlas() { return m_Atlas; }

    void SetAtlasTileSize(Vec2 _Size);

    // 인자로 몇 행 몇 열을 넣어주면 그 인덱스 번째 타일의 정볼르 줌
    tTileInfo* GetTileInfo(UINT _Col, UINT _Row);


public:
    virtual void FinalTick() override;
    void Render();

private:
    void Render_Tile();

public:
    void Save(const wstring& _strPath);
    void Load(const wstring& _strPath);
    void UseLoad(const wstring& _strPath);

public:
    CLONE(CTileMap);
    CTileMap();
    ~CTileMap();
};

