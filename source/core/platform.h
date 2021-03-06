#include "define.h"
//*******************************************************************************************
// NK PLATFORM MACROS
//*******************************************************************************************
// 		http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html
#ifdef EMSCRIPTEN
#  define TARGET_EMSCRIPTEN 1
#elif defined( __WIN32__ ) || defined( _WIN32 )
#  define TARGET_WIN32
#elif defined(__APPLE_CC__)
#  include <TargetConditionals.h>
#  if (TARGET_OS_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
#    define TARGET_IOS 1
#  else
#    define TARGET_OSX 1
#  endif
#elif defined (__ANDROID__)
#  define TARGET_ANDROID 1
#  define TARGET_OPENGLES 1
#elif defined(__ARMEL__)
#  define TARGET_LINUX 1
#  define TARGET_OPENGLES 1
#  define TARGET_LINUX_ARM 1
#else
#  define TARGET_LINUX 1
#endif

//**************
// IOS
//**************
#if TARGET_IOS

#ifndef OF_TARGET_IPHONE
#define OF_TARGET_IPHONE OF_TARGET_IOS
#endif

#define TARGET_LITTLE_ENDIAN		// arm cpu
#define NK_USE_GLES 1
#define NK_USE_GL3 0
#define NK_USE_WEBGL 0
#define NK_SUPPORTS_VAO 1

#pragma - FT ios FIX

#if defined(__ARM_NEON__)
#import <arm_neon.h>
#endif

#if NK_USE_GL3
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#else
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif
#endif

//**************
// OSX
//**************

#if TARGET_OSX

#ifndef __MACOSX_CORE__
#define __MACOSX_CORE__
#endif

#include <unistd.h>
#include <ApplicationServices/ApplicationServices.h>

#if defined(__LITTLE_ENDIAN__)
#define TARGET_LITTLE_ENDIAN		// intel cpu
#endif

#if TARGET_OS_MAC && !TARGET_OS_IPHONE
#ifndef __SSE3__
#warning "SSE3 instruction set not enabled. GLKit math routines will be slower."
#else
#include <immintrin.h>
#include <stdint.h>
#define USE_SSE3 1
#endif
#endif

#define NK_USE_GL3 0
#define NK_USE_WEBGL 0
#define NK_USE_GLES 0
#define NK_SUPPORTS_VAO 1

#import <OpenGL/OpenGL.h>

#if NK_USE_GL3
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>
#else
#ifndef glUniform1f
#define NK_USE_ARB_EXT 0
#endif
#import <OpenGL/gl.h>
#import <OpenGL/glext.h>
#endif

#endif

//**************
// ANDROID
//**************

#if TARGET_ANDROID

#define NK_USE_WEBGL 0
#define NK_SUPPORTS_VAO 1

#include <typeinfo>
#include <unistd.h>
#include <GLES/gl.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES/glext.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#define TARGET_LITTLE_ENDIAN
#endif

//**************
// LINUX
//**************

#if TARGET_LINUX

#define GL_GLEXT_PROTOTYPES
#include <unistd.h>

#ifdef TARGET_LINUX_ARM

#define NK_USE_GLES 1

#ifdef TARGET_RASPBERRY_PI
#include "bcm_host.h"
#define NK_USE_SDL 1
#define NK_USE_WEBGL 0
#define NK_SUPPORTS_VAO 0
#else
#define NK_SUPPORTS_VAO 1
#endif

#include "GLES/gl.h"
#include "GLES/glext.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

#define EGL_EGLEXT_PROTOTYPES
#include "EGL/egl.h"
#include "EGL/eglext.h"

#else // normal linux

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>

#endif

#endif

//**************
// EMSCRIPTEN
//**************

#if TARGET_EMSCRIPTEN

//#define GL_OES_vertex_array_object 1

//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

#define NK_USE_SDL 1

#define NK_SUPPORTS_VAO 0

#include <SDL/SDL.h>
#include "SDL/SDL_opengl.h"

#include <emscripten.h>
#warning !! COMPILING USING EMSCRIPTEN !!

#define NK_USE_WEBGL 1
#define NK_USE_GLES 1
#define NK_USE_GL3 0

#endif


//**************
// HACK
//**************

// for some reason, this isn't defined at compile time,
// so this hack let's us work
// for 99% of the linux folks that are on intel
// everyone one else will have RGB / BGR issues.
//#if defined(__LITTLE_ENDIAN__)
#define TARGET_LITTLE_ENDIAN		// intel cpu
//#endif

// some things for serial compilation:
#define B14400	14400
#define B28800	28800

//**************
// WINDOWS
//**************

// then the the platform specific includes:
#ifdef TARGET_WIN32
//this is for TryEnterCriticalSection
//http://www.zeroc.com/forums/help-center/351-ice-1-2-tryentercriticalsection-problem.html
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x500
#endif
#define WIN32_LEAN_AND_MEAN
#if (_MSC_VER)
#define NOMINMAX
//http://stackoverflow.com/questions/1904635/warning-c4003-and-errors-c2589-and-c2059-on-x-stdnumeric-limitsintmax
#endif
#include <windows.h>
#define GLEW_STATIC
#include "GL\glew.h"
#include "GL\wglew.h"
#include "glu.h"
#define __WINDOWS_DS__
#define __WINDOWS_MM__
#if (_MSC_VER)       // microsoft visual studio
#include <stdint.h>
#include <functional>
#pragma warning(disable : 4018)		// signed/unsigned mismatch (since vector.size() is a size_t)
#pragma warning(disable : 4068)		// unknown pragmas
#pragma warning(disable : 4101)		// unreferenced local variable
#pragma warning(disable : 4267)		// conversion from size_t to Size warning... possible loss of data
#pragma warning(disable : 4311)		// type cast pointer truncation (qt vp)
#pragma warning(disable : 4312)		// type cast conversion (in qt vp)
#pragma warning(disable : 4800)		// 'Boolean' : forcing value to bool 'true' or 'false'
// make microsoft visual studio complain less about double / float conversion and
// truncation
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
// warnings: http://msdn.microsoft.com/library/2c8f766e.aspx
#endif
#define TARGET_LITTLE_ENDIAN			// intel cpu
// some gl.h files, like dev-c++, are old - this is pretty universal
#ifndef GL_BGR_EXT
#define GL_BGR_EXT 0x80E0
#endif
#endif
