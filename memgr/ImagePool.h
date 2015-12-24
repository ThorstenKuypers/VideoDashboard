#ifndef _IMAGE_POOL_H_
#define _IMAGE_POOL_H_

#include <memory>
#include <exception>
#include <vector>
#include <mutex>

namespace MemoryManager
{
	// This structure defines a memory block
	struct MemoryBlock
	{
		void* baseAddress; // start address of memory block
		unsigned char inUse; // block allocated (in use) or not (free)
	};

	// This class is the first image pool used in libLDF rendering
	// It provides functions for allocating/freeing pixel buffers and the corresponding 
	// storage
	class CImagePool
	{
	public:

		// size - size of contingeous bytes allocated (heap size)
		// blockSize - number of bytes per block
		CImagePool() {}
		CImagePool(size_t size, size_t blockSize);
		virtual ~CImagePool();

		void* Alloc();
		void Free(void* ptr);

		CImagePool& operator=(CImagePool& o)
		{
			this->_heap = o._heap;
			this->_heapBegin = o._heapBegin;
			this->_heapEnd = o._heapEnd;
			this->_memBlocks = o._memBlocks;
			this->_numberOfBlocks = o._numberOfBlocks;

			return *this;
		}

	private:
		unsigned long _heapBegin;
		unsigned long _heapEnd;

		void* _heap; // the memory region allocated as heap
		size_t _numberOfBlocks; // number of available memory blocks

		std::vector<MemoryBlock> _memBlocks;
	};
}

#endif // _IMAGE_POOL_H_