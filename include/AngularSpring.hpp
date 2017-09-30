#ifndef AngularSpring_hpp
#define AngularSpring_hpp

#include <atlas/utils/Geometry.hpp>
#include "Mass.hpp"
#include "Coil.hpp"

class AngularSpring : public atlas::utils::Geometry
{
	public:
		AngularSpring();
		~AngularSpring();
	
		void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;
		void updateGeometry(atlas::core::Time<> const &t) override;
		void transformGeometry(atlas::math::Matrix4 const& t) override;
		void drawGui() override;		

        Coil& getCoil();
        
        void setRestAngle(float angle);

        void setAngle(float angle);

        void setAngleSpeed(float angleSpeed);

		void applyTransformations();

		void useRK4(bool use);
		void setName(std::string name);
	
	private:
		Mass mMass;
		Coil mCoil;

		bool mUseRK4;
		std::string mName;

		float computeAngleSpeed(float deltaTime);		
		float computeAngleSpeed(float deltaTime, float angleSpeed);
		float computeAngleAccelerationFromTime(float deltaTime, float angleSpeed);		
        float computeAngleAcceleration(float angle, float angleSpeed);
        
        float mRestAngle;
        float mAngle;
        float mAngleSpeed;
        float mAngleAcceleration;
};

#endif
