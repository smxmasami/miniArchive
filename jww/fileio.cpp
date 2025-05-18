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
	CFile file(_T("C:\\TEMP\\TEST.BIN"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	CArchive ar(&file, CArchive::store);
	// データを書き込む
	CTypedPtrList<CObList, CData*>	m_DataList;

	CDataSen sen;
	sen.m_lGroup = 0;
	sen.m_nPenStyle =1;
	sen.m_nPenColor = 2;
	sen.m_nPenWidth = 25;
	sen.m_nLayer = 8;
	sen.m_nGLayer = 1;
	sen.m_sFlg = 0;
	sen.m_start.x = 100.0;
	sen.m_start.y = 100.0;
	sen.m_end.x = 200.0;
	sen.m_end.y = 200.0;
	m_DataList.AddTail(&sen);

	CDataMoji moji;
	moji.m_lGroup = 0;
	moji.m_nPenStyle = 1;
	moji.m_nPenColor = 2;
	moji.m_nPenWidth = 25;
	moji.m_nLayer = 8;
	moji.m_nGLayer = 1;
	moji.m_sFlg = 0;
	moji.m_start.x = 100.0;
	moji.m_start.y = 100.0;
	moji.m_end.x = 200.0;
	moji.m_end.y = 200.0;
	moji.m_nSunpouFlg = 0;
	moji.m_nMojiShu = 0;
	moji.m_dSizeX = 100.0;        // 文字サイズ横
	moji.m_dSizeY = 20.0;        // 文字サイズ縦
	moji.m_dKankaku = 0.0;      // 文字間隔
	moji.m_degKakudo = 0.0;     // 回転角
	moji.m_strFontName = "Arial";  // フォント名
	moji.m_string = "ABCDE XYZ";       // 文字列
	m_DataList.AddTail(&moji);

	CDataSen sen2(sen);
	sen2.m_nPenColor = 3;
	m_DataList.AddTail(&sen2);

	CDataMoji moji2(moji);
	moji2.m_nPenColor = 3;
	m_DataList.AddTail(&moji2);

	m_DataList.Serialize(ar);

	CTypedPtrList<CObList, CDataList*>	m_DataListList;

	CDataList block;
	block.m_lGroup = 0;
	block.m_nPenStyle = 0;
	block.m_nPenColor = 0;
	block.m_nPenWidth = 0;
	block.m_nLayer = 0;
	block.m_nGLayer = 0;
	block.m_sFlg = 0;
	block.m_nNumber =1 ;
	block.m_bReffered=0;
	block.m_time;
	block.m_strName = _T("TEST");

	CDataSen sen3(sen);
	sen3.m_nPenColor = 4;
	block.m_DataList.AddTail(&sen3);

	m_DataListList.AddTail(&block);

	m_DataListList.Serialize(ar);
	/*
	ar << (CHAR)'A';	//	41
	ar << (WCHAR)L'朝'; //	1d 67	(シフトJISの 下位 上位バイト の順
	ar << (INT16)10;	//	0a 00	(下位 上位バイトの順
	ar << (INT32)20L;	//	14 00 00 00 (最下位 .. 最上位バイトの順
	ar << (INT64)30LL;	//	1e 00 00 00 00 00 00 00 (最下位 .. 最上位バイトの順
	CString str = "Doremi"; // ff fe ff 06 L"Doremi" - fffeff 文字数(バイト) 0～254文字のとき
	ar << str;
	CStringA astr = "Doremi"; // 06 "Doremi" - 文字数(バイト) 文字列 0～254文字のとき
	ar << astr;
	str = CString(_T('X'), 255);
	ar << str; // ff fe ff ff ff 00 L"X..." - fffe ffff 文字数(ワード) 文字列 255～65535文字のとき 
	str += _T("YY");
	ar << str; // ff fe ff ff 01 01 L"X...Y"
	astr = CStringA('x', 256);
	astr += "y";
	ar << astr; // ff 01 01 "x...y" - ff 文字数(ワード) 文字列 255～65534文字のとき
	astr = CStringA('x', 255);
	ar << astr; // ff ff 00 "x..."
	CStringA str = CStringA('A', 65537);
	ar << str; // ff ff ff 01 00 01 00 "A" - ff ffff 文字数(３２ビット) 文字列 65535～文字のとき
	CString str = CString(_T('A'), 65537);
	ar << str; // ff fe ff ff ff ff 01 00 01 00 "A" - fffe ffff ffff 文字数(32ビット) 文字列 65535～文字のとき
	CStringA astr = CStringA('x', 254);
	ar << astr; // fe "x..."
	astr.Empty();
	ar << astr; // 00
	*/
	ar.Close();
	//file.Close();
	return 0;
}
