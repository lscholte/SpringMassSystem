#include "Coil.hpp"
#include "Shader.hpp"
#include <atlas/core/GLFW.hpp>

Coil::Coil() :
	mSpringConstant(5.0f),
	mCurrentLength(3.0f)
{
	setRestLength(3.0f);

	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mPositionBuffer);
	glGenBuffers(1, &mColorBuffer);
	glGenBuffers(1, &mIndexBuffer);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*mNumVertices*sizeof(GLfloat), mVertexPositions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 3*mNumVertices*sizeof(GLfloat), mVertexColors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindVertexArray(0);

	std::vector<atlas::gl::ShaderUnit> shaderUnits
	{
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.vert", GL_VERTEX_SHADER),
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.frag", GL_FRAGMENT_SHADER)
	};
	
	mShaders.push_back(atlas::gl::Shader(shaderUnits));
	
	mShaders[0].compileShaders();
	mShaders[0].linkShaders();
}

Coil::~Coil()
{
	delete [] mVertexPositions;
	delete [] mVertexColors;
	delete [] mVertexIndices;
}

void Coil::setSpringConstant(float springConstant)
{
	mSpringConstant = springConstant;
}

float Coil::getSpringConstant() const
{
	return mSpringConstant;
}

void Coil::setCurrentLength(float currentLength)
{
	//TODO: Check that lenght > 0
	mCurrentLength = currentLength;
}

float Coil::getCurrentLength() const
{
	return mCurrentLength;
}

void Coil::setRestLength(float restLength)
{
	mRestLength = restLength;

	constexpr float PI = 3.14159;	
	constexpr int verticesPerUnitLength = 100;
	constexpr float coilsPerUnitLength = 3;
	
	mNumVertices = verticesPerUnitLength * restLength;
	mVertexPositions = new GLfloat[mNumVertices*3];
	mVertexColors = new GLfloat[mNumVertices*3];
	mVertexIndices = new GLint[mNumVertices];

	for(int i = 0; i < mNumVertices; ++i)
	{
		mVertexPositions[3*i] = static_cast<GLfloat>(0.5*cos(2*i*PI/(verticesPerUnitLength/coilsPerUnitLength)));
		mVertexPositions[3*i+1] = (GLfloat) i / verticesPerUnitLength;
		mVertexPositions[3*i+2] = static_cast<GLfloat>(0.5*sin(2*i*PI/(verticesPerUnitLength/coilsPerUnitLength)));

		mVertexColors[3*i] = 0.9f;
		mVertexColors[3*i+1] = 0.0f;
		mVertexColors[3*i+2] = 0.0f;

		mVertexIndices[i] = i;
	}
}

float Coil::getRestLength() const
{
	return mRestLength;
}

void Coil::setModel(glm::mat4 const &model)
{
	mModel = model;
}

void Coil::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
	mShaders[0].enableShaders();
	
	glm::mat4 modelViewProjection = projection * view * mModel;
	glUniformMatrix4fv(mShaders[0].getUniformVariable("ModelViewProjection"), 1, GL_FALSE, &modelViewProjection[0][0]);
	
	//If I don't do this, my faces will be inside out
	//because I specified the cube vertices in clockwise order
	glFrontFace(GL_CW);
	glLineWidth(10.0f);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumVertices*sizeof(GLfloat), mVertexIndices, GL_STATIC_DRAW);
	glDrawElements(GL_LINE_STRIP, mNumVertices, GL_UNSIGNED_INT, (void *) 0);

	glBindVertexArray(0);

	mShaders[0].disableShaders();	
}

void Coil::updateGeometry(atlas::core::Time<> const &t)
{
}
