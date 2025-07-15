// fileio.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#define _WIN32_WINNT	0x0A00

#include "afx.h"
#include "locale.h"
#include <afxtempl.h>

#define EXTFUNC

/*

*/

struct EXTFUNC DPoint
{
	double x;
	double y;
};

class EXTFUNC CData : public CObject
{
public:
	CData() {}
	CData(const CData&);
	DWORD	m_lGroup;		// 曲線属性番号
	BYTE	m_nPenStyle;	// 線種番号
	WORD	m_nPenColor;	// 線色番号
	WORD	m_nPenWidth;	// 線色幅
	WORD	m_nLayer;		// 画層番号
	WORD	m_nGLayer;		// 画層グループ番号
	WORD	m_sFlg;			// 属性フラグ
public:
	DECLARE_SERIAL(CData);
	virtual void Serialize(CArchive& ar);
};

class EXTFUNC CDataSen : public CData
{
public:
	CDataSen() {}
	CDataSen(const CDataSen&);
	DPoint m_start;       // 始点
	DPoint m_end;         // 終点
public:
	// シリアライズ
	DECLARE_SERIAL(CDataSen);
	virtual void Serialize(CArchive& ar);
};


class EXTFUNC CDataMoji : public CData
{
public:
	CDataMoji() {}
	CDataMoji(const CDataMoji&);
	WORD m_nSunpouFlg;      // 寸法フラグ
	DPoint m_start;         // 始点座標
	DPoint m_end;           // 終点座標
	DWORD m_nMojiShu;       // 文字種
	double m_dSizeX;        // 文字サイズ横
	double m_dSizeY;        // 文字サイズ縦
	double m_dKankaku;      // 文字間隔
	double m_degKakudo;     // 回転角
	CString m_strFontName;  // フォント名
	CString m_string;       // 文字列

public:
	// シリアライズ
	DECLARE_SERIAL(CDataMoji);
	virtual void Serialize(CArchive& ar);
};

IMPLEMENT_SERIAL(CData, CObject, VERSIONABLE_SCHEMA);

CData::CData(const CData& s)
{
	m_lGroup = s.m_lGroup;		// 曲線属性番号
	m_nPenStyle = s.m_nPenStyle;	// 線種番号
	m_nPenColor = s.m_nPenColor;	// 線色番号
	m_nPenWidth = s.m_nPenWidth;	// 線色幅
	m_nLayer = s.m_nLayer;		// 画層番号
	m_nGLayer = s.m_nGLayer;		// 画層グループ番号
	m_sFlg = s.m_sFlg;			// 属性フラグ
}

void CData::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << (DWORD)m_lGroup;            //曲線属性番号
		ar << (BYTE)m_nPenStyle;          //線種番号
		ar << (WORD)m_nPenColor;          //線色番号
		ar << (WORD)m_nPenWidth;      //線色幅
		ar << (WORD)m_nLayer;             //画層番号
		ar << (WORD)m_nGLayer;            //画層グループ番号
		ar << (WORD)m_sFlg;               //属性フラグ
	}
	else
	{
		ar >> m_lGroup;            //曲線属性番号
		ar >> m_nPenStyle;          //線種番号
		ar >> m_nPenColor;          //線色番号
		ar >> m_nPenWidth;      //線色幅
		ar >> m_nLayer;             //画層番号
		ar >> m_nGLayer;            //画層グループ番号
		ar >> m_sFlg;               //属性フラグ
	}
}

IMPLEMENT_SERIAL(CDataSen, CData, VERSIONABLE_SCHEMA);

CDataSen::CDataSen(const CDataSen& s)
	: CData(s)
{
	m_start = s.m_start;
	m_end = s.m_end;
}

void CDataSen::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << (double)m_start.x;
		ar << (double)m_start.y;
		ar << (double)m_end.x;
		ar << (double)m_end.y;
	}
	else
	{
		ar >> m_start.x;
		ar >> m_start.y;
		ar >> m_end.x;
		ar >> m_end.y;
	}
}

IMPLEMENT_SERIAL(CDataMoji, CData, VERSIONABLE_SCHEMA);

CDataMoji::CDataMoji(const CDataMoji& s)
	: CData(s)
{
	m_nSunpouFlg = s.m_nSunpouFlg;      // 寸法フラグ
	m_start = s.m_start;         // 始点座標
	m_end = s.m_end;           // 終点座標
	m_nMojiShu = s.m_nMojiShu;       // 文字種
	m_dSizeX = s.m_dSizeX;        // 文字サイズ横
	m_dSizeY = s.m_dSizeY;        // 文字サイズ縦
	m_dKankaku = s.m_dKankaku;      // 文字間隔
	m_degKakudo = s.m_degKakudo;     // 回転角
	m_strFontName = s.m_strFontName;  // フォント名
	m_string = s.m_string;       // 文字列
}

void CDataMoji::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << m_nSunpouFlg;
		ar << m_start.x;
		ar << m_start.y;
		ar << m_end.x;
		ar << m_end.y;
		ar << m_nMojiShu;
		ar << m_dSizeX;        // 文字サイズ横
		ar << m_dSizeY;        // 文字サイズ縦
		ar << m_dKankaku;      // 文字間隔
		ar << m_degKakudo;     // 回転角
		ar << CStringA(m_strFontName);  // フォント名
		ar << CStringA(m_string);       // 文字列
	}
	else
	{
		ar >> m_nSunpouFlg;      // 寸法フラグ
		ar >> m_start.x;
		ar >> m_start.y;
		ar >> m_end.x;
		ar >> m_end.y;
		ar >> m_nMojiShu;       // 文字種
		ar >> m_dSizeX;        // 文字サイズ横
		ar >> m_dSizeY;        // 文字サイズ縦
		ar >> m_dKankaku;      // 文字間隔
		ar >> m_degKakudo;     // 回転角
		ar >> m_strFontName;  // フォント名
		ar >> m_string;       // 文字列
	}
}

class EXTFUNC CDataList : public CData
{
public:
	DWORD m_nNumber;
	DWORD m_bReffered;
	CTime m_time;
	CString m_strName;
	CTypedPtrList<CObList, CData*> m_DataList;
public:
	DECLARE_SERIAL(CDataList);
	virtual void Serialize(CArchive& ar);
};

IMPLEMENT_SERIAL(CDataList, CData, VERSIONABLE_SCHEMA);

void CDataList::Serialize(CArchive& ar)
{
	CData::Serialize(ar);

	if (ar.IsStoring())
	{
		ar << (DWORD)m_nNumber;
		ar << (DWORD)m_bReffered;
		UINT32 tm32 = (UINT32)m_time.GetTime();
		ar << tm32;
		// ブロックにはSXF複合図形の接頭辞が必要
		if (m_strName.Find(_T("@@SfigorgFlag@@")) < 0)
		{
			m_strName += _T("@@SfigorgFlag@@4");
		}
		// ブロック名はMBCS文字列として出力
		ar << CStringA(m_strName);
	}
	else
	{
		ar >> m_nNumber;
		ar >> m_bReffered;
		ar >> m_time;
		ar >> m_strName;
	}
	m_DataList.Serialize(ar);
}

int _tmain(int argc, TCHAR** argv )
{
	_tsetlocale(LC_ALL, _T(""));
	_tprintf(_T("Run fileio\n"));
	if (argc < 2)
	{
		_tprintf(_T("Usage: JWWIN IN_FILE\n"));
		return 1;
	}
	CFile file(argv[1], CFile::modeRead | CFile::typeBinary);
	CArchive ar(&file, CArchive::load);
	// データを読み込む
	CTypedPtrList<CObList, CData*>	m_DataList;
	m_DataList.Serialize(ar);
	CTypedPtrList<CObList, CDataList*>	m_DataListList;
	m_DataListList.Serialize(ar);
	ar.Close();
	file.Close();
	// データを表示
	_tprintf(_T("DataList count=%lld\n"), m_DataList.GetCount());
	for (auto pos = m_DataList.GetHeadPosition(); pos != nullptr ; )
	{
		CData* obj = m_DataList.GetNext(pos);
		_tprintf(_T(" %hs %x-%x\n"), obj->GetRuntimeClass()->m_lpszClassName, obj->m_nGLayer, obj->m_nLayer);
	}
	_tprintf(_T("DataListList count=%lld\n"), m_DataListList.GetCount());
	for (auto pos = m_DataListList.GetHeadPosition();  pos != nullptr; )
	{
		CDataList* rec = m_DataListList.GetNext(pos);
		_tprintf(_T(" %hs %s %d\n"), rec->GetRuntimeClass()->m_lpszClassName, (LPCTSTR)rec->m_strName, rec->m_nNumber);
		for (auto pos = rec->m_DataList.GetHeadPosition(); pos != nullptr ;)
		{
			CData* obj = rec->m_DataList.GetNext(pos);
			_tprintf(_T("  %hs %x-%x\n"), obj->GetRuntimeClass()->m_lpszClassName, obj->m_nGLayer, obj->m_nLayer);
		}
	}
	return 0;
}

/*
Run fileio
DataList count=4
 CDataSen 1-8
 CDataMoji 1-8
 CDataSen 1-8
 CDataMoji 1-8
DataListList count=2
 CDataList TEST@@SfigorgFlag@@4 1
  CDataSen 1-8
 CDataList TEST2@@SfigorgFlag@@4 2
  CDataSen 1-8
  CDataMoji 1-8
*/