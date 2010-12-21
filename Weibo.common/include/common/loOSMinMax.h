#ifndef __lo_MINMAX_H__
#define __lo_MINMAX_H__

namespace loOS
{
	template <class T>
	inline const T &
		MIN (const T &t1, const T &t2)
	{
		return t2 > t1 ? t1 : t2;
	}

	template <class T>
	inline const T &
		MAX (const T &t1, const T &t2)
	{
		return t1 > t2 ? t1 : t2;
	}

	template <class T>
	inline const T &
		MIN (const T &t1, const T &t2, const T &t3)
	{
		return MIN ( MIN (t1, t2), t3);
	}

	template <class T>
	inline const T &
		MAX (const T &t1, const T &t2, const T &t3)
	{
		return MAX ( MAX (t1, t2), t3);
	}

	template <class T>
	inline const T &
		RANGE (const T &min, const T &max, const T &val)
	{
		return MIN (MAX (min, val), max);
	}
}// namespace loOS

#endif  /* __lo_MINMAX_H__ */
