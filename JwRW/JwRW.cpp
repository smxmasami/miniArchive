// JwRW.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include "afx.h"
#include "atlstr.h"
#include <iostream>

int main()
{
    CFile file(_T("test.bin"), CFile::modeCreate|CFile::modeWrite);
    CArchive ar(&file, CArchive::store);
    ar << CStringA(_T("ABCおはよう"));
    ar << CStringA(_T("DEFこんにちは"));
    ar << CStringA(_T("GHIおやすみ"));
    ar.Close();
    return 0;
}
