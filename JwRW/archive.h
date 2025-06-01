/*
    archive.h
*/
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <list>

//-----------------------------------------------------------
// Windows での型名の再定義
//-----------------------------------------------------------
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
#define LPWSTR  char16_t*
#define LPCWSTR const char16_t*
#define INT_PTR LONGLONG
#define POSITION LONGLONG
#define DECLARE_SERIAL(x)
#define DECLARE_MESSAGE_MAP() 
#define LPCTSTR LPCWSTR
#define TRUE    1
#define FALSE   0
//-----------------------------------------------------------
// CAnsiString UTF8またはShiftJIS文字列
//-----------------------------------------------------------
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

//-----------------------------------------------------------
// CString UTF16(LE)文字列
//-----------------------------------------------------------
class CString
{
public: 
    CString(){}
    CString(const char16_t* str);
    CString& operator = (const char16_t* str );
    void operator += (const char16_t* str );
    operator const char16_t* () const;
    DWORD GetLength() const;
    char16_t GetAt(int p) const;
    void Insert(unsigned int i, const char16_t* t) const;
    void Delete(unsigned int i, unsigned int n = 1) const;
    int Find(const char16_t * t, int p = 0) const;
private:
    std::u16string m_str;    
};
//-----------------------------------------------------------
// CDocument c++ ファイルのカプセル化
//-----------------------------------------------------------
class CObject
{
public:
    CObject(){}
};
//-----------------------------------------------------------
// CDocument c++ ファイルのカプセル化
//-----------------------------------------------------------
class CDocument
{
public:
    CDocument(){}
};
//-----------------------------------------------------------
// CFile c++ ファイルのカプセル化
//-----------------------------------------------------------
class CFile
{
public:
    enum Mode {
       modeRead = 1,
       modeWrite = 2,
       modeCreate = 4
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

//-----------------------------------------------------------
// CArchive MFC CArchive バイナリ形式のファイルストリームを実装
//-----------------------------------------------------------
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
    CArchive& operator << (const CString& str);
    CArchive& operator << (BYTE by);
    CArchive& operator << (WORD w);
    CArchive& operator << (LONG l);
    CArchive& operator << (DWORD dw);
    CArchive& operator << (float f);
    CArchive& operator << (double d);
    CArchive& operator << (int i);
    CArchive& operator << (short w);
    CArchive& operator << (char ch);
    CArchive& operator << (char16_t ch);
    CArchive& operator << (unsigned u);
    CArchive& operator << (bool b);
    CArchive& operator << (ULONGLONG ull);
    CArchive& operator << (LONGLONG ll);
    CArchive& operator >> (CAnsiString& str);
    CArchive& operator >> (CString& str);
    CArchive& operator >> (BYTE& by);
    CArchive& operator >> (WORD& w);
    CArchive& operator >> (int& i);
    CArchive& operator >> (LONG& l);
    CArchive& operator >> (DWORD& dw);
    CArchive& operator >> (float& f);
    CArchive& operator >> (double& d);
    CArchive& operator >> (short& w);
    CArchive& operator >> (char& ch);
    CArchive& operator >> (char16_t& ch);
    CArchive& operator >> (unsigned& u);
    CArchive& operator >> (bool& b);
    CArchive& operator >> (ULONGLONG& ull);
    CArchive& operator >> (LONGLONG& ll);
protected:
    CFile* m_file;
    UINT m_mode;
public:
    CDocument* m_pDocument;
};
