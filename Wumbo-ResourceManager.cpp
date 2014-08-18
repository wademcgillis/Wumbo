#include "Wumbo-ResourceManager.h"

namespace Wumbo
{
	void ResourceManager::internalAddResource(Resource *resource, std::string name, std::string category)
	{
		std::map<std::string,Resource*> *mapToUse;
		mapToUse = getCategoryMapPointerWithName(category);
		if (mapToUse == NULL)
		{
			mapToUse = new std::map<std::string,Resource*>();
			categoryMap.insert(std::map<std::string,std::map<std::string,Resource*>*>::value_type(category,mapToUse));
			printf("ResourceManager::internalLoad() -- The category \"%s\" was created.\n",category.c_str());
		}
		mapToUse->insert(std::map<std::string,Resource*>::value_type(name,resource));
	}
	Resource* ResourceManager::getResourceWithName(std::string name, std::string category) // complete
	{
		std::map<std::string,Resource*> *map = getCategoryMapPointerWithName(category);
		if (map != NULL)
		{
			Resource *resource = getResourceInMapPointerWithName(map,name);
			if (resource != NULL)
				return resource;
			else
				printf("ResourceManager::getResourceWithName() -- No resource with the name \"%s\" exists in the category \"%s\".\n",name.c_str(),category.c_str());
		}
		else
			printf("ResourceManager::getResourceWithName() -- No category with the name \"%s\" exists.\n",category.c_str());
		return NULL;
	}
	Resource* ResourceManager::getResourceInMapPointerWithName(std::map<std::string,Resource*> *map,std::string name) // complete
	{
		std::map<std::string,Resource*>::iterator it;
		it = map->find(name);
		if (it == map->end())
			return NULL;
		return it->second;
	}
	std::map<std::string,Resource*> *ResourceManager::getCategoryMapPointerWithName(std::string category) // complete
	{
		std::map<std::string,std::map<std::string,Resource*>*>::iterator it;
		it = categoryMap.find(category);
		if (it == categoryMap.end())
			return NULL;
		return it->second;
	}
	bool ResourceManager::categoryExists(std::string category) // complete
	{
		return getCategoryMapPointerWithName(category) != NULL;
	}

	void ResourceManager::releaseResourceInCategoryWithName(std::string name, std::string category) // complete
	{
		std::map<std::string,Resource*> *map = getCategoryMapPointerWithName(category);
		if (map != NULL)
		{
			Resource *resource = getResourceInMapPointerWithName(map,name);
			if (resource != NULL)
			{
				delete resource;
				std::map<std::string,Resource*>::iterator it = map->find(name);
				if (it != map->end())
					map->erase(it);
				printf("ResourceManager::releaseResourceInCategoryWithName() -- The resource with the name \"%s\" in the category \"%s\" has been deleted.\n",name.c_str(),category.c_str());
			}
			else
				printf("ResourceManager::releaseResourceInCategoryWithName() -- No resource with the name \"%s\" exists in the category \"%s\".\n",name.c_str(),category.c_str());
		}
		else
			printf("ResourceManager::releaseResourceInCategoryWithName() -- No category with the name \"%s\" exists.\n",category.c_str());
	}
	void ResourceManager::releaseAllResourcesInCategoryWithName(std::string category) // complete
	{
		std::map<std::string,Resource*> *map = getCategoryMapPointerWithName(category);
		std::map<std::string,std::map<std::string,Resource*>*>::iterator categoryIterator;
		if (map != NULL)
		{
			printf("ResourceManager::releaseAllResourcesInCategoryWithName() -- Releasing all resources in category \"%s\".\n",category.c_str());
			for(std::map<std::string,Resource*>::iterator resourceMapIterator = map->begin(); resourceMapIterator != map->end(); ++resourceMapIterator)
			{
				delete resourceMapIterator->second;
				printf("ResourceManager::releaseAllResourcesInCategoryWithName() -- Resource \"%s\" has been released.\n",resourceMapIterator->first.c_str());	
			}
			printf("ResourceManager::releaseAllResourcesInCategoryWithName() -- All resources in category \"%s\" have been released.\n",category.c_str());
			delete map;
			categoryIterator = categoryMap.find(category);
			if (categoryIterator != categoryMap.end())
				categoryMap.erase(categoryIterator);
			printf("ResourceManager::releaseAllResourcesInCategoryWithName() -- Category \"%s\" has been deleted.\n",category.c_str());
		}
		else
			printf("ResourceManager::releaseAllResourcesInCategoryWithName()) -- No category with the name \"%s\" exists.\n",category.c_str());
	}
	void ResourceManager::releaseAllResources() // complete
	{
		printf("ResourceManager::releaseAllResources() -- Releasing all resources in all categories.\n");
		std::map<       std::string,           std::map<  std::string  ,  Resource*  >*       >::iterator iter;

		// iterator equals begin
		iter = categoryMap.begin();
		while(iter != categoryMap.end()) // while beginning != end
		{
			// delete
			releaseAllResourcesInCategoryWithName(iter->first);
			// iterator equals begin
			iter = categoryMap.begin();
		}
		printf("ResourceManager::releaseAllResources() -- Complete.\n");
	}
};