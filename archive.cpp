/*
    archive.cpp
*/
#include <string>
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

/*

CFile::CFile(LPCTSTR lpszFileName, UINT nOpenFlags)
{

}
void CFile::Close()
{

}
CString CFile::GetFilePath() const
{

}
void CFile::SetPosition(ULONGLONG pos)
{}
ULONGLONG CFile::GetLength() const
{}
ULONGLONG CFile::GetPosition() const
{}
UINT CFile::Read(void* lpBuf, UINT nCount)
{}
ULONGLONG CFile::Seek(LONGLONG lOff,UINT nFrom)
{}
void CFile::Write(const void* lpBuf, UINT nCount)    
{}

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