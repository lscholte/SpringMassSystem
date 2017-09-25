#include "BlankScene.hpp"
#include "Shader.hpp"
#include "Spring.hpp"
#include <atlas/core/GLFW.hpp>
#include <atlas/utils/GUI.hpp>

BlankScene::BlankScene() :
	mPaused(true)
{	
	std::unique_ptr<Spring> springRK4 = std::make_unique<Spring>();
	springRK4->setName("RK4 Spring");
	springRK4->useRK4(true);
	springRK4->getCoil().setSpringConstant(3.0f);
	springRK4->getCoil().setDampeningConstant(0.0f);
	springRK4->getCoil().setRestLength(3.0f);
	springRK4->getCoil().setFixedPosition(glm::vec3(-5.0f, 0.0f, 0.0f));	
	springRK4->getMass().setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
	springRK4->getMass().setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
	springRK4->getMass().setMass(1.0f);
	springRK4->applyTransformations();

	std::unique_ptr<Spring> springEuler = std::make_unique<Spring>();
	springEuler->setName("Euler Spring");
	springEuler->useRK4(false);
	springEuler->getCoil().setSpringConstant(3.0f);
	springEuler->getCoil().setDampeningConstant(0.0f);
	springEuler->getCoil().setRestLength(3.0f);
	springEuler->getCoil().setFixedPosition(glm::vec3(5.0f, 0.0f, 0.0f));		
	springEuler->getMass().setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	springEuler->getMass().setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));	
	springEuler->getMass().setMass(1.0f);
	springEuler->applyTransformations();

	mGeometries.push_back(std::move(springRK4));	
	mGeometries.push_back(std::move(springEuler));		
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
	
	glm::vec3 eye(0.0, 0.0, 10.0);
	glm::vec3 look(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 1.0, 0.0);
	
	glm::mat4 view = glm::lookAt(eye, look, up);
	
	//Render black background
	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiSetCond_FirstUseEver);
	
	ImGui::Begin("Scene Options");
	ImGui::Checkbox("Simulation Paused", &mPaused);
	ImGui::End();
			
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
	
	if(!mPaused)
	{
		for(auto &geometry : mGeometries)
		{
			geometry->updateGeometry(mTime);
		}
	}
}	