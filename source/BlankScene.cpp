#include "BlankScene.hpp"
#include "Shader.hpp"
#include "Spring.hpp"
#include <atlas/core/GLFW.hpp>

BlankScene::BlankScene() :
	mGravitationalConstant(9.81)
{	
	std::unique_ptr<Spring> spring = std::make_unique<Spring>();
	spring->getCoil().setSpringConstant(3.0f);
	spring->getCoil().setDampeningConstant(2.0f);
	spring->getCoil().setRestLength(3.0f);
	spring->getMass().setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	spring->getMass().setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));	
	spring->getMass().setMass(1.0f);
	mGeometries.push_back(std::move(spring));	
}

BlankScene::~BlankScene()
{
}

void BlankScene::renderScene()
{

	float aspectRatio = 1.0f;
	
	glm::vec3 eye(10.0*cos(glm::radians(mTheta)), mRow, 10.0*sin(glm::radians(mTheta)));
	glm::vec3 look(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);
	
	glm::mat4 view = glm::lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 100.0f);	
	
	//Render black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
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

void BlankScene::keyPressEvent(int key, int scancode, int action, int mods)
{
	switch(key)
	{
        case 'R':
			mTheta += 5.0;
			break;
		case 'r':
			mTheta -= 5.0;
			break;
		case 'Z':
			mRow += 0.1;
			break;
		case 'z':
			mRow -= 0.1;
			break;
	}
}	