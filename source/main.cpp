#include <atlas/utils/Application.hpp>

int main()
{
	auto &application = atlas::utils::Application::getInstance();
	
	application.createWindow(1024, 1024, "Spring Mass System");
	application.runApplication();
	
	return 0;
}
