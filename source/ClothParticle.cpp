#include "ClothParticle.hpp"
#include "Shader.hpp"
#include <iostream>

constexpr GLfloat ClothParticle::POSITIONS[][3] = {
    {0.0, 0.0, 0.0}
};

constexpr GLfloat ClothParticle::COLORS[][3] = {
	{1.0, 0.0, 0.0},
};

constexpr GLint ClothParticle::INDICES[][1] = {
	{1}
};

ClothParticle::ClothParticle() :
    mPositionFixed(false)
{
    glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mPositionBuffer);
	glGenBuffers(1, &mColorBuffer);
	glGenBuffers(1, &mIndexBuffer);
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ClothParticle::POSITIONS), ClothParticle::POSITIONS, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	
	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ClothParticle::COLORS), ClothParticle::COLORS, GL_STATIC_DRAW);
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

void ClothParticle::setFixed(bool fixed)
{
    mPositionFixed = fixed;
}

void ClothParticle::setMass(float mass)
{
    mMass = mass;
}

float ClothParticle::getMass() const
{
    return mMass;
}

void ClothParticle::setPosition(glm::vec3 const &position)
{
    mPosition = position;
}

glm::vec3 ClothParticle::getPosition() const
{
    return mPosition;
}

void ClothParticle::setVelocity(glm::vec3 const &velocity)
{
    mVelocity = velocity;
}

glm::vec3 ClothParticle::getVelocity() const
{
    return mVelocity;
}

void ClothParticle::setAcceleration(glm::vec3 const &acceleration)
{
    mAcceleration = acceleration;
}

glm::vec3 ClothParticle::getAcceleration() const
{
    return mAcceleration;
}

void ClothParticle::setDampeningConstant(float dampeningConstant)
{
    mDampeningConstant = dampeningConstant;
}

float ClothParticle::getDampeningConstant() const
{
    return mDampeningConstant;
}

void ClothParticle::updateGeometry(atlas::core::Time<> const &t)
{
    if(mPositionFixed)
    {
        mVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
        mAcceleration = glm::vec3(0.0f, 0.0f, 0.0f);
        mNetForce = glm::vec3(0.0f, 0.0f, 0.0f); 

        applyTransformations();        
        return;
    }

    glm::vec3 newPosition;
	glm::vec3 newVelocity;
	if(true) //use RK4
	{
		//Compute the new position by Runge-Kutta Order 4.
		//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
        // glm::vec3 k1Pos = t.deltaTime * computeVelocity(0, mVelocity);
        // glm::vec3 k1Vel = t.deltaTime * computeAcceleration(mPosition, mVelocity);
        
        // glm::vec3 k2Pos = t.deltaTime * computeVelocity(0.5*t.deltaTime, mVelocity + 0.5f*k1Vel);
        // glm::vec3 k2Vel = t.deltaTime * computeAcceleration(mPosition + 0.5f*k1Pos, mVelocity + 0.5f*k1Vel);

        // glm::vec3 k3Pos = t.deltaTime * computeVelocity(0.5*t.deltaTime, mVelocity + 0.5f*k2Vel);
        // glm::vec3 k3Vel = t.deltaTime * computeAcceleration(mPosition + 0.5f*k2Pos, mVelocity + 0.5f*k2Vel);
        
        // glm::vec3 k4Pos = t.deltaTime * computeVelocity(t.deltaTime, mVelocity + k3Vel);
        // glm::vec3 k4Vel = t.deltaTime * computeAcceleration(mPosition + k3Pos, mVelocity + k3Vel);
        
        // newPosition = mPosition + (1.0f / 6.0f) * (k1Pos + 2.0f*k2Pos + 2.0f*k3Pos + k4Pos);
        // newVelocity = mVelocity + (1.0f / 6.0f) * (k1Vel + 2.0f*k2Vel + 2.0f*k3Vel + k4Vel);   
        
        glm::vec3 v1 = t.deltaTime * computeVelocity(0);
		glm::vec3 v2 = t.deltaTime * computeVelocity(0.5*t.deltaTime);
		glm::vec3 v3 = t.deltaTime * computeVelocity(0.5*t.deltaTime);
		glm::vec3 v4 = t.deltaTime * computeVelocity(t.deltaTime);
		newPosition = mPosition + (1.0f / 6.0f) * (v1 + 2.0f*v2 + 2.0f*v3 + v4);

		//Compute the new velocity by Runge-Kutta Order 4
		//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
		glm::vec3 a1 = t.deltaTime * computeAcceleration(0, mVelocity);
		glm::vec3 a2 = t.deltaTime * computeAcceleration(0.5*t.deltaTime, mVelocity + 0.5f*a1);
		glm::vec3 a3 = t.deltaTime * computeAcceleration(0.5*t.deltaTime, mVelocity + 0.5f*a2);
		glm::vec3 a4 = t.deltaTime * computeAcceleration(t.deltaTime, mVelocity + a3);
		newVelocity = mVelocity + (1.0f / 6.0f) * (a1 + 2.0f*a2 + 2.0f*a3 + a4);

		//Compute the new velocity by Runge-Kutta Order 4
		//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
		//Update acceleration so that we use it to compute velocity
		//next the next time we execute Runge-Kutta
		mAcceleration = (newVelocity - mVelocity) / t.deltaTime;
	}
	else
	{
		//Compute new position and velocity by Euler integration
		// newPosition = mPosition + mVelocity*t.deltaTime;
        // newVelocity = mVelocity + computeAcceleration(mPosition, mVelocity)*t.deltaTime;
		// newVelocity = mVelocity + computeAcceleration(mPosition, mVelocity)*t.deltaTime;        
        // newPosition = mPosition + newVelocity*t.deltaTime;

        glm::vec3 a_t = computeAcceleration(0, mVelocity);
        newPosition = mPosition + mVelocity*t.deltaTime + 0.5f*a_t*t.deltaTime*t.deltaTime;
        newVelocity = mVelocity + 0.5f*(a_t + computeAcceleration(t.deltaTime, computeVelocity(t.deltaTime)))*t.deltaTime;


	}

	//Update the mass's velocity and position states
    // this->setPosition(newPosition);
    mPosition = newPosition;
	mVelocity = newVelocity;

    mNetForce = glm::vec3(0.0f, 0.0f, 0.0f);    

	// applyTransformations();
}

bool ClothParticle::isFixed()
{
    return mPositionFixed;
}

void ClothParticle::applyTransformations()
{
    setModel(glm::mat4(1.0f));
    transformGeometry(glm::translate(glm::mat4(1.0f), mPosition)); 
}

glm::vec3 ClothParticle::computeVelocity(float deltaTime, glm::vec3 const &velocity)
{
    return velocity + mAcceleration*deltaTime;    
}

glm::vec3 ClothParticle::computeVelocity(float deltaTime)
{
    return computeVelocity(deltaTime, mVelocity);  
}

glm::vec3 ClothParticle::computeAcceleration(float deltaTime, glm::vec3 const &velocity)		
{
	glm::vec3 position = mPosition + velocity*deltaTime;
	return computeAcceleration(position, velocity);
}

glm::vec3 ClothParticle::computeAcceleration(glm::vec3 const &position, glm::vec3 const &velocity)
{
    const float g = 9.81;
    glm::vec3 forceGravity(0.0f, -g*mMass, 0.0f);

    mNetForce += forceGravity;

    return mNetForce / mMass;    
}

void ClothParticle::addForce(glm::vec3 const &force)
{
    mNetForce += force;
}

void ClothParticle::setModel(glm::mat4 const &model)
{
	mModel = model;
}

void ClothParticle::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
	mShaders[0].enableShaders();
	
	glm::mat4 modelViewProjection = projection * view * mModel;
	const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
	glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);

	//If I don't do this, my faces will be inside out
	//because I specified the cube vertices in clockwise order
    glFrontFace(GL_CW);
    glPointSize(1.5f);    
	
	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ClothParticle::INDICES), ClothParticle::INDICES, GL_STATIC_DRAW);
	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (void *) 0);

	glBindVertexArray(0);

	mShaders[0].disableShaders();}