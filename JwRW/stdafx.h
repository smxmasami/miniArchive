#pragma once

#ifdef linux
#include "archive.h"
#else
#include "afx.h"
#include "afxwin.h"
//#include "atlstr.h"
#endif
#include <set>
#include <iostream>

#ifdef linux
#define EXTFUNC 
#define _T(x)   (x)    
#else
#if defined(JWWIO_EXPORTS)
#define EXTFUNC	__declspec(dllexport)
#else
#define EXTFUNC	__declspec(dllimport)
#endif
#endif

#ifdef linux
class OdString
{
public:
    OdString(const char16_t * t) { str = t; }
    OdString(const char * t) {str = t;}
    const char16_t* c_str() { return (const char16_t*)str; }
    char16_t operator [] (int p) const { return str.GetAt(p); }
    operator const char16_t* () { return (const char16_t*)str; }
    void operator += (const char16_t* t) { str += t; }
    void operator += (const char* t) {str += t;}
    void operator += (OdString& t) { str += t.c_str(); }
    void insert(unsigned int i, const char16_t* t) { str.Insert(i, t); }
    void deleteChars(unsigned int i, unsigned int n=1) { str.Delete(i, n); }
    int find(const char16_t *t, int i = 0) const { return str.Find(t, i); }
private:
    CString str;
};
#else
class OdString
{
public:
    OdString(const wchar_t* t) { str = t; }
    const wchar_t* c_str() { return (const wchar_t*)str; }
    wchar_t operator [] (int p) const { return str.GetAt(p); }
    operator const wchar_t* () { return (const wchar_t*)str; }
    void operator += (const wchar_t* t) { str += t; }
    void operator += (OdString& t) { str += t.c_str(); }
    void insert(unsigned int i, const wchar_t* t) { str.Insert(i, t); }
    void deleteChars(unsigned int i, unsigned int n=1) { str.Delete(i, n); }
    int find(const wchar_t* t, int i = 0) const { return str.Find(t, i); }
private:
    CString str;
};
#endif

class OdGePoint2d
{
public:
    OdGePoint2d() { x = y = 0.0; }
    OdGePoint2d(const OdGePoint2d& p) { x = p.x; y = p.y; }
    OdGePoint2d(const double x, const double y) { this->x = x; this->y = y; }
    OdGePoint2d& operator /= (const double d) { x /= d; y /= d; return *this; }
public:
    double x;
    double y;
};

class OdGePoint3d
{
public:
    OdGePoint3d() { x = y = z = 0.0; }
    OdGePoint3d(const OdGePoint3d& p) { x = p.x; y = p.y; z = p.z; }
    OdGePoint3d(const double x, const double y, const double z) { this->x = x; this->y = y; this->z = z; }
public:
    double x;
    double y;
    double z;
};

class OdGeScale3d
{
public:
    OdGeScale3d() { sx = sy = sz = 1.0; }
    OdGeScale3d(const OdGeScale3d& s) { sx = s.sx; sy = s.sy; sz = s.sz; }
public:
    double sx;
    double sy;
    double sz;
};

#define OdaPI 3.141592
#define OdaToRadian(x) ((x)/180.0*OdaPI)
#define OdaToDegree(x) ((x)*180.0/OdaPI)
bool OdEqual(double x, double y, double f = 1e-10);

#include "JwDocument.h"
#include "JwwDocument.h"

