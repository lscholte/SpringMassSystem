#include "Mass.hpp"
#include "Shader.hpp"

constexpr GLfloat Mass::POSITIONS[][3] = {
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

constexpr GLfloat Mass::NORMALS[][3] = {
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

constexpr GLfloat Mass::COLORS[][3] = {
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
	{0.0, 0.0, 0.9},
};

constexpr GLint Mass::INDICES[][3] = {
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

Mass::Mass() :
	mMass(1.0f)
{
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mPositionBuffer);
	glGenBuffers(1, &mNormalBuffer);
	glGenBuffers(1, &mColorBuffer);
	glGenBuffers(1, &mIndexBuffer);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mass::POSITIONS), Mass::POSITIONS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mass::NORMALS), Mass::NORMALS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mass::COLORS), Mass::COLORS, GL_STATIC_DRAW);
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

Mass::~Mass()
{
}

void Mass::setMass(float mass)
{
	//TODO: Require mass to be positive
	mMass = mass;
}

float Mass::getMass() const
{
	return mMass;
}

glm::vec3 Mass::getRestPosition() const
{
	return mRestPosition;
}

void Mass::setPosition(glm::vec3 const &position)
{
	mPosition = position;
}

glm::vec3 Mass::getPosition() const
{
	return mPosition;
}

void Mass::setVelocity(glm::vec3 const &velocity)
{
	mVelocity = velocity;
}

glm::vec3 Mass::getVelocity() const
{
	return mVelocity;
}

void Mass::setAcceleration(glm::vec3 const &acceleration)
{
	mAcceleration = acceleration;
}

glm::vec3 Mass::getAcceleration() const
{
	return mAcceleration;
}

void Mass::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{	
	mShaders[0].enableShaders();
	
	glm::mat4 modelViewProjection = projection * view * mModel;
	// glUniformMatrix4fv(mUniforms["ModelViewProjection"], 1, GL_FALSE, &modelViewProjection[0][0]);

	const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
    const GLint MODEL_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "Model");
	
	glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);
	glUniformMatrix4fv(MODEL_UNIFORM_LOCATION, 1, GL_FALSE, &mModel[0][0]);

	//If I don't do this, my faces will be inside out
	//because I specified the cube vertices in clockwise order
	glFrontFace(GL_CW);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mass::INDICES), Mass::INDICES, GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, sizeof(Mass::INDICES)/sizeof(GLint)*3, GL_UNSIGNED_INT, (void *) 0);

	glBindVertexArray(0);

	mShaders[0].disableShaders();
}

void Mass::updateGeometry(atlas::core::Time<> const &t)
{
}

void Mass::setModel(glm::mat4 const &model)
{
	mModel = model;
}
