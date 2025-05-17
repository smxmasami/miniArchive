/*
    sample.cpp
*/
#include "archive.h"
#include <iostream>
#include <fstream>

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
