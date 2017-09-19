#include "Coil.hpp"
#include "Shader.hpp"
#include <atlas/core/GLFW.hpp>

constexpr GLfloat Coil::POSITIONS[][3] = {
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	{0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{-0.5, -0.5, -0.5},
	
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	{0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{-0.5, -0.5, -0.5},
	
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	{0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{-0.5, 0.5, 0.5},
	{-0.5, 0.5, -0.5},
	{-0.5, -0.5, 0.5},
	{-0.5, -0.5, -0.5},
};

constexpr GLfloat Coil::NORMALS[][3] = {
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	{0.0, 0.0, 1.0},
	{0.0, 0.0, -1.0},
	
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	{-1.0, 0.0, 0.0},
	
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0},
	{0.0, -1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, 1.0, 0.0},
	{0.0, -1.0, 0.0},
	{0.0, -1.0, 0.0},
};

constexpr GLfloat Coil::COLORS[][3] = {
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
	{0.9, 0.0, 0.0},
};

constexpr GLint Coil::INDICES[][3] = {
	{5, 7, 3},
	{3, 1, 5}, //Face 1 -z
	{6+8, 7+8, 5+8},
	{5+8, 4+8, 6+8}, //Face 2 -x
	{3+8, 2+8, 0+8},
	{0+8, 1+8, 3+8}, //Face 3 +x
	{6, 4, 0},
	{0, 2, 6}, //Face 4 +z
	{5+16, 1+16, 0+16},
	{0+16, 4+16, 5+16}, //Face 5 +y
	{7+16, 6+16, 3+16},
	{3+16, 6+16, 2+16}, //Face 6 -y
};

Coil::Coil()
{
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mPositionBuffer);
	glGenBuffers(1, &mNormalBuffer);
	glGenBuffers(1, &mColorBuffer);
	glGenBuffers(1, &mIndexBuffer);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Coil::POSITIONS), Coil::POSITIONS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Coil::NORMALS), Coil::NORMALS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Coil::COLORS), Coil::COLORS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);
	
	std::vector<atlas::gl::ShaderUnit> shaderUnits
	{
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.vert", GL_VERTEX_SHADER),
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.frag", GL_FRAGMENT_SHADER)
	};
	
	mShader = atlas::gl::Shader(shaderUnits);
	
	mShader.compileShaders();	
	mShader.linkShaders();
	
	mUniforms.insert(UniformKey("ModelViewProjection", mShader.getUniformVariable("ModelViewProjection")));
}

Coil::~Coil()
{
}

void Coil::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
	mShader.enableShaders();
	
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 3.0f));
	glm::mat4 modelViewProjection = projection * view * model;
	glUniformMatrix4fv(mUniforms["ModelViewProjection"], 1, GL_FALSE, &modelViewProjection[0][0]);
	
	//If I don't do this, my faces will be inside out
	//because I specified the cube vertices in clockwise order
	glFrontFace(GL_CW);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Coil::INDICES), Coil::INDICES, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, sizeof(Coil::INDICES), GL_UNSIGNED_INT, (void *) 0);

	glBindVertexArray(0);
	
	mShader.disableShaders();
}

void Coil::updateGeometry(atlas::core::Time<> const &t)
{
}
