/**
******************************************************************************
* @file    fixed_vector.h
* @author  Chenxx
* @version V1.0
* @date    2017-5-19
* @brief   This file defines the fixed vector.
******************************************************************************/
//release information:
//2017/10/24 chenxx: add erase() and adjust remove()
//2017/11/15 chenxx: add resize() function

#ifndef FIXED_VECTOR_H
#define FIXED_VECTOR_H
#include <stdint.h>
#include <string.h>

template <typename T, uint16_t N>
class fixed_vector
{
	public:
		
/**
	* @brief  	Constructor
	* @array:	static array for queue
	* @size: 	size of static array
	* @retval 	invalid
	*/
		fixed_vector()
			:size_(0)
		{
			
		}

/**
	* @brief  push an element to the back of the vector
	* @param  obj
	* @retval Success or Not
	*/		
		bool push_back(const T& obj)
		{
			if(size_ == N)
				return false;
			
			static_array_[size_++] = obj;
			return true;
		}
		
/**
	* @brief  find the object and remove from the vector
	* @param  value
	* @retval Success or Not
	*/
		bool remove(const T& obj)
		{
            T* ptr = begin();
            uint16_t dist = 0;
            while (ptr != end())
            {
                if (obj == *ptr)
                {
                    dist++;
                }
                else if (dist > 0)
                {
                    *(ptr - dist) = *ptr;
                }
                ptr++;
            }
            return true;
		}
		
		void clear()	{ size_ = 0; }

/**
	* @brief  find the object and return the pointer
	* @param  value
	* @retval pointer
	*/
		T* find(const T& obj)
		{
			for(int i = 0; i < size_; i++)
			{
				if(obj == static_array_[i])
					return &static_array_[i];
			}
			return NULL;
		}
		
/**
	* @brief  Returns a reference to the element at specified location pos. 
	* 				No bounds checking is performed.
	* @param 	pos: position of the element to return
	* @retval Reference to the requested element.
	*/
		T& operator[](uint16_t pos)
		{
			return static_array_[pos];
		}
		
/**
	* @brief  Returns a reference to the element at specified location pos. 
	* 				No bounds checking is performed.
	* @param 	pos: position of the element to return
	* @retval Reference to the requested element.
	*/
		const T& operator[] (uint16_t pos) const
		{
			return static_array_[pos];
		}
/**
	* @brief  Returns the number of elements in the vector.
	*		  This is the number of actual objects held in the vector, 
	*		  which is not necessarily equal to its storage capacity.
	* @param  None
	* @retval The number of elements in the container.
	*/	
		uint16_t size() { return size_; }
		
		uint16_t capacity() { return N; }

/**
	* @brief  Resizes the container so that it contains n elements.
	* @param  n: New container size, expressed in number of elements.
	* @retval None
	*/
		void resize(uint16_t n) 
		{
   			size_ = n < N ? n : N;
 		}

        T* begin()
        {
            return static_array_;
        }

        T* end()
        {
            return &static_array_[size_];
        }

        T* erase(T* p)
        {
            if (p < begin() || p >= end())
                return NULL;
            uint16_t len = end() - p;
            memmove(p, p + 1, len * sizeof(T));
            size_--;
            return p;
        }

/**
	* @brief  Returns a reference to the element at specified location pos,
	* 				with bounds checking.
	* @param 	pos: position of the element to return
	* @retval Reference to the requested element.
	*/	
	T& at(size_t pos)
	{
		if(!(pos < N))
		{
#ifdef __EXCEPTIONS
			throw static_array_[0];
#else
			while(1);
#endif		
		}
		return static_array_[pos];
	}
	
/**
	* @brief  Returns a reference to the element at specified location pos,
	* 				with bounds checking.
	* @param 	pos: position of the element to return
	* @retval Reference to the requested element.
	*/		
	const T& at(size_t pos) const
	{
		if(!(pos < N))
		{
#ifdef __EXCEPTIONS
			throw static_array_[0];
#else
			while(1);
#endif		
		}
		return static_array_[pos];
	}
	
	private:
		T static_array_[N == 0 ? 1 : N];
		uint16_t size_;
};
#endif
//end of file
