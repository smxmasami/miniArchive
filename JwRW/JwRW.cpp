// JwRW.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include "afx.h"
#include "atlstr.h"
#include <iostream>

int main()
{
    //CJwwDocument* pDoc = new CJwwDocument();
    std::cout << "Read JW file." << std::endl;
    {
        CFile file(_T("test.bin"), CFile::modeRead);
        CArchive ar(&file, CArchive::load);
        CStringA str;
        ar >> str;
        std::cout << str << std::endl;
        ar >> str;
        std::cout << str << std::endl;
        ar >> str;
        std::cout << str << std::endl;
        ar.Close();
    }
    std::cout << "Write JW file." << std::endl;
    {
        CFile file(_T("test.bin"), CFile::modeCreate | CFile::modeWrite);
        CArchive ar(&file, CArchive::store);
        ar << CStringA(_T("ABCおはよう"));
        ar << CStringA(_T("DEFこんにちは"));
        ar << CStringA(_T("GHIおやすみ"));
        ar.Close();
    }
    //delete pDoc;
    return 0;
}
