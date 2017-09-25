#include "Spring.hpp"
#include <iostream>
#include <atlas/utils/GUI.hpp>

Spring::Spring()
{
}

Spring::~Spring()
{
}

Mass& Spring::getMass()
{
	return mMass;
}

Coil& Spring::getCoil()
{
	return mCoil;
}

void Spring::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
	mMass.renderGeometry(projection, view);
	mCoil.renderGeometry(projection, view);
}

void Spring::updateGeometry(atlas::core::Time<> const &t)
{	
	//Compute the new position by Runge-Kutta Order 4.
	//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
	glm::vec3 v1 = t.deltaTime * computeVelocity(0);
	glm::vec3 v2 = t.deltaTime * computeVelocity(0.5*t.deltaTime);
	glm::vec3 v3 = t.deltaTime * computeVelocity(0.5*t.deltaTime);
	glm::vec3 v4 = t.deltaTime * computeVelocity(t.deltaTime);
	glm::vec3 newPosition = mMass.getPosition() + (1.0f / 6.0f) * (v1 + 2.0f*v2 + 2.0f*v3 + v4);

	//Compute the new velocity by Runge-Kutta Order 4
	//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
	glm::vec3 a1 = t.deltaTime * computeAcceleration(0, mMass.getVelocity());
	glm::vec3 a2 = t.deltaTime * computeAcceleration(0.5*t.deltaTime, mMass.getVelocity() + 0.5f*a1);
	glm::vec3 a3 = t.deltaTime * computeAcceleration(0.5*t.deltaTime, mMass.getVelocity() + 0.5f*a2);
	glm::vec3 a4 = t.deltaTime * computeAcceleration(t.deltaTime, mMass.getVelocity() + a3);
	glm::vec3 newVelocity = mMass.getVelocity() + (1.0f / 6.0f) * (a1 + 2.0f*a2 + 2.0f*a3 + a4);

	//Update acceleration so that we use it to compute velocity
	//next the next time we execute Runge-Kutta
	mMass.setAcceleration((newVelocity - mMass.getVelocity()) / t.deltaTime);

	//Compute new position and velocity by Euler integration
	// const glm::vec3 newPosition = mMass.getPosition() + mMass.getVelocity()*t.deltaTime;
	// const glm::vec3 newVelocity = mMass.getVelocity() + computeAcceleration(mMass.getPosition(), mMass.getVelocity())*t.deltaTime;

	//Update the mass's velocity and position states
	mMass.setPosition(newPosition);
	mMass.setVelocity(newVelocity);

	//Resets the model matrices so that we can re-apply scaling and/or translations to them
	mCoil.setModel(glm::mat4(1.0f));
	mMass.setModel(glm::mat4(1.0f));
	
	//Compute the axis of the coil and the length of the coil
	glm::vec3 coilAxis = mCoil.getFixedPosition() - mMass.getPosition();
	float coilLength = glm::length(coilAxis);

	//Scale the coil to its correct length
	mCoil.transformGeometry(glm::scale(glm::mat4(1.0f), glm::vec3(1.0, coilLength/mCoil.getRestLength(), 1.0)));
	
	if(coilLength > 0)
	{
		//Move the mass down by the length of the coil
		mMass.transformGeometry(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -coilLength, 0.0)));

		//Convert the coil axis to a unit vector
		coilAxis /= coilLength;

		//If coil is perfectly vertical
		glm::vec3 w = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), coilAxis);
		if(w == glm::vec3(0.0f, 0.0f, 0.0f))
		{
			//We may need to flip the entire spring
			if(mMass.getPosition().y > 0)
			{
				this->transformGeometry(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f)));				
			}
		}

		//Otherwise we rotate the entire spring so that it is algined with its correct axis
		else
		{
			float theta = acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), coilAxis));
			this->transformGeometry(glm::rotate(glm::mat4(1.0f), theta, w));
		}
	}
}

glm::vec3 Spring::computeVelocity(float deltaTime)
{
	return mMass.getVelocity() + mMass.getAcceleration()*deltaTime;
}

glm::vec3 Spring::computeAcceleration(float deltaTime, glm::vec3 const &velocity)
{
	glm::vec3 position = mMass.getPosition() + velocity*deltaTime;
	return computeAcceleration(position, velocity);
}

glm::vec3 Spring::computeAcceleration(glm::vec3 const &position, glm::vec3 const &velocity)
{
	//Compute the force of gravity
	constexpr float g = 9.81;
	glm::vec3 forceGravity(0, -mMass.getMass()*g, 0);
	
	//Compute the force of the spring. Set to 0 if length of the spring is 0
	float length = glm::length(mCoil.getFixedPosition() - position);
	glm::vec3 forceSpring;
	if(length == 0)
	{
		forceSpring = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		glm::vec3 coilAxis = glm::normalize(mCoil.getFixedPosition() - position);	
		forceSpring = mCoil.getSpringConstant() * coilAxis * (length - mCoil.getRestLength());
	}

	//Compute the dampening force
	glm::vec3 forceDampening = -mCoil.getDampeningConstant() * velocity;

	//Add up the forces to get net force and solve for the acceleration
	glm::vec3 netForce = forceGravity + forceSpring + forceDampening;
	return netForce / mMass.getMass();
}

void Spring::drawGui()
{
	float ks = mCoil.getSpringConstant();
	float kd = mCoil.getDampeningConstant();
	float length = glm::length(mCoil.getFixedPosition() - mMass.getPosition());
	
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_FirstUseEver);
	
	ImGui::Begin("Spring Options");
	ImGui::SliderFloat("Spring Constant", &ks, 0.0f, 50.0f);
	ImGui::SliderFloat("Dampening Constant", &kd, 0.0f, 50.0f);
	ImGui::End();

	mCoil.setSpringConstant(ks);
	mCoil.setDampeningConstant(kd);
}

void Spring::transformGeometry(atlas::math::Matrix4 const& t)
{
	mCoil.transformGeometry(t);
	mMass.transformGeometry(t);	
}
