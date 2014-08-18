#ifndef __Wumbo_Functions_h__
#define __Wumbo_Functions_h__
#include "Wumbo-Header.h"
#include "Wumbo-Types.h"
#include <string>
#if PLATFORM_APPLE
#import <Foundation/Foundation.h>
#endif
namespace Wumbo
{
#if !PLATFORM_APPLE
	typedef char NSString;
#endif
	extern void MsgBox(std::string msg, std::string title);

	extern FILE *fopen_safe4ios(const char *path, const char *opts);
	
	extern vec2df rotatePointDEG(float pointX, float pointY, float originX, float originY, float angle);
	extern NSString* cstr2nsstring(const char *cstr);
	extern NSString* concat(NSString* one, NSString* two);
	extern NSString* NSStd(std::string a);
	extern const char* nsstring2cstr(NSString *str);
#if PLATFORM_APPLE
	extern NSString* BundleResource(NSString *name);
#endif
	extern const char *BundleResource(const char *name);
	extern std::string BundleResource(std::string name);
	
#if PLATFORM_APPLE
	extern NSString* DocumentResource(NSString *name);
#endif
	extern const char* DocumentResource(const char *name);
	extern NSString* DocumentResource(std::string name);
	
	extern std::string GoUpFoldersResource(std::string name, unsigned int count);
};
#endif