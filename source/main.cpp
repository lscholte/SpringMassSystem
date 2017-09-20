#include <atlas/utils/Application.hpp>
#include <atlas/utils/GUI.hpp>
#include "BlankScene.hpp"

int main()
{
	auto &application = atlas::utils::Application::getInstance();
	
	application.createWindow(1024, 1024, "Spring Mass System");
	
	atlas::utils::ScenePointer scene = std::make_unique<BlankScene>();

	glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	
	application.addScene(std::move(scene));
	application.runApplication();
	
	return 0;
}
