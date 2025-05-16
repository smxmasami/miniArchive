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

CString& CString::operator = (const wchar_t* str )
{
    m_str = str;
    return *this;
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

BOOL CArchive::ReadString(CAnsiString& rString)
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

void CArchive::WriteString(const CAnsiString& wstring) 
{}

CArchive& CArchive::operator << (const CAnsiString& str)
{
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

CArchive& CArchive::operator << (wchar_t ch)
{
    if(m_file)
    {
        m_file->Write(static_cast<void*>(&ch), sizeof(wchar_t));
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

CArchive& CArchive::operator >> (CAnsiString& str)
{
    return *this;
}

CArchive& CArchive::operator >> (BYTE& by)
{
    return *this;
}

CArchive& CArchive::operator >> (WORD& w)
{
    return *this;
}

CArchive& CArchive::operator >> (int& i)
{
    return *this;
}

CArchive& CArchive::operator >> (LONG& l)
{
    return *this;
}

CArchive& CArchive::operator >> (DWORD& dw)
{
    return *this;
}

CArchive& CArchive::operator >> (float& f)
{
    return *this;
}

CArchive& CArchive::operator >> (double& d)
{
    return *this;
}

CArchive& CArchive::operator >> (short& w)
{
    return *this;
}

CArchive& CArchive::operator >> (char& ch)
{
    return *this;
}

CArchive& CArchive::operator >> (wchar_t& ch)
{
    return *this;
}

CArchive& CArchive::operator >> (unsigned& u)
{
    return *this;
}

CArchive& CArchive::operator >> (bool& b)
{
    return *this;
}

CArchive& CArchive::operator >> (ULONGLONG& ull)
{
    return *this;
}

CArchive& CArchive::operator >> (LONGLONG& ll)
{
    return *this;
}
