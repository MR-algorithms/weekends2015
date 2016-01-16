/********************************************************************
	Filename: 	macros.h
	Created:	2009/06/15
	Author:		Yang Guang, gyang@phy.ecnu.edu.cn

	Purpose:	
	
	All right reserved. 2009 -
*********************************************************************/
#ifndef macros_h__
#define macros_h__

#include <locale.h>

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC2__ __FILE__ "("__STR1__(__LINE__)") : "

#define BUG(x) __pragma(message(__LOC2__  "Known bug - "__STR1__(x)" in "__FUNCTION__"()"))
#define NOT_IMPLEMENTED BUG("Not implemented.");TRACEX(_T("Error, function called has not been implemented."))

#ifdef _UNICODE
#define TRACEX(A) setlocale(LC_ALL, "chs"); TRACE(_T("%S: ")A _T("\n"), __FUNCTION__ )
#define TRACEX1(A, B) setlocale(LC_ALL, "chs"); TRACE(_T("%S: ") A _T("\n"), __FUNCTION__, B)
#define TRACEX2(A, B, C) setlocale(LC_ALL, "chs"); TRACE(_T("%S: ") A _T("\n"), __FUNCTION__, B, C)
#define TRACEX3(A, B, C, D) setlocale(LC_ALL, "chs"); TRACE(_T("%S: ") A _T("\n"), __FUNCTION__, B, C, D)
#else
#define TRACEX(A) setlocale(LC_ALL, "chs"); TRACE(_T("%s: ")A _T("\n"), __FUNCTION__ )
#define TRACEX1(A, B) setlocale(LC_ALL, "chs"); TRACE(_T("%s: ") A _T("\n"), __FUNCTION__, B)
#define TRACEX2(A, B, C) setlocale(LC_ALL, "chs"); TRACE(_T("%s: ") A _T("\n"), __FUNCTION__, B, C)
#define TRACEX3(A, B, C, D) setlocale(LC_ALL, "chs"); TRACE(_T("%s: ") A _T("\n"), __FUNCTION__, B, C, D)
#endif

#define TRACE_CALL_COUNT static int count = 0; TRACEX1(_T("%d"), count++);

#endif // macros_h__
