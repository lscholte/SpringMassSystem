#include <atlas/utils/Application.hpp>
#include <atlas/utils/WindowSettings.hpp>
#include <atlas/gl/ErrorCheck.hpp>
#include "SpringScene.hpp"

int main()
{

	atlas::gl::setGLErrorSeverity(ATLAS_GL_ERROR_SEVERITY_HIGH | ATLAS_GL_ERROR_SEVERITY_MEDIUM);
	
	atlas::utils::WindowSettings settings;
	settings.title = "Spring Mass System";
	settings.contextVersion = atlas::utils::ContextVersion(4, 1);
	settings.isForwardCompat = true;
	settings.windowSize = std::make_tuple(1024, 1024);
		
	auto &application = atlas::utils::Application::getInstance();
	
	application.createWindow(settings);
	
	atlas::utils::ScenePointer springScene = std::make_unique<SpringScene>();
	
	application.addScene(std::move(springScene));
	application.runApplication();
	
	return 0;
}
