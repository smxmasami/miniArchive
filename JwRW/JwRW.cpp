// JwRW.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include "stdafx.h"

int _tmain(int argc, TCHAR* argv[])
{
    setlocale(LC_ALL, "japanese");
    CString infile;
    CString outfile;
    if (argc < 3)
    {
        TCHAR buf[BUFSIZ];
        std::cout << "In file: ";
        std::wcin >> buf;
        infile = buf;
        std::cout << "Out file: ";
        std::wcin >> buf;
        outfile = buf;
    }
    else
    {
        infile = argv[1];
        outfile = argv[2];
    }
    CJwwDocument* pDoc = new CJwwDocument();
    std::cout << "Read JW file." << std::endl;
    {
        CFile file(infile, CFile::modeRead);
        CArchive ar(&file, CArchive::load);
        ar.m_pDocument = pDoc;
        pDoc->Serialize(ar);
        ar.Close();
        file.Close();
    }
    std::cout << "Write JW file." << std::endl;
    {
        CFile file(outfile, CFile::modeCreate | CFile::modeWrite);
        CArchive ar(&file, CArchive::store);
        ar.m_pDocument = pDoc;
        pDoc->m_jwwPath = outfile;
#ifdef NDEBUG
        pDoc->Serialize(ar);
#else
        // 図形データの出力
        pDoc->SetDepth(0);
        pDoc->m_DataList.Serialize(ar);
        // ブロック図形の出力
        pDoc->SetDepth(1);
        pDoc->m_DataListList.Serialize(ar);
#endif
        // ファイルを閉じる
        ar.Close();
        file.Close();
    }
    delete pDoc;
    return 0;
}

// おまけ
bool OdEqual(double x, double y, double f)
{
    return (std::abs(x - y) < f);
}
