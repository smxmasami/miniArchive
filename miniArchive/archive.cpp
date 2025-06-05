/*
    archive.cpp
*/
#include "archive.h"

//-----------------------------------------------------------
// CStringA UTF8またはShiftJIS文字列
//-----------------------------------------------------------
CStringA::CStringA(const char* str)
{
    m_str = std::string(str);
}

CStringA::CStringA(const char16_t* str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    m_str = converter.to_bytes(str);
}


CStringA& CStringA::operator = (const char* str )
{
    m_str = std::string(str);
    return *this;
}

DWORD  CStringA::GetLength() const
{
    return m_str.length();
}

CStringA::operator const char* () const
{
    return m_str.c_str();
}

//-----------------------------------------------------------
// CString UTF16(LE)文字列
//-----------------------------------------------------------
CString::CString(const char16_t* str)
{
    m_str = std::u16string(str);
}

CString::CString(const char* str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    m_str = converter.from_bytes(str);
}

CString& CString::operator = (const char16_t* str )
{
    m_str = std::u16string(str);
    return *this;
}

CString& CString::operator = (const char* str )
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    m_str = converter.from_bytes(str);
    return *this;
}

void CString::operator += (const char16_t* str )
{
    m_str = m_str + std::u16string(str);
}

void CString::operator += (const char* str )
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    m_str = m_str + converter.from_bytes(str);
}

CString::operator const char16_t* () const
{
    return m_str.c_str();
}

DWORD  CString::GetLength() const
{
    return m_str.length();
}

char16_t CString::GetAt(int p) const
{
    return m_str[p];
}

void CString::Insert(unsigned int i, const char16_t* t)
{
    m_str.insert(i, t);
}

void CString::Delete(unsigned int i, unsigned int n)
{
    m_str.erase(i,n);
}

int CString::Find(const char16_t* str, int i ) const
{
   return (int)m_str.find(str,i);
}

//-----------------------------------------------------------
// CFile c++ ファイルのカプセル化
//-----------------------------------------------------------
CFile::CFile(LPCSTR lpszFileName, UINT nOpenFlags)
{
    if(nOpenFlags & modeRead)
    {
        m_in.open(lpszFileName,std::ios::in | std::ios::binary);
    }
    else if( nOpenFlags & modeWrite)
    {
        m_out.open(lpszFileName,std::ios::out | std::ios::binary);
    }
    m_name = lpszFileName;
}
void CFile::Close()
{
    if(m_in.is_open())
        m_in.close();
    if(m_out.is_open())
        m_out.close();
}

CStringA CFile::GetFilePath() const
{
    return m_name.c_str();
}

void CFile::SetPosition(ULONGLONG pos)
{
    if(m_in.is_open())
        m_in.seekg(pos);
    if(m_out.is_open())
        m_out.seekp(pos);
}
ULONGLONG CFile::Seek(LONGLONG lOff,UINT nFrom)
{
    std::ios_base::seekdir dir;
    if(nFrom == CFile::begin)
        dir = std::ios::beg;
    if(nFrom == CFile::current)
        dir = std::ios::cur;
    if(nFrom == CFile::end)
        dir = std::ios::end;
    if(m_in.is_open())
    {
        m_in.seekg(lOff,dir);
    }
    if(m_out.is_open())
    {
        m_out.seekp(lOff,dir);
    }
    return GetPosition();
}

ULONGLONG CFile::GetLength() const
{
    return std::filesystem::file_size(m_name);
}

ULONGLONG CFile::GetPosition()
{
    std::streampos pos;
    if(m_in.is_open())
    {
        pos = m_in.tellg();
    }
    if(m_out.is_open())
    {
        pos = m_out.tellp();
    }
    return static_cast<ULONGLONG>(pos);
}

UINT CFile::Read(void* lpBuf, UINT nCount)
{
    if(m_in.is_open())
    {
        m_in.read(static_cast<char*>(lpBuf), nCount);
        if(m_in.eof())
        {
            nCount = m_in.gcount();
        }
    }
    else
    {
        nCount = 0;
    }
    return nCount;
}

void CFile::Write(const void* lpBuf, UINT nCount)    
{
    if(m_out.is_open())
    {
        m_out.write(static_cast<const char*>(lpBuf), nCount);
    }
}

//-----------------------------------------------------------
// CArchive MFC CArchive バイナリ形式のファイルストリームを実装
//-----------------------------------------------------------
CArchive::CArchive()
    : m_file(nullptr)
    , m_mode(0)
{

}
CArchive::CArchive(CFile* pFile, UINT nMode, int nBufSize, void* lpBuf)
    : m_file(pFile)
    , m_mode(nMode)
{

}

CArchive::~CArchive()
{

}

void CArchive::Close()
{
    m_file->Close();
}

CFile* CArchive::GetFile() const
{
    return m_file;
}

BOOL CArchive::IsLoading() const
{
    return m_mode == CArchive::load;
}

BOOL CArchive::IsStoring() const
{
    return m_mode == CArchive::store;
}

UINT CArchive::Read(void* lpBuf, UINT nMax)
{
    UINT ret = 0;
    if(m_file)
    {
        ret = m_file->Read(lpBuf,nMax);
    }
    return ret;
}

// '\n' まで読み込む
BOOL CArchive::ReadString(CStringA& rString)
{
    return 0;
}

void CArchive::Write(const void* lpBuf, UINT nMax)
{
    if(m_file)
    {
        m_file->Write(lpBuf, nMax);
    }
}

// '\0' まで書き込む
void CArchive::WriteString(const CStringA& u16string) 
{}

CArchive& CArchive::operator << (const CStringA& str)
{
    BYTE l1 = 255;
    WORD l2 = 65535;
    DWORD sz = str.GetLength();
    if( sz < 255 )
    {
        m_file->Write(&sz,sizeof(BYTE));
        m_file->Write((const char*)str,sz);
    }
    else if( sz < 65534 )
    {
        m_file->Write(&l1,sizeof(BYTE));
        m_file->Write(&sz,sizeof(WORD));
        m_file->Write((const char*)str,sz);
    }
    else
    {
        m_file->Write(&l1,sizeof(BYTE));
        m_file->Write(&l2,sizeof(WORD));
        m_file->Write(&sz,sizeof(DWORD));
        m_file->Write((const char*)str,sz);
    }
    return *this;
}

CArchive& CArchive::operator << (const CString& str)
{
    BYTE l1 = 255;
    WORD l2 = 65535;
    WORD bom = 0xfffe;
    DWORD sz = str.GetLength();
    if( sz < 255 )
    {
        m_file->Write(&l1,sizeof(BYTE));
        m_file->Write(&bom,sizeof(WORD));
        m_file->Write(&sz,sizeof(BYTE));
        m_file->Write((const char16_t*)str,sz*sizeof(WORD));
    }
    else if( sz < 65534 )
    {
        m_file->Write(&l1,sizeof(BYTE));
        m_file->Write(&bom,sizeof(WORD));
        m_file->Write(&l1,sizeof(BYTE));
        m_file->Write(&sz,sizeof(WORD));
        m_file->Write((const char16_t*)str,sz*sizeof(WORD));
    }
    else
    {
        m_file->Write(&l1,sizeof(BYTE));
        m_file->Write(&l2,sizeof(WORD));
        m_file->Write(&sz,sizeof(DWORD));
        m_file->Write((const char16_t*)str,sz*sizeof(WORD));
    }
    return *this;
}

CArchive& CArchive::operator << (BYTE by)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&by), sizeof(BYTE));
    }
    return *this;
}

CArchive& CArchive::operator << (WORD w)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&w), sizeof(WORD));
    }
    return *this;
}

CArchive& CArchive::operator << (LONG l)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&l), sizeof(LONG));
    }
    return *this;
}

CArchive& CArchive::operator << (DWORD dw)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&dw), sizeof(DWORD));
    }
    return *this;
}

CArchive& CArchive::operator << (float f)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&f), sizeof(float));
    }
    return *this;
}

CArchive& CArchive::operator << (double d)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&d), sizeof(double));
    }
    return *this;
}

CArchive& CArchive::operator << (int i)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&i), sizeof(int));
    }
    return *this;
}

CArchive& CArchive::operator << (short w)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&w), sizeof(short));
    }
    return *this;
}

CArchive& CArchive::operator << (char ch)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&ch), sizeof(char));
    }
    return *this;
}

CArchive& CArchive::operator << (char16_t ch)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&ch), sizeof(char16_t));
    }
    return *this;
}

CArchive& CArchive::operator << (unsigned u)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&u), sizeof(unsigned));
    }
    return *this;
}

CArchive& CArchive::operator << (bool b)
{
    if(m_file)
    {
        char bb = b;
        m_file->Write(static_cast<void*>(&bb), sizeof(char));
    }
    return *this;
}

CArchive& CArchive::operator << (ULONGLONG ull)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&ull), sizeof(ULONGLONG));
    }
    return *this;
}

CArchive& CArchive::operator << (LONGLONG ll)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&ll), sizeof(LONGLONG));
    }
    return *this;
}

CArchive& CArchive::operator >> (CStringA& str)
{
    size_t sz;
    BYTE l1;
    m_file->Read(static_cast<void*>(&l1),sizeof(BYTE));
    if( l1 < 255 )
    {
        sz = l1 + 1;
        char* lpBuf = new char[sz] { 0 };
        m_file->Read(lpBuf,l1);
        str = lpBuf;
        delete [] lpBuf;
    }
    else
    {
        WORD l2;
        m_file->Read(static_cast<void*>(&l2),sizeof(WORD));
        if( l2 < 65534 )
        {
            sz = l2 + 1;
            char* lpBuf = new char[sz] { 0 };
            m_file->Read(lpBuf,l2);
            str = lpBuf;
            delete [] lpBuf;
        }
        else
        {
            DWORD l3;
            m_file->Read(static_cast<void*>(&l3),sizeof(DWORD));
            sz = l3 + 1;
            char* lpBuf = new char[sz] { 0 };
            m_file->Read(lpBuf,l3);
            str = lpBuf;
            delete [] lpBuf;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (CString& str)
{
    size_t sz;
    BYTE l1;
    m_file->Read(static_cast<void*>(&l1),sizeof(BYTE));
    if( l1 == 255 )
    {
        WORD l2;
        m_file->Read(static_cast<void*>(&l2),sizeof(WORD));
        if( l2 == 65534 )
        {
            m_file->Read(static_cast<void*>(&l1),sizeof(BYTE));
            if( l1 < 255 )
            {
                sz = l1 + 1;
                char16_t* lpBuf = new char16_t[sz] { 0 };
                m_file->Read(lpBuf,l1*sizeof(WORD));
                str = lpBuf;
                delete [] lpBuf;
            }
            else
            {
                m_file->Read(static_cast<void*>(&l2),sizeof(WORD));
                if( l2 < 65534 )
                {
                    sz = l2 + 1;
                    char16_t* lpBuf = new char16_t[sz] { 0 };
                    m_file->Read(lpBuf,l2*sizeof(WORD));
                    str = lpBuf;
                    delete [] lpBuf;
                }
                else
                {
                    DWORD l3;
                    m_file->Read(static_cast<void*>(&l3),sizeof(DWORD));
                    sz = l3 + 1;
                    char16_t* lpBuf = new char16_t[sz] { 0 };
                    m_file->Read(lpBuf,l3*sizeof(WORD));
                    str = lpBuf;
                    delete [] lpBuf;
                }
            }
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (BYTE& by)
{
    if(m_file)
    {
        BYTE data;
        if( m_file->Read(&data, sizeof(BYTE)) == sizeof(BYTE))
        {
            by = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (WORD& w)
{
    if(m_file)
    {
        WORD data;
        if( m_file->Read(&data, sizeof(WORD)) == sizeof(WORD))
        {
            w = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (int& i)
{
    if(m_file)
    {
        int data;
        if( m_file->Read(&data, sizeof(int)) == sizeof(int))
        {
            i = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (LONG& l)
{
    if(m_file)
    {
        LONG data;
        if( m_file->Read(&data, sizeof(LONG)) == sizeof(LONG))
        {
            l = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (DWORD& dw)
{
    if(m_file)
    {
        DWORD data;
        if( m_file->Read(&data, sizeof(DWORD)) == sizeof(DWORD))
        {
            dw = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (float& f)
{
    if(m_file)
    {
        float data;
        if( m_file->Read(&data, sizeof(float)) == sizeof(float))
        {
            f = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (double& d)
{
    if(m_file)
    {
        double data;
        if( m_file->Read(&data, sizeof(double)) == sizeof(double))
        {
            d = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (short& w)
{
    if(m_file)
    {
        short data;
        if( m_file->Read(&data, sizeof(short)) == sizeof(short))
        {
            w = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (char& ch)
{
    if(m_file)
    {
        char data;
        if( m_file->Read(&data, sizeof(char)) == sizeof(char))
        {
            ch = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (char16_t& ch)
{
    if(m_file)
    {
        char16_t data;
        if( m_file->Read(&data, sizeof(char16_t)) == sizeof(char16_t))
        {
            ch = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (unsigned& u)
{
    if(m_file)
    {
        unsigned data;
        if( m_file->Read(&data, sizeof(unsigned)) == sizeof(unsigned))
        {
            u = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (bool& b)
{
    if(m_file)
    {
        BYTE data;
        if( m_file->Read(&data, sizeof(BYTE)) == sizeof(BYTE))
        {
            b = (bool)data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (ULONGLONG& ull)
{
    if(m_file)
    {
        ULONGLONG data;
        if( m_file->Read(&data, sizeof(ULONGLONG)) == sizeof(ULONGLONG))
        {
            ull = data;
        }
    }
    return *this;
}

CArchive& CArchive::operator >> (LONGLONG& ll)
{
    if(m_file)
    {
        LONGLONG data;
        if( m_file->Read(&data, sizeof(LONGLONG)) == sizeof(LONGLONG))
        {
            ll = data;
        }
    }
    return *this;
}
