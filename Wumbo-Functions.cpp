#include "Wumbo-Functions.h"
#include "Wumbo-Types.h"
#include <cmath>
#define M_PI 3.14159265359f
namespace Wumbo
{
	FILE *fopen_safe4ios(const char *path, const char *opts)
	{
#if PLATFORM_IOS
		FILE *fp = NULL;
		if (strncmp(opts,"wb",2) == 0)
		{
			//printf("fopen_safe4ios() open file \"wb\" at location %s\n",path);
			fp = fopen(path,"wb");
			if (fp == NULL)
			{
				//printf("fopen_safe4ios() fopen failed to open file for writing at location %s\n",path);
				NSFileHandle *file = [NSFileHandle fileHandleForWritingAtPath:[NSString stringWithCString:path encoding:NSASCIIStringEncoding]];
				if (file == nil)
				{
					//printf("fopen_safe4ios() NSFileHandle failed fileHandleForWritingAtPath location %s\n",path);
					if ([[NSFileManager defaultManager] createFileAtPath:[NSString stringWithCString:path encoding:NSASCIIStringEncoding] contents:nil attributes:nil])
					{
						//printf("fopen_safe4ios() NSFileHandle successfully createFileAtPath at location %s\n",path);
						fp = fopen(path,"wb");
						//if (fp != NULL)
							//printf("fopen_safe4ios() successfully? created new file for writing at location %s\n",path);
					}
					else
					{
						//printf("fopen_safe4ios() NSFileHandle failed createFileAtPath at location %s\n",path);
					}
				}
				else
				{
					//printf("fopen_safe4ios() NSFileHandle successfully fileHandleForWritingAtPath location %s\n",path);
					fp = fopen(path,"wb");
					//if (fp != NULL)
					//	printf("fopen_safe4ios() successfully? opened file for writing at location %s\n",path);
				}
			}
			//else
				//printf("fopen_safe4ios() fopen successfully opened file for writing at location %s\n",path);
		}
		if (strncmp(opts, "rb", 2) == 0)
		{
			//printf("fopen_safe4ios() open file \"rb\" at location %s\n",path);
			fp = fopen(path,"rb");
			//if (fp != NULL)
				//printf("fopen_safe4ios() successfully? opened file for reading at location %s\n",path);
		}
		return fp;
#else
		return fopen(path,opts);
#endif
	}
	
	void MsgBox(std::string msg, std::string title)
	{
#if PLATFORM_WINDOWS
		MessageBox(0,msg.c_str(),title.c_str(),MB_OK | MB_ICONEXCLAMATION);
#endif
	}

	vec2df rotatePointDEG(float pointX, float pointY, float originX, float originY, float angle)
	{
		angle *= M_PI/180.f;
		vec2df point;
		point.x = cos(angle) * (pointX-originX) - sin(angle) * (pointY-originY) + originX;
		point.y = sin(angle) * (pointX-originX) + cos(angle) * (pointY-originY) + originY;
		return point;
	}
	
	const char *nsstring2cstr(NSString *str)
	{
#if PLATFORM_APPLE
		return [str UTF8String];
#else
		return str;
#endif
	}
	NSString *cstr2nsstring(const char *cstr)
	{
#if PLATFORM_APPLE
		return [NSString stringWithFormat:@"%s",cstr];
#else
		return (char*) cstr;
#endif	
	}
#if PLATFORM_APPLE
	NSString* concat(NSString* one, NSString* two)
	{
		return [NSString stringWithFormat:@"%@%@",one,two];
	}
	
	NSString* NSStd(std::string a)
	{
		NSString* cereal = [NSString stringWithUTF8String: a.c_str()];
		return cereal;
	}
#endif
	
#if PLATFORM_APPLE
	NSString* BundleResource(NSString *name)
	{
		//NSString* fname = concat(@"/",name);
		NSString* nspath = [NSString stringWithFormat:@"%@%s%@",[[NSBundle mainBundle] resourcePath],"/",name];//[[NSBundle mainBundle] pathForResource: fname_prefix ofType: fname_ext];
		return nspath;
	}
#endif
	
	const char *BundleResource(const char *name)
	{
#if PLATFORM_APPLE
		//NSString* fname = concat(@"/",name);
		NSString* nspath = [NSString stringWithFormat:@"%@%s%@",[[NSBundle mainBundle] resourcePath],"/",cstr2nsstring(name)];//[[NSBundle mainBundle] pathForResource: fname_prefix ofType: fname_ext];
		return [nspath UTF8String];
#else
		return name;
#endif
	}
	
	std::string BundleResource(std::string name)
	{
#if PLATFORM_APPLE
		//NSString* fname = concat(@"/",name);
		NSString* nspath = [NSString stringWithFormat:@"%@%s%@",[[NSBundle mainBundle] resourcePath],"/",cstr2nsstring(name.c_str())];//[[NSBundle mainBundle] pathForResource: fname_prefix ofType: fname_ext];
		return std::string([nspath UTF8String]);
#else
		return name;
#endif
	}
	
	
	
#if PLATFORM_APPLE	
	NSString* DocumentResource(NSString *name)
	{

		NSArray *search = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
		NSMutableString *nspath = [NSMutableString stringWithFormat:@"%@%s%@", [search objectAtIndex:0], "/", name];
		return nspath;
	}
#endif

	const char* DocumentResource(const char *name)
	{
#if PLATFORM_APPLE
		NSArray *search = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
		NSMutableString *nspath = [NSMutableString stringWithFormat:@"%@%s%s", [search objectAtIndex:0], "/", name];
		return [nspath UTF8String];
#else
		return name;
#endif
	}
	
	NSString* DocumentResource(std::string name)
	{
#if PLATFORM_APPLE
		NSArray *search = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
		NSMutableString *nspath = [NSMutableString stringWithFormat:@"%@%s%s", [search objectAtIndex:0], "/", name.c_str()];
		return nspath;
#else
		return (char*) name.c_str();
#endif
	}
	
	
	
	
	
	std::string GoUpFoldersResource(std::string name, unsigned int count)
	{
		count += 1;
#if PLATFORM_APPLE
		count += 3;
		std::string location = std::string([[[NSBundle mainBundle] executablePath] UTF8String]);
#elif PLATFORM_WINDOWS
		char p[256];
		GetCurrentDirectoryA(256,p);
		std::string location = std::string(p)+"/";
		for(int i=0;i<location.size();i++)
			if (location[i] == '\\')
				location[i] = '/';
		//printf("LOCATION = %s\n",location.c_str());
#endif
		std::string retval = name;
		int c = 0;
		for(int i=location.length()-1;i>=0;i--)
		{
			if (location.at(i) == '/')
			{
				c++;
				//printf("DETECTED '/' | c = %i | i = %i | %s\n",c,i,(location.substr(0,i+1)+name).c_str());
			}
			if (c == count)
			{
				retval = location.substr(0,i+1)+name;
				i = -1;
				continue;
			}
		}
		//printf("RETVAL = %s\n",retval.c_str());
		return retval;
	}

};