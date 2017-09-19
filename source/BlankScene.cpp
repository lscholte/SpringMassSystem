#include "BlankScene.hpp"
#include "Shader.hpp"
#include "Mass.hpp"
#include "Coil.hpp"
#include <atlas/core/GLFW.hpp>

BlankScene::BlankScene()
{	
	mGeometries.push_back(std::make_unique<Coil>(
		glm::scale(
			glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f)
		), glm::vec3(0.1f, 0.1f, 3.0f))
	));
	mGeometries.push_back(std::make_unique<Mass>());
	
	std::vector<atlas::gl::ShaderUnit> shaderUnits
	{
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.vert", GL_VERTEX_SHADER),
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.frag", GL_FRAGMENT_SHADER)
	};
	
	mShader = atlas::gl::Shader(shaderUnits);
	
	mShader.compileShaders();
	mShader.linkShaders();
	
}

BlankScene::~BlankScene()
{
}

void BlankScene::renderScene()
{
	mShader.enableShaders();

	float theta = 45.0f;
	float row = 3.0f;
	float aspectRatio = 1.0f;
	
	glm::vec3 eye(5.0*cos(glm::radians(theta)), 5.0*sin(glm::radians(theta)), row);
	glm::vec3 look(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 0.0, 1.0);
	
	glm::mat4 view = glm::lookAt(eye, look, up);
	glm::mat4 projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 100.0f);
	glm::mat4 viewProjection = projection * view;
	
	
	
	//Render red colour
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	for(auto &geometry : mGeometries)
	{
		glm::mat4 modelViewProjection = viewProjection * geometry->getModel();
		glUniformMatrix4fv(mShader.getUniformVariable("ModelViewProjection"), 1, GL_FALSE, &modelViewProjection[0][0]);
		geometry->renderGeometry(modelViewProjection, view);
	}
	
	mShader.disableShaders();
}
