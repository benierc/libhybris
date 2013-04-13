#include <ws.h>
#include <dlfcn.h>
#include <stdlib.h>

static void * (*_androidCreateDisplaySurface)();

static void *_libui = NULL;

static void _init_androidui()
{
       _libui = (void *) android_dlopen("/system/lib/libui.so", RTLD_LAZY);
}

#define UI_DLSYM(fptr, sym) do { if (_libui == NULL) { _init_androidui(); }; if (*(fptr) == NULL) { *(fptr) = (void *) android_dlsym(_libui, sym); } } while (0) 

static EGLNativeWindowType android_createDisplaySurface()
{
 	UI_DLSYM(&_androidCreateDisplaySurface, "android_createDisplaySurface");
	return (EGLNativeWindowType) (*_androidCreateDisplaySurface)();
}



static int nullws_IsValidDisplay(EGLNativeDisplayType display)
{
	return 1;
}

static EGLNativeWindowType nullws_CreateWindow(EGLNativeWindowType win, EGLNativeDisplayType display)
{
	if (win == 0)
	{
		return android_createDisplaySurface();
	}
	else
		return win;
}

static __eglMustCastToProperFunctionPointerType nullws_eglGetProcAddress(const char *procname) 
{
	return NULL;
}

static void nullws_passthroughImageKHR(EGLenum *target, EGLClientBuffer *buffer)
{
}

const char *nullws_eglQueryString(EGLDisplay dpy, EGLint name, const char *(*real_eglQueryString)(EGLDisplay dpy, EGLint name))
{
	return (*real_eglQueryString(dpy, name));
}


struct ws_module ws_module_info = {
	.IsValidDisplay = nullws_IsValidDisplay,
	.CreateWindow = nullws_CreateWindow,
	.eglGetProcAddress = nullws_eglGetProcAddress,
	.passthroughImageKHR = nullws_passthroughImageKHR,
	.eglQueryString = nullws_eglQueryString
};

