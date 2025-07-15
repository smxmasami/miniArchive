/*
    sample.cpp
*/
#include "archive.h"
#include "string.h"
#include <codecvt>
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
    char filename[128];
    std::cout << "Filename : ";
    std::cin >> filename;
    if(strlen(filename) == 0 )
    {
        return 1;
    }
    CFile file(filename, CFile::modeRead);
    CArchive ar(&file, CArchive::load);
    std::map<clskey,std::string> clsmap;
    WORD clskey = 1;
    // 個数 4バイト
    WORD number;
    ar >> number;
    for(WORD i = 0 ; i < number ; i++ )
    {
        // 01 80 クラスID バージョン
        // FFFF-0000 マジックナンバー？
        BYTE clsid;
        ar >> clsid;
        BYTE verid;
        ar >> verid;
        if( clsid == 0xff && verid == 0xff)
        {
            // ダミー2バイト
            WORD dummy;
            ar >> dummy;
            // クラス名の長さ 2バイト
            WORD clen;
            ar >> clen;
            // クラス名
            char cname[256] = { 0 };
            ar.Read(cname, clen);
            clsmap[clskey] = std::string(cname);
            clsid = clskey;
            clskey++;
        }
        std::string name = clsmap[clsid];
        CData* obj = nullptr;
        if(name.compare("CDataSen")==0)
        {
            obj = new CDataSen();
            obj->Serialize(ar);
            // 00000000 曲線属性番号
            // 01 線種
            // 0200 線色
            // 0000 線幅
            // 0000 レイヤ
            // 0000 グループ
            // 0000 フラグ
            // 00A501EF046D7140 始点X
            // 672C35859f3f6e40 始点Y
            // 16c84158c5f87940 終点X
            // 672c35859f3f6e40 終点Y
        }
        if(name.compare("CDataEnko")==0)
        {
            obj = new CDataEnko();
            obj->Serialize(ar);
            // CData() 部分 
        }
        if(name.compare("CDataTen")==0)
        {
            obj = new CDataTen();
            obj->Serialize(ar);
            // CData() 部分 
            // 00A501EF046D7140 中心X
            // 672C35859f3f6e40 中心Y
            // 00000000 仮点
            // 以下、線種により追加情報あり
        }
        if(name.compare("CDataMoji")==0)
        {
            obj = new CDataMoji();
            obj->Serialize(ar);
        }
        if(name.compare("CDataSunpou")==0)
        {
            obj = new CDataSunpou();
            obj->Serialize(ar);
        }
        if(name.compare("CDataSolid")==0)
        {
            obj = new CDataSolid();
            obj->Serialize(ar);
        }
        if(name.compare("CDataBlock")==0)
        {
            obj = new CDataBlock();
            obj->Serialize(ar);
        }
    }

    if(strcmp(name,"CDataList")==0)
        ;
    ar.Close();
    file.Close();
    return 0;
}

/*
int main(int argc, char** argv)
{
    CFile file("test.bin", CFile::modeRead);
    CArchive ar(&file, CArchive::load);
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    for(int i = 0 ;; i++)
    {
        CString str;
        ar >> str;
        if(str.GetLength() == 0 )
            break;
        std::string utf8str = converter.to_bytes(str);
        std::cout << i << " " << str.GetLength() << " " << utf8str << std::endl;
    }
    ar.Close();
    return 0;
}

int main(int argc, char** argv)
{
    CFile file("test.bin", CFile::modeWrite);
    CArchive ar(&file, CArchive::store);
    ar << CString(u"ABCおはよう");
    ar << CString(u"DEFこんにちは");
    ar << CString(u"GHIおやすみ");
    ar.Close();
    return 0;
}
*/
/*
int main(int argc, char** argv)
{
    CFile file("test.bin", CFile::modeRead);
    CArchive ar(&file, CArchive::load);
    for(int i = 0 ;; i++)
    {
        CAnsiString str;
        ar >> str;
        if(str.GetLength() == 0 )
            break;
        std::cout << i << " " << str.GetLength() << " " << str << std::endl;
    }
    ar.Close();
    return 0;
}
    */
/*
int main(int argc, char** argv)
{
    CFile file("test.bin", CFile::modeWrite);
    CArchive ar(&file, CArchive::store);
    ar << CAnsiString("ABC");
    ar << CAnsiString("DEF");
    ar << CAnsiString("GHI");
    ar.Close();
    return 0;
}
*/
/* CArchive クラスの読み込みテスト
int main(int argc, char** argv)
{
    CFile file("test.bin", CFile::modeRead);
    CArchive ar(&file, CArchive::load);
    char            c;
    unsigned char   uc;
    short           s;
    unsigned short  us;
    int             i;
    unsigned int    ui;
    long            l;
    unsigned long   ul;
    long long       ll;
    unsigned long long  ull;
    float           f;
    double          d;

    ar >> c;
    ar >> uc;
    ar >> s;
    ar >> us;
    ar >>          i;
    ar >>     ui;
    ar >>           l;
    ar >>    ul;
    ar >>        ll;
    ar >>   ull;
    ar >>           f;
    ar >>           d;
    ar.Close();

    std::cout << "char=" << c << std::endl;
    std::cout << "uchar=" <<uc << std::endl;
    std::cout << "short=" <<s << std::endl;
    std::cout << "ushort=" <<us << std::endl;
    std::cout << "int=" <<i << std::endl;
    std::cout << "uint=" <<    ui << std::endl;
    std::cout << "long=" <<          l << std::endl;
    std::cout << "ulong=" <<   ul << std::endl;
    std::cout << "llong=" <<       ll << std::endl;
    std::cout << "ullong=" <<  ull << std::endl;
    std::cout << "float=" <<          f << std::endl;
    std::cout << "double=" <<          d << std::endl;
    return 0;
}
*/
/* CArchive クラスの書き込みテスト
int main(int argc, char** argv)
{
    CFile file("test.bin", CFile::modeWrite);
    CArchive ar(&file, CArchive::store);
    ar << (char)'A' << (unsigned char)102 << (short)102 << (unsigned short)102;
    ar << (int)102 << (unsigned int)102 << (long)102 << (unsigned long)102;
    ar << (long long)102 << (unsigned long long)102 << (float)1.8e10 << (double)1.8e10;
    ar.Close();
    return 0;
}
*/

/* CFile クラスの書き込みテスト
int main(int argc, char** argv)
{
    CFile file("test.txt", CFile::modeWrite);
    char buffer[] = "CMakeFiles/miniArchive.dir/build.make:90: CMakeFiles/miniArchive.dir/archive.cpp.o";
    file.Write(buffer,sizeof(buffer)-1);
    file.Close();    
    return 0;
}
*/
/* CFile クラスの読み込みテスト
int main(int argc, char** argv)
{
    CFile file("test.txt", CFile::modeRead);
    char buffer[1024] = {0};
    std::cout << "File size:" << file.GetLength() << std::endl;
    file.SetPosition(10);
    file.Read(buffer,sizeof(buffer));
    file.Close();    
    std::cout << buffer << std::endl;
    return 0;
}
*/
