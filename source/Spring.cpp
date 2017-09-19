#include "Spring.hpp"

Spring::Spring()
{
	mCoil.transformGeometry(
		glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f)), glm::vec3(0.1f, 0.1f, 3.0f))
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
}
