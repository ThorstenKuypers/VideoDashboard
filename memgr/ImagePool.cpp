#include "ImagePool.h"

using namespace MemoryManager;

CImagePool::CImagePool(size_t size, size_t blockSize) :
_heap(nullptr),
_numberOfBlocks(0),
_memBlocks()
{
	// allocate the heap block
	try {
		_heap = (void*)new unsigned char[size];
		memset(_heap, 0, size);

		_heapBegin = (unsigned long)_heap;
		_heapEnd = (unsigned long)((unsigned long)_heap + size);

		size_t r = size % blockSize; // check if size is a multiple of block size
		if (r > 0)
		{
			throw std::exception("heap size must be a multiple of block size!");
		}
		else
		{
			_numberOfBlocks = size / blockSize;
			_memBlocks = std::vector<MemoryBlock>(_numberOfBlocks);

			for (size_t i = 0; i < _numberOfBlocks; i++)
			{
				_memBlocks[i].baseAddress = ((unsigned char*)_heap + (i*blockSize));
				_memBlocks[i].inUse = 0;
			}
		}
	}
	catch (std::exception e)
	{
		throw e;
	}

#ifdef _DEBUG
	int x = 0;
#endif
}


CImagePool::~CImagePool()
{
	// free the heap block
	if (_heap != nullptr)
	{
		delete[] _heap;
		_heap = nullptr;
	}
}

void* CImagePool::Alloc()
{
	//std::unique_lock<std::mutex> lock;
	// find first free(inUse == false) block
	for (size_t i = 0; i < _numberOfBlocks; i++)
	{
		if (_memBlocks[i].inUse == 0)
		{
			_memBlocks[i].inUse = 1;
			return _memBlocks[i].baseAddress;
		}
	}

	return nullptr;
}

void CImagePool::Free(void* ptr)
{
	//std::unique_lock<std::mutex> lock;
	if (ptr != nullptr)
	{
		if ((unsigned long)ptr >= _heapBegin && (unsigned long)ptr <= _heapEnd)
		{
			// mark block as free
			for (size_t i = 0; i < _numberOfBlocks; i++)
			{
				if (_memBlocks[i].baseAddress == ptr)
					_memBlocks[i].inUse = 0;
			}
		}
	}
}