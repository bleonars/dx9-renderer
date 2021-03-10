#pragma once

//#pragma comment( lib, "d3d9.lib" )
//#pragma comment( lib, "d3dx9.lib" )

//
// include defs
//

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define _HAS_EXCEPTIONS 0
#define _ITERATOR_DEBUG_LEVEL 0
#define _CRT_SECURE_INVALID_PARAMETER
#define _CRT_SECURE_NO_WARNINGS

#define FINI_WIDE_SUPPORT
#define INIP_USE_UNICODE

// no min(a,b) / max(a,b) macros
#define NOMINMAX

//
// macros, macro defs, etc
//

// try to force func to not be inlined by the compiler
#define NOINLINE __declspec( noinline )

//
// types
//

using ulong_t = unsigned long;

//
// windows / stl includes / etc
//

#include <Windows.h>
#include <cstdint>
#include <vector>
#include <memory>
#include <array>
#include <map>
#include <unordered_map>
#include <Shlobj.h>
#include <Shlobj_core.h>

// d3d 
#include <d3d9.h>
#include <d3dx9.h>

// dependencies
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H 
#include FT_BITMAP_H 

// misc
#include "math.h"
#include "utils.h"
#include "vector.h"
#include "renderer.h"

// d3d related
#include "d3d9_wrapper.h"