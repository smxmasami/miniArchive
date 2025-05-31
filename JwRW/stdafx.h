#pragma once

#include "afx.h"
#include "afxwin.h"
//#include "atlstr.h"
#include <set>
#include <iostream>

#if defined(JWWIO_EXPORTS)
#define EXTFUNC	__declspec(dllexport)
#else
#define EXTFUNC	__declspec(dllimport)
#endif

class OdString
{
public:
    OdString(const wchar_t* t) { str = t; }
    const wchar_t* c_str() { return (const wchar_t*)str; }
private:
    CString str;
};

class OdGePoint2d
{
public:
    OdGePoint2d() { x = y = 0.0; }
    OdGePoint2d(const OdGePoint2d& p) { x = p.x; y = p.y; }
public:
    double x;
    double y;
};

class OdGePoint3d
{
public:
    OdGePoint3d() { x = y = z = 0.0; }
    OdGePoint3d(const OdGePoint3d& p) { x = p.x; y = p.y; z = p.z; }
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

#include "JwDocument.h"
#include "JwwDocument.h"

