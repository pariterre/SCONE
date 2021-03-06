/*
** memory_tools.h
**
** Copyright (C) 2013-2019 Thomas Geijtenbeek and contributors. All rights reserved.
**
** This file is part of SCONE. For more information, see http://scone.software.
*/

#pragma once

#include <memory>

// class and unique pointer type forward declaration
#define SCONE_DECLARE_CLASS_AND_PTR( _class_ ) \
	class _class_; \
	typedef std::unique_ptr< _class_ > _class_##UP;

// struct and unique pointer type forward declaration
#define SCONE_DECLARE_STRUCT_AND_PTR( _class_ ) \
struct _class_; \
	typedef std::unique_ptr< _class_ > _class_##UP;

namespace scone
{
	// dynamic cast a unique_ptr, throws on failure
	template <typename To, typename From> 
    std::unique_ptr< To > dynamic_unique_cast( std::unique_ptr< From >&& p )
	{
		To& cast = dynamic_cast< To& >( *p ); // throws on failure, freeing p
		p.release(); // release ownership from p
		return std::unique_ptr< To >( &cast ); // return pointer with ownership
	}
}
