#ifndef Mass_hpp
#define Mass_hpp

#include <atlas/utils/Geometry.hpp>

class Mass : public atlas::utils::Geometry
{
	public:
		Mass();	
		~Mass();
	
		void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;
		void updateGeometry(atlas::core::Time<> const &t) override;
	
		void setMass(float mass);
		float getMass() const;

		glm::vec3 getRestPosition() const;

		void setPosition(glm::vec3 const &position);
		glm::vec3 getPosition() const;

		void setVelocity(glm::vec3 const &velocity);
		glm::vec3 getVelocity() const;

		void setAcceleration(glm::vec3 const &acceleration);
		glm::vec3 getAcceleration() const;

		void setModel(glm::mat4 const &model);		
	
	private:
		GLuint mVao;
		GLuint mPositionBuffer, mNormalBuffer, mColorBuffer, mIndexBuffer;
	
		//Mass of the object in kilograms
		float mMass;
		glm::vec3 mRestPosition;
		glm::vec3 mPosition;
		glm::vec3 mVelocity;
		glm::vec3 mAcceleration;
	
		static const GLfloat POSITIONS[][3];
		static const GLfloat NORMALS[][3];
		static const GLfloat COLORS[][3];
		static const GLint INDICES[][3];
};

#endif
