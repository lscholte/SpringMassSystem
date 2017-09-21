#include "Spring.hpp"
#include <iostream>

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
	//Resets the model matrices so that we can re-apply scaling and/or translations to them
	mCoil.setModel(glm::mat4(1.0f));
	mMass.setModel(glm::mat4(1.0f));

	const float g = 9.81;

	//Compute the coil length and the axis in which it lies
	const float length = glm::length(mCoil.getFixedPosition() - mMass.getPosition());
	const glm::vec3 coilAxis = glm::normalize(mCoil.getFixedPosition() - mMass.getPosition());

	//Compute the two forces acting upon the spring
	const glm::vec3 forceGravity(0, -mMass.getMass()*g, 0);
	const glm::vec3 forceSpring = mCoil.getSpringConstant() * coilAxis * (length - mCoil.getRestLength());
	const glm::vec3 dampeningForce = -mCoil.getDampeningConstant() * mMass.getVelocity();

	//Sum up the forces to obtain the net force
	const glm::vec3 netForce = forceGravity + forceSpring + dampeningForce;

	//Compute the acceleration of the mass using the net force
	const glm::vec3 acceleration = netForce / mMass.getMass();	

	//Compute the new position and velocity of the mass
	const glm::vec3 newPosition = mMass.getPosition() + mMass.getVelocity()*t.deltaTime;
	const glm::vec3 newVelocity = mMass.getVelocity() + acceleration*t.deltaTime;

	//Translate the mass to its new position
	mMass.transformGeometry(glm::translate(glm::mat4(1.0f), newPosition));

	//Compute the new length of the coil
	float newLength = glm::length(mCoil.getFixedPosition() - newPosition);

	//Scale the coil to its new length
	mCoil.transformGeometry(glm::scale(glm::mat4(1.0f), glm::vec3(1.0, newLength/mCoil.getRestLength(), 1.0)));

	//Rotate the coil so that it is aligned with the mass
	const glm::vec3 newCoilAxis = glm::normalize(mCoil.getFixedPosition() - mMass.getPosition());	
	glm::vec3 w = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), newCoilAxis);
	float theta = acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), newCoilAxis));
	mCoil.transformGeometry(glm::rotate(glm::mat4(1.0f), theta, w));

	//Update the mass's velocity and position states
	mMass.setVelocity(newVelocity);
	mMass.setPosition(newPosition);
}

void Spring::transformGeometry(atlas::math::Matrix4 const& t)
{
	mCoil.transformGeometry(t);
	mMass.transformGeometry(t);	
}
