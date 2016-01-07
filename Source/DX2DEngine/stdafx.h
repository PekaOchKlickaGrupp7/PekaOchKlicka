#pragma once

#include "../targetver.h"
#include "error/error_manager.h"

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif


#include "math/matrix44.h"
#include "math/vector2.h"
#include "math/vector4.h"
#include "common_converters.h"

#include <d3d11_1.h>

#include <algorithm>
#include <array>
#include <exception>
#include <malloc.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// VS 2010's stdint.h conflicts with intsafe.h
#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#include <intsafe.h>
#pragma warning(pop)

#define SAFE_RELEASE(aPointer) if(aPointer){ aPointer->Release();}
#define SAFE_DELETE(aPointer) delete aPointer; aPointer = nullptr;
#define SAFE_ARRAYDELETE( x )  \
    if( NULL != x )        \
            {                      \
        delete [] x;       \
        x = NULL;          \
            }
