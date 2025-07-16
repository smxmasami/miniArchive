// fileio.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "stdafx.h"
#include "locale.h"
#include "JwwDocument.h"

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