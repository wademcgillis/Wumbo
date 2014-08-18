#ifndef __Wumbo_ResourceManager_h__
#define __Wumbo_ResourceManager_h__
#include "Wumbo-Resource.h"
#include "Wumbo-Texture.h"
#include <map>

namespace Wumbo
{
	class ResourceManager
	{
	private:
		std::map<std::string,std::map<std::string,Resource*>*> categoryMap;
		Resource* getResourceInMapPointerWithName(std::map<std::string,Resource*> *map, std::string name); // complete
		std::map<std::string,Resource*>* getCategoryMapPointerWithName(std::string category); // complete
		bool categoryExists(std::string category); // complete
	protected:
		void internalAddResource(Resource *resource, std::string name, std::string category = "DEFAULT CATEGORY");
	public:
		Resource* getResourceWithName(std::string name, std::string category = "DEFAULT CATEGORY"); // complete
		void releaseResourceInCategoryWithName(std::string name, std::string category = "DEFAULT CATEGORY"); // complete
		void releaseAllResourcesInCategoryWithName(std::string category = "DEFAULT CATEGORY");
		void releaseAllResources();

	};

	class TextureManager : public ResourceManager
	{
	public:
		void addResource(Texture *resource, std::string name, std::string category = "DEFAULT CATEGORY")
		{
			internalAddResource(resource,name,category);
		}
#if PLATFORM_APPLE
		Texture *loadAndAddResource(NSString *fname, std::string name, std::string category = "DEFAULT CATEGORY")
#else
		Texture *loadAndAddResource(std::string fname, std::string name, std::string category = "DEFAULT CATEGORY")
#endif
		{
			Texture *texture = new Texture();
			bool success = texture->load(fname);
			if (success)
			{
				internalAddResource(texture, name, category);
				return texture;
			}
			else
			{
				delete texture;
				return NULL;
			}
		}
	};
};
#endif