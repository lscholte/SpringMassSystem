#include "BlankScene.hpp"
#include "Shader.hpp"
#include "Spring.hpp"
#include <atlas/core/GLFW.hpp>

BlankScene::BlankScene()
{	
	mGeometries.push_back(std::make_unique<Spring>());	
}

BlankScene::~BlankScene()
{
}

void BlankScene::renderScene()
{
	float theta = 45.0f;
	float row = 3.0f;
	float aspectRatio = 1.0f;
	
	glm::vec3 eye(50.0*cos(glm::radians(theta)), 50.0*sin(glm::radians(theta)), row);
	glm::vec3 look(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);
	
	glm::mat4 view = glm::lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 100.0f);	
	
	//Render black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	for(auto &geometry : mGeometries)
	{
		geometry->renderGeometry(projection, view);
	}
}

void BlankScene::updateScene(double time)
{
	mTime.deltaTime = time - mTime.currentTime;
	mTime.totalTime += mTime.deltaTime;
	mTime.currentTime = time;

	for(auto &geometry : mGeometries)
	{
		geometry->updateGeometry(mTime);
	}
}