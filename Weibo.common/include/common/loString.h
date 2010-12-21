#ifndef __loSSTRING_H__
#define __loSSTRING_H__

#include "loStringfwd.h"

#include "loMallocBase.h"

#include "loOSString.h"

// declare CloWString, et. al.
#include "string-template-def-unichar.h"
#include "loStringT.h"
#include "string-template-undef.h"

  // declare loCString, et. al.
#include "string-template-def-char.h"
#include "loStringT.h"
#include "string-template-undef.h"


DEFINE_NAMESPACE(locom)

// -----------------------------------------------------------------

/**
* @class CloString
*
* @brief A very Simple String CloString class.  This is not a
* general-purpose string class, and you should probably consider
* using loCString is you don't understand why this class
* exists...
*
* This class is optimized for efficiency, so it doesn't provide
* any internal locking.
* CAUTION: This class is only intended for use with applications
* that understand how it works.  In particular, its destructor
* does not deallocate its memory when it is destroyed...  We need
* this class since the loMap_Manager requires an object that
* supports the operator == and operator !=.  This class uses an
* CloAllocator to allocate memory.  The user can make this a
* persistant class by providing an CloAllocator with a
* persistable memory pool.
*/
class loCOMMON_API CloString
{
public:

	typedef CloAllocator::size_type size_type;

	/// No position constant
	static const size_type npos;

	/// Default constructor.
	CloString (CloAllocator *alloc = 0);

	/// Constructor that copies @a s into dynamically allocated memory.
	CloString (const char *s, CloAllocator *alloc = 0);

	/// Constructor that copies @a len chars of  @s  into dynamically
	/// allocated memory (will NUL terminate the result).
	CloString (const char *s, size_type len, CloAllocator *alloc = 0);

	/// Copy constructor.
	CloString (const CloString &);

	/// Constructor that copies @a c into dynamically allocated memory.
	CloString (char c, CloAllocator *alloc = 0);

	/// Default destructor.
	~CloString (void);

	/// Return the <slot'th> character in the string (doesn't perform
	/// bounds checking).
	char operator [] (size_type slot) const;

	/// Return the <slot'th> character by reference in the string
	/// (doesn't perform bounds checking).
	char &operator [] (size_type slot);

	/// Assignment operator (does copy memory).
	CloString &operator = (const CloString &);

	/**
	* Return a substring given an offset and length, if length == npos
	* use rest of str return empty substring if offset or offset/length
	* are invalid
	*/
	CloString substring (size_type offset, size_type length = npos) const;

	/// Same as substring
	CloString substr (size_type offset, size_type length = npos) const;

	/// Return the length of the string.
	size_type length (void) const;

	/// Set the underlying pointer.  Since this does not copy memory or
	/// delete existing memory use with extreme caution!!!
	void rep (char *s);

	/// Get the underlying pointer.
	const char *rep (void) const;

	/// Get the underlying pointer.
	const char *fast_rep (void) const;

	/// Same as STL String's <c_str> and <fast_rep>.
	const char *c_str (void) const;

	/// Comparison operator that will match substrings.  Returns the
	/// slot of the first location that matches, else @c npos.
	size_type strstr (const CloString &s) const;

	/// Find <str> starting at pos.  Returns the slot of the first
	/// location that matches (will be >= pos), else npos.
	size_type find (const CloString &str, size_type pos = 0) const;

	/// Find <s> starting at pos.  Returns the slot of the first
	/// location that matches (will be >= pos), else npos.
	size_type find (const char *s, size_type pos = 0) const;

	/// Find <c> starting at pos.  Returns the slot of the first
	/// location that matches (will be >= pos), else npos.
	size_type find (char c, size_type pos = 0) const;

	/// Find <c> starting at pos (counting from the end).  Returns the
	/// slot of the first location that matches, else npos.
	size_type rfind (char c, size_type pos = npos) const;

	/// Equality comparison operator (must match entire string).
	bool operator == (const CloString &s) const;

	/// Less than comparison operator.
	bool operator < (const CloString &s) const;

	/// Greater than comparison operator.
	bool operator > (const CloString &s) const;

	/// Inequality comparison operator.
	bool operator != (const CloString &s) const;

	/// Performs a <strcmp>-style comparison.
	int compare (const CloString &s) const;

	/// Dump the state of an object.
	void dump (void) const;

private:
	/// Pointer to a memory allocator.
	CloAllocator *allocator_;

	/// Length of the CloString (not counting the trailing '\0').
	size_type len_;

	/// Pointer to data.
	char *rep_;
};

#if defined (__INLINE_lo__)
#include "loString.inl"
#endif /* __INLINE_lo__ */

END_NAMESPACE(locom)

#endif /* __loSSTRING_H__ */
