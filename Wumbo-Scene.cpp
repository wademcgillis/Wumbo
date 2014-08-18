#include "Wumbo-Scene.h"

namespace Wumbo
{
	Scene::Scene()
	{
		program = NULL;
	}
	Scene::Scene(Program *prog)
	{
		program = prog;
	}
	Program *Scene::getProgram()
	{
		return program;
	}
	void Scene::begin()
	{
		printf("Scene::start() -- This method should be overridden by a child class.\n");
	}
	void Scene::update()
	{
		//printf("Scene::update() -- This method should be overridden by a child class.\n");
	}
	void Scene::render()
	{
		//printf("Scene::render() -- This method should be overridden by a child class.\n");
	}
	void Scene::end()
	{
		printf("Scene::end() -- This method should be overridden by a child class.\n");
	}
	void Scene::onClose()
	{
#if PLATFORM_IOS
		printf("Scene::onClose() -- This method is available on PC platforms only.\n");
#else
		printf("Scene::onClose() -- This method should be overridden by a child class.\n");
#endif
	}
};