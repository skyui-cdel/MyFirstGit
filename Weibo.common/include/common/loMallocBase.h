#ifndef __loMALLOC_BASE_H__
#define __loMALLOC_BASE_H__

#include <common/loCommon.h>
// The definition of this class is located in Malloc.cpp.

DEFINE_NAMESPACE(locom)

/**
 * @class CloAllocator
 *
 * @brief Interface for a dynamic memory allocator that uses inheritance
 * and dynamic binding to provide extensible mechanisms for
 * allocating and deallocating memory.
 */
class loCOMMON_API CloAllocator
{
public:

  /// Unsigned integer type used for specifying memory block lengths.
  typedef size_t size_type;

  // = Memory Management

  /// Get pointer to a default CloAllocator.
  static CloAllocator *instance (void);

  /// "No-op" constructor (needed to make certain compilers happy).
  CloAllocator (void);

  /// Virtual destructor
  virtual ~CloAllocator (void);

  /// Allocate @a nbytes, but don't give them any initial value.
  virtual void *malloc (size_type nbytes) = 0;

  /// Allocate @a nbytes, giving them @a initial_value.
  virtual void *calloc (size_type nbytes, char initial_value = '\0') = 0;

  /// Allocate <n_elem> each of size @a elem_size, giving them
  /// @a initial_value.
  virtual void *calloc (size_type n_elem, size_type elem_size, char initial_value = '\0') = 0;

  /// Free <ptr> (must have been allocated by <CloAllocator::malloc>).
  virtual void free (void *ptr) = 0;
};

END_NAMESPACE(locom)

#define NEW_RETURN_lo(DEST , SRC , RET)\
	do{\
	DEST = new SRC;\
	if( !DEST ) return RET;\
	}while(0)

#define ALLOCATOR_lo(DEST , SRC )\
	do{\
	DEST = SRC;\
	if( !DEST ) return;\
	}while(0)


#define ALLOCATOR_RETURN_lo(DEST , SRC,RET)\
	do{\
	DEST = SRC;\
	if( !DEST ) return RET;\
	}while(0)

#endif /* __loMALLOC_BASE_H__ */
