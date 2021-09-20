#include"HeapInfo.h"

enum class HEAP_INFO::_HEAP_OFFSET
{
	EncodeFlagMask=0x07c,
	Encoding=0x080,

	SegmentList=0x120,
	FreeLists=0x150
};

enum class HEAP_INFO::_HEAP_SEGMENT_OFFSET
{
	SegmentSignature = 0x10,
	SegmentListEntry = 0x18,

	Heap = 0x28,

	FirstEntry = 0x040,
	LastValidEntry = 0x048,
	NumberOfUnCommittedPages=0x50

};

enum class HEAP_INFO::_HEAP_TYPE:DWORD {
	NT_HEAP=0xFFEEFFEE,
	SEGMENT_HEAP=0xDDEEDDEE
}HEAP_TYPE;

enum class HEAP_INFO::_HEAP_ENTRY_OFFSET {
	Size = 0x008,
	flags = 0x00a
};

VOID HEAP_INFO::EnumHeapSegment()
{
	_PHEAP_SEGMENT startSegment = NULL, endSegment = NULL, currentSegment = NULL;
	LIST_ENTRY* segmentList = NULL;

	currentSegment = startSegment = m_heap;
	segmentList = PTR_ADD(PLIST_ENTRY, m_heap, _HEAP_SEGMENT_OFFSET::SegmentListEntry);
	endSegment = PTR_ADD(_PHEAP_SEGMENT, m_heap, _HEAP_OFFSET::SegmentList);
	
	do {
		m_heapSegment[m_dwNumSegment] = currentSegment;
		currentSegment = segmentList->Flink;
		m_dwNumSegment += 1;
	} while (currentSegment != endSegment );
}

VOID HEAP_INFO::EnumHeapEnter(_PHEAP_SEGMENT heapSegment)
{
	LPVOID lpEnd = NULL;
	_PHEAP_ENTRY endEntry = NULL, currentEntry = NULL;
	
	ULONGLONG data[2]; DWORD dwUnCommittedPages = 0;
	ULONGLONG blockSize = 0; UCHAR blockFlag = 0;

	dwUnCommittedPages = *PTR_ADD(DWORD*, heapSegment, _HEAP_SEGMENT_OFFSET::NumberOfUnCommittedPages);
	lpEnd = *PTR_ADD(_PHEAP_ENTRY*, heapSegment, (ULONGLONG)_HEAP_SEGMENT_OFFSET::LastValidEntry);
	endEntry = PTR_SUB(_PHEAP_ENTRY, lpEnd, ((ULONGLONG)dwUnCommittedPages * 0x1000));
	_tprintf(L"%p\n", endEntry);
	_tprintf_s(L"heapStartAddress:0x%p		heapEndAddress:0x%p		unCommitSize:%0x\n", heapSegment, lpEnd, ((ULONGLONG)dwUnCommittedPages * 0x1000));
	for (currentEntry = heapSegment; currentEntry != endEntry;currentEntry=PTR_ADD(_PHEAP_ENTRY,currentEntry,blockSize))
	{
		memset(data, 0, 0x10);
		memcpy_s(data, 0x10, currentEntry, 0x10);

		data[0] ^= m_xorKey[0];
		data[1] ^= m_xorKey[1];

		blockFlag = *PTR_ADD(UCHAR*, data, _HEAP_ENTRY_OFFSET::flags);
		blockSize = *PTR_ADD(WORD*, data, _HEAP_ENTRY_OFFSET::Size);

		blockSize *= 0x10;

		_tprintf_s(L"startAddress:0x%p	blockSize:0x%p	flag:0x%p\n", currentEntry, blockSize , blockFlag);
		
	}
}

HEAP_INFO::HEAP_INFO(HANDLE hHeap) :m_heap(hHeap) {
	LPVOID lpKey = PTR_ADD(LPVOID, m_heap, _HEAP_OFFSET::Encoding);
	memcpy_s(m_xorKey, 0x10, lpKey, 0x10);
};


HEAP_INFO heapInfo(GetProcessHeap());

int main()
{
	heapInfo.EnumHeapSegment();
	heapInfo.EnumHeapEnter(GetProcessHeap());
}