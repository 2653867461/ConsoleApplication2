#pragma once

#include<Windows.h>
#include<tchar.h>

#define PTR_ADD(type,ptr,value) (type)(((ULONGLONG)ptr)+((ULONGLONG)(value))) 
#define PTR_SUB(type,ptr,value)	(type)(((ULONGLONG)ptr)-((ULONGLONG)(value)))
#define MAXINUM_HEAP_SEGMENT 64

class HEAP_INFO
{
private:
	typedef LPVOID _PHEAP;
	typedef LPVOID _PHEAP_ENTRY;
	typedef LPVOID _PHEAP_SEGMENT;

	enum class _HEAP_OFFSET;
	enum class _HEAP_SEGMENT_OFFSET;
	enum class _HEAP_TYPE:DWORD;
	enum class _HEAP_ENTRY_OFFSET;

	_PHEAP m_heap;
	_PHEAP_SEGMENT m_heapSegment[MAXINUM_HEAP_SEGMENT] = { 0 };
	DWORD m_dwNumSegment = 0;
	
	ULONGLONG m_xorKey[2] = { 0 };

public:
	HEAP_INFO(HANDLE hHeap);
	VOID EnumHeapSegment();
	VOID EnumHeapEnter(_PHEAP_SEGMENT heapSegment);

};
