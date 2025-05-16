/*
    archive.cpp
*/
#include "archive.h"

CAnsiString::CAnsiString(const char* str)
{
    m_str = std::string(str);
}

CAnsiString& CAnsiString::operator = (const char* str )
{
    m_str = str;
    return *this;
}

CString::CString(const wchar_t* str)
{
    m_str = std::wstring(str);
}

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

CAnsiString CFile::GetFilePath() const
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

/*
CArchive::CArchive(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL)
{}
CArchive::~CArchive()
{}
void CArchive::Close()
{}
CFile* CArchive::GetFile() const
{}
BOOL CArchive::IsLoading() const
{}
BOOL CArchive::IsStoring() const
{}
UINT CArchive::Read(void* lpBuf, UINT nMax)
{}

BOOL CArchive::ReadString(CString& rString)
{}

LPTSTR CArchive::ReadString(LPTSTR lpsz, UINT nMax) 
{}

void CArchive::Write(const void* lpBuf, UINT nMax)
{}

void CArchive::WriteString(LPCTSTR lpsz) 
{}

CArchive& CArchive::operator<<(const CString& str)
{
    return *this;
}

CArchive& CArchive::operator<<(BYTE by)
{
    return *this;
}

CArchive& CArchive::operator<<(WORD w)
{
    return *this;
}

CArchive& CArchive::operator<<(LONG l)
{
    return *this;
}

CArchive& CArchive::operator<<(DWORD dw)
{
    return *this;
}

CArchive& CArchive::operator<<(float f)
{
    return *this;
}

CArchive& CArchive::operator<<(double d)
{
    return *this;
}

CArchive& CArchive::operator<<(int i)
{
    return *this;
}

CArchive& CArchive::operator<<(short w)
{
    return *this;
}

CArchive& CArchive::operator<<(char ch)
{
    return *this;
}

CArchive& CArchive::operator<<(wchar_t ch)
{
    return *this;
}

CArchive& CArchive::operator<<(unsigned u)
{
    return *this;
}

CArchive& CArchive::operator<<(bool b)
{
    return *this;
}

CArchive& CArchive::operator<<(ULONGLONG dwdw)
{
    return *this;
}

CArchive& CArchive::operator<<(LONGLONG dwdw)
{
    return *this;
}

CArchive& CArchive::operator>>(CString& str)
{
    return *this;
}

CArchive& CArchive::operator>>(BYTE& by)
{
    return *this;
}

CArchive& CArchive::operator>>(WORD& w)
{
    return *this;
}

CArchive& CArchive::Archive::operator>>(int& i)
{
    return *this;
}

CArchive& CArchive::Archive::operator>>(LONG& l)
{
    return *this;
}

CArchive& CArchive::operator>>(DWORD& dw)
{
    return *this;
}

CArchive& CArchive::operator>>(float& f)
{
    return *this;
}

CArchive& CArchive::operator>>(double& d)
{
    return *this;
}

CArchive& CArchive::operator>>(short& w)
{
    return *this;
}

CArchive& CArchive::operator>>(char& ch)
{
    return *this;
}

CArchive& CArchive::operator>>(wchar_t& ch)
{
    return *this;
}

CArchive& CArchive::operator>>(unsigned& u)
{
    return *this;
}

CArchive& CArchive::operator>>(bool& b)
{
    return *this;
}

CArchive& CArchive::operator>>(ULONGLONG& dwdw)
{
    return *this;
}

CArchive& CArchive::operator>>(LONGLONG& dwdw)
{
    return *this;
}

*/