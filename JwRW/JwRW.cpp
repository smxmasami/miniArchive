// JwRW.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include "stdafx.h"

int _tmain(int argc, TCHAR* argv[])
{
    setlocale(LC_ALL, "japanese");
    if (argc < 3)
    {
        std::cout << "JwRW in_file out_file" << std::endl;
        return EPERM;
    }
    CJwwDocument* pDoc = new CJwwDocument();
    std::cout << "Read JW file." << std::endl;
    {
        CFile file(argv[1], CFile::modeRead);
        CArchive ar(&file, CArchive::load);
        ar.m_pDocument = pDoc;
        pDoc->Serialize(ar);
        ar.Close();
        file.Close();
    }
    std::cout << "Write JW file." << std::endl;
    {
        CFile file(argv[2], CFile::modeCreate | CFile::modeWrite);
        CArchive ar(&file, CArchive::store);
        ar.m_pDocument = pDoc;
        pDoc->Serialize(ar);
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
