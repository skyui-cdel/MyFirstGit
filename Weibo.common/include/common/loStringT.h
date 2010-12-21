DEFINE_NAMESPACE(locom)

class loCOMMON_API String_Base_lo
{
public:
    typedef size_t             size_type;
    typedef CharT              char_type;
	typedef String_Base_lo     self_type;

	static size_type const npos;
	/**
	*  Default constructor.
	*
	*  @param the_allocator CloAllocator associated with string
	*  @return Default String_Base_lo string.
	*/
	String_Base_lo (CloAllocator *the_allocator = 0);

	/**
	* Constructor that copies @a s into dynamically allocated memory.
	*
	* if release == true then a new buffer is allocated internally, and
	*   s is copied to the internal buffer.
	* if release == false then the s buffer is used directly. If s == 0
	*   then it will _not_ be used, and instead the internal buffer
	*   is set to NULL_String_.
	*
	* @param s Zero terminated input string
	* @param the_allocator CloAllocator associated with string
	* @param release Allocator responsible(true)/not reponsible(false) for
	*    freeing memory.
	* @return String_Base_lo containing const char_type *s
	*/
	String_Base_lo (const char_type *s,CloAllocator *the_allocator = 0,bool release = true);

	/**
	* Constructor that copies @a len CHARs of @a s into dynamically
	* allocated memory (will zero terminate the result).
	*
	* if release == true then a new buffer is allocated internally.
	*   s is copied to the internal buffer.
	* if release == false then the s buffer is used directly. If s == 0
	*   then it will _not_ be used, and instead the internal buffer
	*   is set to NULL_String_.
	*
	* @param s Non-zero terminated input string
	* @param len Length of non-zero terminated input string
	* @param the_allocator CloAllocator associated with string
	* @param release Allocator responsible(true)/not reponsible(false) for
	*    freeing memory.
	* @return String_Base_lo containing const char_type *s
	*/
	String_Base_lo (const char_type *s,size_type len,CloAllocator *the_allocator = 0,	bool release = true);

	/**
	*  Copy constructor.
	*
	*  @param s Input String_Base_lo string to copy
	*  @return Copy of input string @a s
	*/
	String_Base_lo (const self_type &s);

	/**
	*  Constructor that copies @a c into dynamically allocated memory.
	*
	*  @param c Single input character.
	*  @param the_allocator CloAllocator associated with string
	*  @return String_Base_lo containing char_type 'c'
	*/
	String_Base_lo (char_type c, CloAllocator *the_allocator = 0);

	/**
	*  Constructor that allocates a len long string.
	*
	*  Warning : This constructor was incorrectly documented in the past.
	*  It simply calls resize(len, c).
	*  It is probably not advisable to use the second parameter. See
	*  resize() for more information.
	*
	*  @param len Amount of space to reserve for the string.
	*  @param c The array is filled with c's
	*  @param the_allocator CloAllocator associated with string
	*  @return Empty String_Base_lo with room for len CHARs
	*/
	String_Base_lo (size_type len,	char_type c = 0,CloAllocator *the_allocator = 0);

	/**
	*  Deletes the memory...
	*/
	virtual ~String_Base_lo (void);

	/**
	* Return the <slot'th> character in the string (doesn't perform
	* bounds checking).
	*
	* @param slot Index of the desired character
	* @return The character at index @a slot
	*/
	const char_type & operator[] (size_type slot) const;

	/**
	* Return the <slot'th> character by reference in the string
	* (doesn't perform bounds checking).
	*
	* @param slot Index of the desired character
	* @return The character at index @a slot
	*/
	char_type & operator[] (size_type slot);

	/**
	*  Assignment operator (does copy memory).
	*
	*  @param s Input null-terminated char_type string to assign to this object.
	*  @return Return a copy of the this string.
	*/
	String_Base_lo &operator = (const char_type * s);

	/**
	*  Assignment operator (does copy memory).
	*
	*  @param s Input String_Base_lo string to assign to this object.
	*  @return Return a copy of the this string.
	*/
	String_Base_lo &operator = (const self_type &s);

	/**
	*  Assignment alternative method (does not copy memory).
	*
	*  @param s Input String_Base_lo string to assign to this object.
	*  @return Return this string.
	*/
	String_Base_lo  &assign_nocopy (const self_type &s);

	/**
	* Copy @a s into this @a String_Base_lo.
	*
	* If release == true then a new buffer is allocated internally if the
	*   existing one is not big enough to hold s. If the existing
	*   buffer is big enough, then it will be used. This means that
	*   set(*, 1) can be illegal when the string is constructed with a
	*   const char*. (e.g. String_Base_lo("test", 0, false)).
	*
	* if release == false then the s buffer is used directly, and any
	*   existing buffer is destroyed. If s == 0 then it will _not_ be
	*   used, and instead the internal buffer is set to NULL_String_.
	*
	* @param s Null terminated input string
	* @param release Allocator responsible(true)/not reponsible(false) for
	*    freeing memory.
	*/
	void set (const char_type * s, bool release = true);

	/**
	*  Copy @a len bytes of @a s (will zero terminate the result).
	*
	* If release == true then a new buffer is allocated internally if the
	*   existing one is not big enough to hold s. If the existing
	*   buffer is big enough, then it will be used. This means that
	*   set(*, *, 1) is illegal when the string is constructed with a
	*   non-owned const char*. (e.g. String_Base_lo("test", 0, 0))
	*
	* If release == false then the s buffer is used directly, and any
	*   existing buffer is destroyed. If s == 0 then it will _not_ be
	*   used, and instead the internal buffer is set to NULL_String_.
	*
	*  @param s Non-zero terminated input string
	*  @param len Length of input string 's'
	*  @param release Allocator responsible(true)/not reponsible(false) for
	*    freeing memory.
	*/
	void set (const char_type * s, size_type len, bool release);

	/**
	* Clear this string. Memory is _not_ freed if @a release is false.
	*
	* Warning: This method was incorrectly documented in the past, but
	* the current implementation has been changed to match the documented
	* behavior.
	*
	* Warning: clear(false) behaves like fast_clear() below.
	*
	* @param release Memory is freed if true, and not freed if false.
	*/
	void clear (bool release = false);

	/**
	* A more specialized version of clear(): "fast clear". fast_clear()
	* resets the string to 0 length. If the string owns the buffer
	* (@arg release_== true):
	*  - the string buffer is not freed
	*  - the first character of the buffer is set to 0.
	*
	* If @arg release_ is false (this object does not own the buffer):
	*  - the buffer pointer is reset to the NULL_String_ and does not
	*    maintain a pointer to the caller-supplied buffer on return
	*  - the maximum string length is reset to 0.
	*
	* Warning : Calling clear(false) or fast_clear() can have unintended
	*   side-effects if the string was constructed (or set()) with an
	*   external buffer. The string will be disassociated with the buffer
	*   and the next append() or +=() will cause a new buffer to be
	*   allocated internally.
	*/
	void fast_clear (void);

	/**
	* Return a substring given an offset and length.
	* If length == @c npos use rest of str.  Return empty substring if
	* offset or offset/length are invalid.
	*
	* @param offset Index of first desired character of the substring.
	* @param length How many characters to return starting at the offset.
	* @return The string containing the desired substring
	*/
	String_Base_lo substring (size_type offset,size_type length = npos) const;

	/**
	*  Same as <substring>.
	*
	* @param offset Index of first desired character of the substring.
	* @param length How many characters to return starting at the offset.
	* @return The string containing the desired substring
	*/
	String_Base_lo substr (size_type offset,size_type length = npos) const;

	/**
	*  Concat operator (copies memory).
	*
	*  @param s Input String_Base_lo string to concatenate to this string.
	*  @return The combined string (input append to the end of the old). New
	*    string is zero terminated.
	*/
	String_Base_lo  &operator += (const self_type &s);

	/**
	*  Concat operator (copies memory).
	*
	*  @param s Input C string to concatenate to this string.
	*  @return The combined string (input append to the end of the old). New
	*    string is zero terminated.
	*/
	String_Base_lo & operator += (const char_type* s);

	/**
	*  Concat operator (copies memory).
	*
	*  @param c Input char_type to concatenate to this string.
	*  @return The combined string (input append to the end of the old). New
	*    string is zero terminated.
	*/
	String_Base_lo & operator += (const char_type c);

	/**
	*  Append function (copies memory).
	*
	*  @param s Input char_type array to concatenate to this string.
	*  @param slen The length of the array.
	*  @return The combined string (input append to the end of the old). New
	*    string is zero terminated.
	*/
	String_Base_lo & append (const char_type* s, size_type slen);
	String_Base_lo & append (const char_type* s, size_type slen , size_t index);
	/**
	*  Return the length of the string.
	*
	*  @return Length of stored string
	*/
	size_type length (void) const;

	/**
	*  Return the number of allocated CHARs in the string object.
	*  This may be greater than the current length of the string.
	*
	*  @return Maximum number of char_type units that can be stored, including
	*          any terminating nul that may be needed.
	*/
	size_t capacity (void) const;

	/**
	* Return @c true if the length of the string is zero, else @c false.
	*/
	bool is_empty (void) const;

	/**
	* Return @c true if the length of the string is zero, else @c
	* false.  We recommend using @c is_empty() instead since it's more
	* consistent with the ACE container naming conventions.
	*/
	bool empty (void) const;

	/**
	* Get a copy of the underlying representation.
	*
	* This method allocates memory for a copy of the string and returns
	* a pointer to the new area. The caller is responsible for freeing
	* the memory when finished; use delete []
	*
	* @return Pointer reference to the string data. Returned string is
	*    zero terminated.
	*/
	char_type *rep (void) const;

	/**
	* Get at the underlying representation directly!
	* _Don't_ even think about casting the result to (char *) and modifying it,
	* if it has length 0!
	*
	* @return Pointer reference to the stored string data. No guarantee is
	*    that the string is zero terminated.
	*
	*/
	const char_type *fast_rep (void) const;

	/**
	*  Same as STL String's <c_str> and <fast_rep>.
	*/
	const char_type *c_str (void) const;

	/**
	*  Comparison operator that will match substrings.  Returns the
	*  slot of the first location that matches, else @c npos.
	*
	*  @param s Input String_Base_lo string
	*  @return Integer index value of the first location of string @a s or
	*          @c npos (not found).
	*/
	size_type strstr (const self_type &s) const;

	//! Makes the string lower case.
	void make_lower();


	//! Makes the string upper case.
	void make_upper();

	//! Replaces all characters of a special type with another one
	/** \param toReplace Character to replace.
	\param replaceWith Character replacing the old one. */
	void replace(char_type toReplace, char_type replaceWith);
	void replace(const char_type *sRelplace , size_type len , const char_type* s , size_type count);
	void replace(size_type beginpos , size_type len , const char_type* s , size_type count); 
	/**
	*  Find <str> starting at pos.  Returns the slot of the first
	*  location that matches (will be >= pos), else @c npos.
	*
	*  @param str Input String_Base_lo string to search for in stored string.
	*  @param pos Starting index position to start searching for string @a str.
	*  @return Index value of the first location of string @a str else
	*          @c npos.
	*/
	size_type find (const self_type &str, size_type pos = 0) const;

	/**
	*  Find @a s starting at pos.  Returns the slot of the first
	*  location that matches (will be >= pos), else @c npos.
	*
	*  @param s non-zero input string to search for in stored string.
	*  @param len s length, can is -1
	*  @param pos Starting index position to start searching for string @a str.
	*  @return Index value of the first location of string @a str else
	*          @c npos.
	*/
	size_type find (const char_type *s , size_type len , size_type pos = 0) const;

	/**
	*  Find @a c starting at pos.  Returns the slot of the first
	*  location that matches (will be >= pos), else @c npos.
	*
	*  @param c Input character to search for in stored string.
	*  @param pos Starting index position to start searching for string @a str.
	*  @return Index value of the first location of string @a str else
	*          @c npos.
	*/
	size_type find (char_type c, size_type pos = 0) const;

	/**
	*  Find @a c starting at pos (counting from the end).  Returns the
	*  slot of the first location that matches, else @c npos.
	*
	*  @param c Input character to search for in stored string.
	*  @param pos Starting index position to start searching for string @a str.
	*  @return Index value of the first location of string @a str else
	*          @c npos.
	*/
	size_type rfind (char_type c, size_type pos = npos) const;

	//! Erases a character from the string.
	/** May be slow, because all elements
	following after the erased element have to be copied.
	\param index: Index of element to be erased. */
	void erase(size_type index , size_type len = 1);

	//! Trims the string.
	/** Removes the specified characters (by default, Latin-1 whitespace)
	from the begining and the end of the string. */
    self_type& trim(const self_type& whitespace);
	/**
	*  Equality comparison operator (must match entire string).
	*
	* @param s Input String_Base_lo string to compare against stored string.
	* @return @c true if equal, @c false otherwise.
	*/
	bool operator == (const self_type &s) const;

	/**
	*  Equality comparison operator (must match entire string).
	*
	* @param s Null terminated string to compare against stored string.
	* @return @c true if equal, @c false otherwise.
	*/
	bool operator == (const char_type *s) const;

	/**
	*  Less than comparison operator.
	*
	*  @param s Input String_Base_lo string to compare against stored string.
	*  @return @c true if less than, @c false otherwise.
	*/
	bool operator < (const self_type &s) const;

	/**
	*  Greater than comparison operator.
	*
	*  @param s Input String_Base_lo string to compare against stored string.
	*  @return @c true if greater than, @c false otherwise.
	*/
	bool operator > (const self_type &s) const;

	/**
	*  Inequality comparison operator.
	*
	*  @param s String to compare against stored string.
	*  @return @c true if not equal, @c false otherwise.
	*/
	bool operator != (const self_type &s) const;

	/**
	*  Inequality comparison operator.
	*
	*  @param s Null terminated string to compare against stored string.
	*  @return @c true if not equal, @c false otherwise.
	*/
	bool operator != (const char_type *s) const;

	/**
	*  Performs a strncmp comparison.
	*
	*  @param s Input String_Base_lo string to compare against stored string.
	*  @return Integer value of result (less than 0, 0, greater than 0)
	*    depending on how input string @a s is to the stored string.
	*/
	int compare (const self_type &s) const;

	/**
	*  Dump the state of an object.
	*/
	void dump (void) const;

	/**
	* This method is designed for high-performance. Please use with
	* care ;-)
	*
	* Warning : This method was documented incorrectly in the past.
	* The original intention was to change the length of the string to
	* len, and to fill the whole thing with c CHARs.
	* However, what was actually done was to set the length of the
	* string to zero, and fill the buffer with c's. The buffer was
	* also not null-terminated unless c happened to be zero.
	* Rather than fix the method to work as documented, the code is
	* left as is, but the second parameter should probably not be used.
	*
	* fast_resize just adjusts the buffer if needed and sets the length,
	* it doesn't fill the buffer, so is much faster.
	* 
	* @param len The number of CHARs to reserve
	* @param c The char_type to use when filling the string.
	*/
	void resize (size_type len, char_type c = 0);
	void fast_resize (size_t len);

	/// Swap the contents of this @c String_Base_lo with @a str.
	/**
	* @note This is non-throwing operation.
	*/
	void swap (self_type & str);

protected:
	/**
	*  Pointer to a memory allocator.
	*/
	CloAllocator *allocator_;

	/**
	*  Length of the String_Base_lo data (not counting the trailing '\0').
	*/
	size_type len_;

	/**
	*  Length of the String_Base_lo data buffer.  Keeping track of the
	*  length allows to avoid unnecessary dynamic allocations.
	*/
	size_type buf_len_;

	/**
	*  Pointer to data.
	*/
	char_type *rep_;

	/**
	*  Flag that indicates if we own the memory
	*/
	bool release_;

	/**
	*  Represents the "NULL" string to simplify the internal logic.
	*/
	static char_type NULL_String_;
};

loCOMMON_API 
String_Base_lo operator+ (const String_Base_lo &s, const String_Base_lo &t);

loCOMMON_API 
String_Base_lo operator+ (const String_Base_lo::char_type *s, const String_Base_lo &t);

loCOMMON_API 
String_Base_lo operator+ (const String_Base_lo &s, const String_Base_lo::char_type *t);

loCOMMON_API 
String_Base_lo operator + (const String_Base_lo &t,const String_Base_lo::char_type c);

loCOMMON_API 
String_Base_lo operator + (const String_Base_lo::char_type c,const String_Base_lo &t);

END_NAMESPACE(locom)