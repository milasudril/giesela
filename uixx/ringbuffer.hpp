//@	{"targets":[{"name":"ringbuffer.hpp","type":"include"}]}

#ifndef UIXX_RINGBUFFER_HPP
#define UIXX_RINGBUFFER_HPP

#include "nextpow2.hpp"
#include <memory>
#include <cassert>

namespace UIxx
	{
	template<class T,class OffsetType>
	class RingBuffer
		{
		public:
			typedef T value_type;

			explicit RingBuffer(size_t N):
				m_offset_read(0),m_offset_write(0),m_data(new T[Nextpow2<size_t>(N)] )
				{static_assert(sizeof(T)<=sizeof(size_t),"Type not compatible");}

			void push_back(T x) noexcept
				{
				assert(!full());
				auto owr=m_offset_write;
				m_data[owr%capacity()]=x;
				++m_offset_write;
				}

			T pop_front() noexcept
				{
				assert(!empty());
				auto ore=m_offset_read;
				auto ret=m_data[ore%capacity()];
				++m_offset_read;
				return ret;
				}

			bool full() const noexcept
				{return length()==size_t(capacity());}

			bool empty() const noexcept
				{return m_offset_read==m_offset_write;}

			Nextpow2<size_t> capacity() const noexcept
				{return m_capacity;}

			Nextpow2<size_t> capacityMax() const noexcept
				{return capacity();}

			size_t length() const noexcept
				{return m_offset_write - m_offset_read;}

			void clear() noexcept
				{
				m_offset_read=0;
				m_offset_write=0;
				}

		private:
			OffsetType m_offset_read;
			OffsetType m_offset_write;
			std::unique_ptr<T[]> m_data;
			Nextpow2<size_t> m_capacity;
		};
	}

#endif
