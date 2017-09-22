#include "BlankScene.hpp"
#include "Shader.hpp"
#include "Spring.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/utils/GUI.hpp>

BlankScene::BlankScene()
{	
	std::unique_ptr<Spring> spring = std::make_unique<Spring>();
	spring->getCoil().setSpringConstant(3.0f);
	spring->getCoil().setDampeningConstant(0.0f);
	spring->getCoil().setRestLength(3.0f);
	spring->getMass().setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
	spring->getMass().setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));	
	spring->getMass().setMass(1.0f);
	mGeometries.push_back(std::move(spring));	
}

BlankScene::~BlankScene()
{
}

void BlankScene::mousePressEvent(int button, int action, int modifiers, double xPos, double yPos)
{
	atlas::utils::Gui::getInstance().mousePressed(button, action, modifiers);
}

void BlankScene::mouseMoveEvent(double xPos, double yPos)
{
	atlas::utils::Gui::getInstance().mouseMoved(xPos, yPos);
}

void BlankScene::mouseScrollEvent(double xOffset, double yOffset)
{
	atlas::utils::Gui::getInstance().mouseScroll(xOffset, yOffset);
}

void BlankScene::keyPressEvent(int key, int scancode, int action, int mods)
{
	atlas::utils::Gui::getInstance().keyPress(key, scancode, action, mods);
}

void BlankScene::screenResizeEvent(int width, int height)
{
	glViewport(0, 0, width, height);

	float aspectRatio = (float) width / height;
	mProjection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 100.0f);	

	atlas::utils::Gui::getInstance().screenResize(width, height);

	mWidth = (float) width;
	mHeight = (float) height;
}

void BlankScene::renderScene()
{
	atlas::utils::Gui::getInstance().newFrame();
	
	float aspectRatio = 1.0f;
	
	glm::vec3 eye(10.0*cos(glm::radians(mTheta)), mRow, 10.0*sin(glm::radians(mTheta)));
	glm::vec3 look(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);
	
	glm::mat4 view = glm::lookAt(eye, look, up);
	
	//Render black background
	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
	for(auto &geometry : mGeometries)
	{
		geometry->drawGui();
		geometry->renderGeometry(mProjection, view);
	}

	ImGui::Render();				
}

void BlankScene::updateScene(double time)
{
	mTime.deltaTime = time - mTime.currentTime;
	mTime.totalTime += mTime.deltaTime;
	mTime.currentTime = time;

	atlas::utils::Gui::getInstance().update(mTime);
	
	for(auto &geometry : mGeometries)
	{
		geometry->updateGeometry(mTime);
	}
}	