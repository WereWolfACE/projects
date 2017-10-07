#ifndef _INTRUSIVE_WEAK_PTR_HPP_INCLUDED
#define _INTRUSIVE_WEAK_PTR_HPP_INCLUDED

//
// Based on:
//
//  intrusive_ptr.hpp
//
//  Copyright (c) 2001, 2002 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/intrusive_ptr.html for documentation.
//

#include <boost/config.hpp>

#include <boost/assert.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/smart_ptr/detail/sp_convertible.hpp>
#include <boost/smart_ptr/detail/sp_nullptr_t.hpp>

#include <boost/config/no_tr1/functional.hpp>           // for std::less

#include <boost/intrusive_ptr.hpp>

#if !defined(BOOST_NO_IOSTREAM)
#if !defined(BOOST_NO_IOSFWD)
#include <iosfwd>               // for std::basic_ostream
#else
#include <ostream>
#endif
#endif


//
//  intrusive_weak_ptr
//
//  A smart pointer that uses intrusive reference counting.
//
//  Relies on unqualified calls to
//
//      void intrusive_weak_ptr_add_ref(T * p);
//      void intrusive_weak_ptr_release(T * p);
//      bool intrusive_weak_ptr_alive(T * p);
//
//          (p != 0)
//
//  The object is responsible for destroying itself.
//

template<class T> class intrusive_weak_ptr
{
private:

	typedef intrusive_weak_ptr this_type;

public:

	typedef T element_type;

	intrusive_weak_ptr() BOOST_NOEXCEPT : px( 0 )
	{
	}

	intrusive_weak_ptr( T * p, bool add_ref = true ): px( p )
	{
		if( px != 0 && add_ref ) intrusive_weak_ptr_add_ref( px );
	}

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)

	template<class U>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )

	intrusive_weak_ptr( intrusive_weak_ptr<U> const & rhs, typename boost::detail::sp_enable_if_convertible<U,T>::type = boost::detail::sp_empty() )

#else

	intrusive_weak_ptr( intrusive_weak_ptr<U> const & rhs )

#endif
	: px( rhs.get() )
	{
		if( px != 0 ) intrusive_weak_ptr_add_ref( px );
	}

#endif

	intrusive_weak_ptr(intrusive_weak_ptr const & rhs): px( rhs.px )
	{
		if( px != 0 ) intrusive_weak_ptr_add_ref( px );
	}

	template<class U>
	intrusive_weak_ptr( boost::intrusive_ptr<U> const & rhs ) // non-explicit
	: px( rhs.get() )
	{
		if( px != 0 ) intrusive_weak_ptr_add_ref( px );
	}

	~intrusive_weak_ptr()
	{
		if( px != 0 ) intrusive_weak_ptr_release( px );
	}

#if !defined(BOOST_NO_MEMBER_TEMPLATES) || defined(BOOST_MSVC6_MEMBER_TEMPLATES)

	template<class U> intrusive_weak_ptr & operator=(intrusive_weak_ptr<U> const & rhs)
	{
		this_type(rhs).swap(*this);
		return *this;
	}

#endif

// Move support

#if !defined( BOOST_NO_CXX11_RVALUE_REFERENCES )

	intrusive_weak_ptr(intrusive_weak_ptr && rhs) BOOST_NOEXCEPT : px( rhs.px )
	{
		rhs.px = 0;
	}

	intrusive_weak_ptr & operator=(intrusive_weak_ptr && rhs) BOOST_NOEXCEPT
	{
		this_type( static_cast< intrusive_weak_ptr && >( rhs ) ).swap(*this);
		return *this;
	}

#endif

	intrusive_weak_ptr & operator=(intrusive_weak_ptr const & rhs)
	{
		this_type(rhs).swap(*this);
		return *this;
	}

	intrusive_weak_ptr & operator=(T * rhs)
	{
		this_type(rhs).swap(*this);
		return *this;
	}

	void reset() BOOST_NOEXCEPT
	{
		this_type().swap( *this );
	}

	void reset( T * rhs )
	{
		this_type( rhs ).swap( *this );
	}

	T * get() const BOOST_NOEXCEPT
	{
		if (px && !intrusive_weak_ptr_alive(px)) {
			intrusive_weak_ptr_release( px );
			px = 0;
		}
		return px;
	}

	/*~ Forbidden! Use get() and check for NULL.
	T & operator*() const
	{
		T * p = get();
		BOOST_ASSERT( p != 0 );
		return *p;
	}

	T * operator->() const
	{
		T * p = get();
		BOOST_ASSERT( p != 0 );
		return p;
	}
	*/

#if !defined( BOOST_NO_CXX11_EXPLICIT_CONVERSION_OPERATORS ) && !defined( BOOST_NO_CXX11_NULLPTR )

	explicit operator bool () const BOOST_NOEXCEPT
	{
		return get();
	}

#elif defined( _MANAGED )

	static void unspecified_bool( this_type*** )
	{
	}

	typedef void (*unspecified_bool_type)( this_type*** );

	operator unspecified_bool_type() const BOOST_NOEXCEPT
	{
		return get() ? unspecified_bool : 0;
	}

#else

	typedef element_type * this_type::*unspecified_bool_type;

	operator unspecified_bool_type() const BOOST_NOEXCEPT
	{
		return get() ? &this_type::px : 0;
	}

#endif

	// operator! is redundant, but some compilers need it
	bool operator! () const BOOST_NOEXCEPT
	{
		return !get();
	}


	void swap(intrusive_weak_ptr & rhs) BOOST_NOEXCEPT
	{
		T * tmp = px;
		px = rhs.px;
		rhs.px = tmp;
	}

private:

	mutable T * px;
};

template<class T, class U> inline bool operator==(intrusive_weak_ptr<T> const & a, intrusive_weak_ptr<U> const & b)
{
	return a.get() == b.get();
}

template<class T, class U> inline bool operator!=(intrusive_weak_ptr<T> const & a, intrusive_weak_ptr<U> const & b)
{
	return a.get() != b.get();
}

template<class T, class U> inline bool operator==(intrusive_weak_ptr<T> const & a, U * b)
{
	return a.get() == b;
}

template<class T, class U> inline bool operator!=(intrusive_weak_ptr<T> const & a, U * b)
{
	return a.get() != b;
}

template<class T, class U> inline bool operator==(T * a, intrusive_weak_ptr<U> const & b)
{
	return a == b.get();
}

template<class T, class U> inline bool operator!=(T * a, intrusive_weak_ptr<U> const & b)
{
	return a != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<class T> inline bool operator!=(intrusive_weak_ptr<T> const & a, intrusive_weak_ptr<T> const & b)
{
	return a.get() != b.get();
}

#endif

#if !defined( BOOST_NO_CXX11_NULLPTR )

template<class T> inline bool operator==( intrusive_weak_ptr<T> const & p, boost::detail::sp_nullptr_t ) BOOST_NOEXCEPT
{
	return p.get() == 0;
}

template<class T> inline bool operator==( boost::detail::sp_nullptr_t, intrusive_weak_ptr<T> const & p ) BOOST_NOEXCEPT
{
	return p.get() == 0;
}

template<class T> inline bool operator!=( intrusive_weak_ptr<T> const & p, boost::detail::sp_nullptr_t ) BOOST_NOEXCEPT
{
	return p.get() != 0;
}

template<class T> inline bool operator!=( boost::detail::sp_nullptr_t, intrusive_weak_ptr<T> const & p ) BOOST_NOEXCEPT
{
	return p.get() != 0;
}

#endif

template<class T> inline bool operator<(intrusive_weak_ptr<T> const & a, intrusive_weak_ptr<T> const & b)
{
	return std::less<T *>()(a.get(), b.get());
}

template<class T> void swap(intrusive_weak_ptr<T> & lhs, intrusive_weak_ptr<T> & rhs)
{
	lhs.swap(rhs);
}

// mem_fn support

template<class T> T * get_pointer(intrusive_weak_ptr<T> const & p)
{
	return p.get();
}

template<class T, class U> intrusive_weak_ptr<T> static_pointer_cast(intrusive_weak_ptr<U> const & p)
{
	return static_cast<T *>(p.get());
}

template<class T, class U> intrusive_weak_ptr<T> const_pointer_cast(intrusive_weak_ptr<U> const & p)
{
	return const_cast<T *>(p.get());
}

template<class T, class U> intrusive_weak_ptr<T> dynamic_pointer_cast(intrusive_weak_ptr<U> const & p)
{
	return dynamic_cast<T *>(p.get());
}

// operator<<

#if !defined(BOOST_NO_IOSTREAM)

#if defined(BOOST_NO_TEMPLATED_IOSTREAMS) || ( defined(__GNUC__) &&  (__GNUC__ < 3) )

template<class Y> std::ostream & operator<< (std::ostream & os, intrusive_weak_ptr<Y> const & p)
{
	os << p.get();
	return os;
}

#else

// in STLport's no-iostreams mode no iostream symbols can be used
#ifndef _STLP_NO_IOSTREAMS

# if defined(BOOST_MSVC) && BOOST_WORKAROUND(BOOST_MSVC, < 1300 && __SGI_STL_PORT)
// MSVC6 has problems finding std::basic_ostream through the using declaration in namespace _STL
using std::basic_ostream;
template<class E, class T, class Y> basic_ostream<E, T> & operator<< (basic_ostream<E, T> & os, intrusive_weak_ptr<Y> const & p)
# else
template<class E, class T, class Y> std::basic_ostream<E, T> & operator<< (std::basic_ostream<E, T> & os, intrusive_weak_ptr<Y> const & p)
# endif
{
	os << p.get();
	return os;
}

#endif // _STLP_NO_IOSTREAMS

#endif // __GNUC__ < 3

#endif // !defined(BOOST_NO_IOSTREAM)

// hash_value

template< class T > struct hash;

template< class T > std::size_t hash_value( intrusive_weak_ptr<T> const & p )
{
	return boost::hash< T* >()( p.get() );
}

#endif  // #ifndef _INTRUSIVE_WEAK_PTR_HPP_INCLUDED