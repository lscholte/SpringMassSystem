#include "Spring.hpp"

Spring::Spring()
{
	mCoil.transformGeometry(
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)), glm::vec3(0.1f, 3.0f, 0.1f))
	);
}

Spring::~Spring()
{
}

void Spring::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
	mMass.renderGeometry(projection, view);
	mCoil.renderGeometry(projection, view);
}

void Spring::updateGeometry(atlas::core::Time<> const &t)
{
	constexpr float g = 9.81;
	
	const glm::vec3 forceGravity(0, -mMass.getMass()*g, 0);
	const glm::vec3 forceSpring(
		0,
		mCoil.getSpringConstant()*(mCoil.getCurrentLength() - mCoil.getRestLength()),
		0
	);

	const glm::vec3 acceleration = (forceGravity + forceSpring) / mMass.getMass();
	const glm::vec3 velocity = mMass.getVelocity() + acceleration*t.deltaTime; //TODO: What are time units of deltaTime?
	const glm::vec3 position = mMass.getPosition() + velocity*t.deltaTime;

	const glm::vec3 deltaPosition = position - mMass.getPosition();
	mMass.transformGeometry(glm::translate(glm::mat4(1.0f), deltaPosition));

	mMass.setVelocity(velocity);
	mMass.setPosition(position);

	mCoil.setCurrentLength(mCoil.getCurrentLength() - deltaPosition.y);
}
