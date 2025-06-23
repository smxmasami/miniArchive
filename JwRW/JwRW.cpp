// JwRW.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include "stdafx.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "japanese");
#ifdef linux
    CStringA infile;
    CStringA outfile;
#else        
    CString infile;
    CString outfile;
#endif        
    if (argc < 3)
    {
        char buf[BUFSIZ];
        std::cout << "In file: ";
        std::cin >> buf;
        infile = buf;
        std::cout << "Out file: ";
        std::cin >> buf;
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
#ifdef linux
        pDoc->m_JwwHeader.Serialize(ar);
        pDoc->SetDepth(0);
        WORD count;
        ar >> count;
        for(WORD i = 0 ; i < count ; i++ )
        {
            CData* pObj = readObject(ar);
            pDoc->m_DataList.push_back(pObj);            
        }
        pDoc->SetDepth(1);
        count = pDoc->m_DataListList.size();
        ar >> count;
        for(WORD i = 0 ; i < count ; i++ )
        {
            CDataList* pObj = (CDataList*)readObject(ar);
            pDoc->m_DataListList.push_back(pObj);            
        }
#else        
        pDoc->Serialize(ar);
#endif        
        ar.Close();
        file.Close();
    }
    std::cout << "Write JW file." << std::endl;
    {
        CFile file(outfile, CFile::modeCreate | CFile::modeWrite);
        CArchive ar(&file, CArchive::store);
        ar.m_pDocument = pDoc;
        pDoc->m_jwwPath = outfile;
#ifdef linux
        pDoc->m_JwwHeader.Serialize(ar);
        pDoc->SetDepth(0);
        WORD count = pDoc->m_DataList.size();
        ar << count;
        for(auto it = pDoc->m_DataList.begin(); it != pDoc->m_DataList.end() ; it++ )
        {
            (*it)->Serialize(ar);
        }
        pDoc->SetDepth(1);
        count = pDoc->m_DataListList.size();
        ar << count;
        for(auto it = pDoc->m_DataListList.begin(); it != pDoc->m_DataListList.end() ; it++ )
        {
            CDataList* pObj = static_cast<CDataList*>(*it);
            pObj->Serialize(ar);
        }
#else        
        pDoc->Serialize(ar);
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
