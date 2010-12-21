
#ifndef __loMALLOC_ALLOCATOR_H__
#define __loMALLOC_ALLOCATOR_H__

#include <common/loMallocBase.h>

DEFINE_NAMESPACE(locom)
/**
* @class CloNewAllocator
*
* @brief Defines a class that provided a simple implementation of
* memory allocation.
*
* This class uses the new/delete operators to allocate and free up
* memory.  Please note that the only methods that are supported are
* <malloc>, <calloc>, and <free>. All other methods are no-ops that
* return -1 and set @c errno to <ENOTSUP>.  If you require this
* functionality, please use: CAllocator_lo_Adapter <common/loMalloc
* <common/loLOCAL_MEMORY_POOL, MUTEX> >, which will allow you to use the
* added functionality of bind/find/etc. while using the new/delete
* operators.
*/
class loCOMMON_API CloNewAllocator : public CloAllocator
{
public:
	/// These methods are defined.
	virtual void *malloc (size_t nbytes);
	virtual void *calloc (size_t nbytes, char initial_value = '\0');
	virtual void *calloc (size_t n_elem, size_t elem_size, char initial_value = '\0');
	virtual void free (void *ptr);
};

END_NAMESPACE(locom)

#endif /* __loMALLOC_ALLOCATOR_H__ */
