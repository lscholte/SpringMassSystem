#include "BlankScene.hpp"
#include <atlas/core/GLFW.hpp>

BlankScene::BlankScene()
{
}

BlankScene::~BlankScene()
{
}

void BlankScene::renderScene()
{
	//Render red colour
	glClearColor(1.0f, 0.0f, 0.0f,0);
	glClear(GL_COLOR_BUFFER_BIT);
}
