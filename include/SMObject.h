#ifndef SMOBJECT_H
#define SMOBJECT_H

#include <Windows.h>
#include <tchar.h>
//Code framework by Associate Professor Jay Katupitiya (c) 2022

#define SM_NO_ERROR      0
#define SM_CREATE_ERROR -1
#define SM_OPEN_EOOR    -2
#define SM_ACCESS_ERROR -3

class SMObject
{
	HANDLE CreateHandle;
	HANDLE AccessHandle;
	TCHAR* szName;
	int Size;

public:
	void* pData;

public:
	SMObject();
	SMObject(TCHAR* szname, int size);
	~SMObject();
	int SMCreate();
	int SMAccess();
};
#endif
