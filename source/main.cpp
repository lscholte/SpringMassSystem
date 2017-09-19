#include <atlas/utils/Application.hpp>
#include "BlankScene.hpp"

int main()
{
	auto &application = atlas::utils::Application::getInstance();
	
	application.createWindow(1024, 1024, "Spring Mass System");
	
	atlas::utils::ScenePointer scene = std::make_unique<BlankScene>();
	
	application.addScene(std::move(scene));
	application.runApplication();
	
	return 0;
}
