#pragma once

/*
 * Written by Eric Hollas
 *
 * Master::Memory::MemoryAllocator
 * 
 * A simple free list that reuses memory in blocks of sizeof(typ)
 * 
 */


#include "Error.h"

namespace Master {
	namespace Memory {
		template<class typ>
		class MemoryAllocator {
		public:
			MemoryAllocator() {
				m_type_size = sizeof(typ);
				p_unused_blocks = nullptr;
			}
			~MemoryAllocator() {
				Unused* curr;
				while (p_unused_blocks != nullptr) {
					curr = p_unused_blocks;
					p_unused_blocks = p_unused_blocks->next;
					delete curr;
				}
			}
			// this list will be defined statically, thus delete copy constructor and assignment operator
			MemoryAllocator(const MemoryAllocator&) = delete;
			MemoryAllocator& operator=(const MemoryAllocator&) = delete;


			// the usual alloc function,
			// when there is no memory available it calls RequestMemory to get get more memory
			inline void* alloc() throw (Master::Error::MasterError) {
				try {
					if (p_unused_blocks == nullptr)
						RequestMoreMemory();
				}
				catch (Master::Error::MasterError& err) {
					throw Master::Error::MasterError(err, L"Master::Error::MemoryAllocator::alloc");
				}

				void* data = static_cast<void*>(p_unused_blocks);
				p_unused_blocks = p_unused_blocks->next;

				return data;
			}

			// recycles the memory to be deleted into a list to be reused
			inline void dealloc(void* data) {
				Unused* replacement = static_cast<Unused*>(data);
				replacement->next = p_unused_blocks;
				p_unused_blocks = replacement;
			}
		private:
			struct Unused {
				Unused* next;
			};

			const int c_pool_size = 32;
			size_t m_type_size;
			Unused* p_unused_blocks;

			// requests more memory with new and then replenishes the list of unused memory
			// with the newly created memory into blocks
			// requests c_pool_size number of blocks
			void RequestMoreMemory() throw (Master::Error::MasterError) {
				Unused* request = reinterpret_cast<Unused*>(new char[m_type_size]);

				try {
					Master::Error::ThrowIfFail(L"Failed to allocate memory", request == nullptr);
				}
				catch (Master::Error::MasterError& err) {
					throw Master::Error::MasterError(err, L"", L"");
				}

				p_unused_blocks = request;

				int count_partition = c_pool_size - 1;
				for (int i = 0; i < count_partition; i++) {
					request->next = reinterpret_cast<Unused*>(new char[m_type_size]);
					request = request->next;
				}
				request->next = nullptr;
			}

		};
	}
}