/*
    archive.h
*/
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#define BOOL    int
#define BYTE    unsigned char
#define WORD    unsigned short
#define INT     int
#define UINT    unsigned int
#define LONG    long
#define ULONG   unsigned long
#define DWORD   ULONG
#define LONGLONG long long
#define ULONGLONG unsigned long long
#define DWORD     ULONG
#define DWORD_PTR ULONGLONG
#define LPSTR  char*
#define LPCSTR const char*
#define LPWSTR  wchar_t*
#define LPCWSTR const wchar_t*

class CAnsiString
{
public: 
    CAnsiString(){}
    CAnsiString(const char* str);
    CAnsiString& operator = (const char* str );
    DWORD GetLength() const;
    operator const char* () const;
private:
    std::string m_str;    
};

class CString
{
public: 
    CString(){}
    CString(const wchar_t* str);
    CString& operator = (const wchar_t* str );
    DWORD GetLength() const;
    operator const wchar_t* () const;
private:
    std::wstring m_str;    
};

class CFile
{
public:
    enum Mode {
       modeRead = 1,
       modeWrite = 2,
    };
    enum Pos {
        begin = 0,
        current = 1,
        end = 2
    };
    CFile(){}
    CFile(LPCSTR fileName, UINT nOpenFlags);
    virtual void Close();
    CAnsiString GetFilePath() const;
    void SetPosition(ULONGLONG pos);
    ULONGLONG GetLength() const;
    ULONGLONG GetPosition();
    virtual UINT Read(void* lpBuf, UINT nCount);
    virtual ULONGLONG Seek(LONGLONG lOff,UINT nFrom);
    virtual void Write(const void* lpBuf, UINT nCount);
private:
    std::ifstream m_in;
    std::ofstream m_out;
    std::string m_name;
};

class CArchive
{
public:
    // Flag values
	enum Mode { store = 0, load = 1, bNoFlushOnDelete = 2, bNoByteSwap = 4 };
    CArchive();
	CArchive(CFile* pFile, UINT nMode, int nBufSize = 4096, void* lpBuf = NULL);
	~CArchive();
    void Close();
    CFile* GetFile() const;
    BOOL IsLoading() const;
    BOOL IsStoring() const;
    UINT Read(void* lpBuf, UINT nMax);
    BOOL ReadString(CAnsiString& rString);
    void Write(const void* lpBuf, UINT nMax);
    void WriteString(const CAnsiString& wString); 
    CArchive& operator << (const CAnsiString& str);
    CArchive& operator << (BYTE by);
    CArchive& operator << (WORD w);
    CArchive& operator << (LONG l);
    CArchive& operator << (DWORD dw);
    CArchive& operator << (float f);
    CArchive& operator << (double d);
    CArchive& operator << (int i);
    CArchive& operator << (short w);
    CArchive& operator << (char ch);
    CArchive& operator << (wchar_t ch);
    CArchive& operator << (unsigned u);
    CArchive& operator << (bool b);
    CArchive& operator << (ULONGLONG ull);
    CArchive& operator << (LONGLONG ll);
    CArchive& operator >> (CAnsiString& str);
    CArchive& operator >> (BYTE& by);
    CArchive& operator >> (WORD& w);
    CArchive& operator >> (int& i);
    CArchive& operator >> (LONG& l);
    CArchive& operator >> (DWORD& dw);
    CArchive& operator >> (float& f);
    CArchive& operator >> (double& d);
    CArchive& operator >> (short& w);
    CArchive& operator >> (char& ch);
    CArchive& operator >> (wchar_t& ch);
    CArchive& operator >> (unsigned& u);
    CArchive& operator >> (bool& b);
    CArchive& operator >> (ULONGLONG& ull);
    CArchive& operator >> (LONGLONG& ll);
protected:
    CFile* m_file;
    UINT m_mode;
};
