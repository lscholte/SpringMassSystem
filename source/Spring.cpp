#include "Spring.hpp"

Spring::Spring()
{
	mCoil.transformGeometry(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.5f, 0.0f)));
	mCoil.setPosition(glm::vec3(0.0f, 0.5f, 0.0f));
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
	//Resets the coil model matrix so that we can re-apply scaling and translations to it
	mCoil.setModel(glm::mat4(1.0f));

	float g = 9.81;

	//Compute the two forces acting upon the spring
	const glm::vec3 forceGravity(0, -mMass.getMass()*g, 0);
	const glm::vec3 forceSpring(
		0,
		mCoil.getSpringConstant()*(mCoil.getCurrentLength() - mCoil.getRestLength()) - mCoil.getDampeningConstant()*mCoil.getVelocity().y,
		0
	);

	//Determine the new position of the mass
	const glm::vec3 acceleration = (forceGravity + forceSpring) / mMass.getMass();
	const glm::vec3 velocity = mMass.getVelocity() + acceleration*t.deltaTime;
	const glm::vec3 position = mMass.getPosition() + velocity*t.deltaTime;

	//Determine the change in position of the mass since the previous timestep
	//and translate the mass by that amount
	const glm::vec3 deltaPosition = position - mMass.getPosition();
	mMass.transformGeometry(glm::translate(glm::mat4(1.0f), deltaPosition));

	//Update the mass's velocity and position states
	mMass.setVelocity(velocity);
	mMass.setPosition(position);

	mCoil.setCurrentLength(mCoil.getCurrentLength() - deltaPosition.y);

	// mModel = glm::mat4(1.0f);
	const glm::vec3 coilNewPosition = mCoil.getPosition() + deltaPosition;
	mCoil.transformGeometry(glm::scale(glm::mat4(1.0f), glm::vec3(1.0, mCoil.getCurrentLength()/mCoil.getRestLength(), 1.0)));
	mCoil.transformGeometry(glm::translate(glm::mat4(1.0f), coilNewPosition));
	
	mCoil.setPosition(coilNewPosition);
	mCoil.setVelocity(velocity);	
}
