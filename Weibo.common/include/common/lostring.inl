
// -------------------------------------------------------

INLINE_lo
CloString::~CloString (void)
{
	if( rep_ )
	{
		this->allocator_->free(rep_);
		rep_ = 0;
		len_ = 0;
	}
}

INLINE_lo CloString
CloString::substr (size_type offset,
                     size_type length) const
{
  return this->substring (offset, length);
}

// Return the <slot'th> character in the string.

INLINE_lo char
CloString::operator[] (size_type slot) const
{
  return this->rep_[slot];
}

// Return the <slot'th> character in the string by reference.

INLINE_lo char &
CloString::operator[] (size_type slot)
{
  return this->rep_[slot];
}

// Get the underlying pointer (does not make a copy, so beware!).

INLINE_lo const char *
CloString::rep (void) const
{
  return this->rep_;
}

// Get the underlying pointer (does not make a copy, so beware!).

INLINE_lo const char *
CloString::fast_rep (void) const
{
  return this->rep_;
}

// Get the underlying pointer (does not make a copy, so beware!).

INLINE_lo const char *
CloString::c_str (void) const
{
  return this->rep_;
}

// Comparison operator.

INLINE_lo bool
CloString::operator== (const CloString &s) const
{
  return this->len_ == s.len_
    && loOS::strcmp (this->rep_, s.rep_) == 0;
}

// Less than comparison operator.

INLINE_lo bool
CloString::operator < (const CloString &s) const
{
  return (this->rep_ && s.rep_)
    ? loOS::strcmp (this->rep_, s.rep_) < 0
    : ((s.rep_) ? true : false);
}

// Greater than comparison operator.

INLINE_lo bool
CloString::operator > (const CloString &s) const
{
  return (this->rep_ && s.rep_)
    ? loOS::strcmp (this->rep_, s.rep_) > 0
    : ((this->rep_) ? true : false );
}

// Comparison operator.

INLINE_lo bool
CloString::operator!= (const CloString &s) const
{
  return !(*this == s);
}

INLINE_lo int
CloString::compare (const CloString &s) const
{
  return loOS::strcmp (this->rep_, s.rep_);
}

INLINE_lo CloString::size_type
CloString::find (const char *s, size_type pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = loOS::strstr (substr, s);
  if (pointer == 0)
    return CloString::npos;
  else
    return pointer - this->rep_;
}

INLINE_lo CloString::size_type
CloString::find (char c, size_type pos) const
{
  char *substr = this->rep_ + pos;
  char *pointer = loOS::strchr (substr, c);
  if (pointer == 0)
    return CloString::npos;
  else
    return pointer - this->rep_;
}

INLINE_lo CloString::size_type
CloString::strstr (const CloString &s) const
{
  return this->find (s.rep_);
}

INLINE_lo CloString::size_type
CloString::find (const CloString &str, size_type pos) const
{
  return this->find (str.rep_, pos);
}

INLINE_lo CloString::size_type
CloString::rfind (char c, size_type pos) const
{
  if (pos == CloString::npos)
    pos = this->len_;

  // Do not change to prefix operator!  Proper operation of this loop
  // depends on postfix decrement behavior.
  for (size_type i = pos; i-- != 0; )
    if (this->rep_[i] == c)
      return i;

  return CloString::npos;
}

INLINE_lo CloString::size_type
CloString::length (void) const
{
  return this->len_;
}