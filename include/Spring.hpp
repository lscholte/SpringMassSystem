#ifndef Spring_hpp
#define Spring_hpp

#include <atlas/utils/Geometry.hpp>
#include "Mass.hpp"
#include "Coil.hpp"

class Spring : public atlas::utils::Geometry
{
	public:
		Spring();
		~Spring();
	
		void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;
		void updateGeometry(atlas::core::Time<> const &t) override;
		void transformGeometry(atlas::math::Matrix4 const& t) override;
		void drawGui() override;		

		Mass& getMass();
		Coil& getCoil();
	
	private:
		Mass mMass;
		Coil mCoil;

		glm::vec3 computeVelocity(float deltaTime);
		glm::vec3 computeAcceleration(float deltaTime, glm::vec3 const &velocity);		
		glm::vec3 computeAcceleration(glm::vec3 const &position, glm::vec3 const &velocity);
};

#endif
