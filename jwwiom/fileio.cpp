// fileio.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "stdafx.h"
#include "locale.h"
#include "JwwDocument.h"

int _tmain(int argc, TCHAR** argv)
{
	_tsetlocale(LC_ALL, _T(""));
	_tprintf(_T("Run fileiom\n"));
	CFile file(_T("TEST.BIN"), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	CArchive ar(&file, CArchive::store);
	// データを書き込む
	CTypedPtrList<CObList, CData*>	m_DataList;
	for (;;)
	{
		std::string buf;
		std::cout << "Type Line/Circle/Text/Point/Dimension/Solid/Block/eXit: ";
		std::cin >> buf;
		if (buf.compare("L") == 0)
		{
			auto obj = new CDataSen();
			obj->curve(0);
			obj->style(1);
			obj->color(1);
			obj->width(25);
			obj->layer(0);
			obj->group(0);
			obj->flags(0);
			OdGePoint3d point = { 0,0,0 };
			obj->start(point);
			point = { 100, 100, 0 };
			obj->end(point);
			m_DataList.AddTail(obj);
		}
		if (buf.compare("C") == 0)
		{
			auto obj = new CDataEnko();
			obj->curve(0);
			obj->style(1);
			obj->color(1);
			obj->width(25);
			obj->layer(0);
			obj->group(0);
			obj->flags(0);
			OdGePoint3d point = { 0,0,0 };
			obj->center(point);
			obj->radius(10.0);
			obj->flattenings(0);
			obj->start(0.0);
			obj->end(0.0);
			obj->tilt(0.0);
			obj->full(true);
			m_DataList.AddTail(obj);
		}
		if (buf.compare("P") == 0)
		{
			auto obj = new CDataTen();
			obj->curve(0);
			obj->style(1);
			obj->color(1);
			obj->width(25);
			obj->layer(0);
			obj->group(0);
			obj->flags(0);
			OdGePoint3d point = { 0,0,0 };
			obj->position(point);
			obj->pseudo(0);
			obj->code(1);
			obj->rotation(0.0);
			obj->scale(1.0);
			m_DataList.AddTail(obj);
		}
		if (buf.compare("T") == 0)
		{
			auto obj = new CDataMoji();
			obj->curve(0);
			obj->style(1);
			obj->color(1);
			obj->width(25);
			obj->layer(0);
			obj->group(0);
			obj->flags(0);
			obj->dimension(0);
			OdGePoint3d point = { 0,0,0 };
			obj->start(point);
			point = { 10,0,0 };
			obj->end(point);
			obj->code(1);
			obj->sizeX(1.0);
			obj->sizeY(2.5);
			obj->spacing(0.0);
			obj->rotation(0.0);
			OdString text = "Arial";
			obj->font(text);
			text = "ABC Sample";
			obj->contents(text);
			m_DataList.AddTail(obj);

		}
		if (buf.compare("D") == 0)
		{
			auto obj = new CDataSunpou();
			obj->curve(0);
			obj->style(1);
			obj->color(1);
			obj->width(25);
			obj->layer(0);
			obj->group(0);
			obj->flags(0);
			obj->sxfSunpou(true);
			auto diml = obj->getDimLine();
			OdGePoint3d point = { 0,50,0 };
			diml->start(point);
			point = { 100, 50, 0 };
			diml->end(point);
			auto dimt = obj->getDimText();
			point = { 40, 60, 0 };
			dimt->start(point);
			point = { 60,60,0 };
			dimt->end(point);
			dimt->code(1);
			dimt->sizeX(1.0);
			dimt->sizeY(2.5);
			dimt->spacing(0.0);
			dimt->rotation(0.0);
			OdString text = "100.0";
			dimt->contents(text);
			text = "Arial";
			dimt->font(text);
			diml = obj->getExt1Line();
			point = { 0,0,0 };
			diml->start(point);
			point = { 0,60,0 };
			diml->end(point);
			diml = obj->getExt2Line();
			point = { 100,0,0 };
			diml->start(point);
			point = { 100,60,0 };
			diml->end(point);
			auto dimp = obj->getDim1Pos();
			point = { 0,50,0 };
			dimp->position(point);
			dimp = obj->getDim2Pos();
			point = { 100,50,0 };
			dimp->position(point);
			dimp = obj->getExt1Pos();
			point = { 0,0,0 };
			dimp->position(point);
			dimp = obj->getExt2Pos();
			point = { 100,0,0 };
			dimp->position(point);
			m_DataList.AddTail(obj);
		}
		if (buf.compare("S") == 0)
		{
			auto obj = new CDataSolid();
			obj->curve(0);
			obj->style(1);
			obj->color(1);
			obj->width(25);
			obj->layer(0);
			obj->group(0);
			obj->flags(0);
			OdGePoint3d point = { 100,100,0 };
			obj->location(1, point);
			point = { 200,100,0 };
			obj->location(2, point);
			point = { 200,200,0 };
			obj->location(3, point);
			point = { 100,200,0 };
			obj->location(4, point);
			obj->solidColor(0x00808080);
			m_DataList.AddTail(obj);
		}
		if (buf.compare("B") == 0)
		{
			auto obj = new CDataBlock();
			obj->curve(0);
			obj->style(1);
			obj->color(1);
			obj->width(25);
			obj->layer(0);
			obj->group(0);
			obj->flags(0);
			obj->blockNo(1);
			OdGePoint3d point = { 0,200,0 };
			obj->start(point);
			obj->rotation(0.0);
			OdGeScale3d scale = { 1.0, 1.0, 1.0 };
			obj->scale(scale);
			m_DataList.AddTail(obj);
		}
		if (buf.compare("X") == 0)
		{
			break;
		}
	}
	m_DataList.Serialize(ar);
	CTypedPtrList<CObList, CDataList*>	m_DataListList;
	{
		auto part = new CDataEnko();
		part->curve(0);
		part->style(1);
		part->color(1);
		part->width(25);
		part->layer(0);
		part->group(0);
		part->flags(0);
		OdGePoint3d point = { 0,0,0 };
		part->center(point);
		part->radius(10.0);
		part->flattenings(0);
		part->start(0.0);
		part->end(0.0);
		part->tilt(0.0);
		part->full(true);
		auto obj = new CDataList();
		obj->m_nNumber = 1;
		obj->m_bReffered = 1;
		obj->m_time = 0;
		obj->m_strName = "TEST";
		obj->m_DataList.AddTail(part);
		m_DataListList.AddTail(obj);
	}
	m_DataListList.Serialize(ar);
	ar.Close();
	file.Close();
	return 0;
}


/*
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
*/
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