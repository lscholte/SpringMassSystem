#ifndef Coil_hpp
#define Coil_hpp

#include <atlas/utils/Geometry.hpp>

class Coil : public atlas::utils::Geometry
{
	public:
		Coil();	
		~Coil();
	
		void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;
		void updateGeometry(atlas::core::Time<> const &t) override;

		void setSpringConstant(float springConstant);
		float getSpringConstant() const;

		void setDampeningConstant(float dampeningConstant);
		float getDampeningConstant() const;

		void setCurrentLength(float currentLength);
		float getCurrentLength() const;

		void setRestLength(float restLength);		
		float getRestLength() const;

		void setPosition(glm::vec3 position);
		glm::vec3 getPosition() const;

		void setVelocity(glm::vec3 velocity);
		glm::vec3 getVelocity() const;

		void setModel(glm::mat4 const &model);
	
	private:
		GLuint mVao;
		GLuint mPositionBuffer, mNormalBuffer, mColorBuffer, mIndexBuffer;

		float mSpringConstant;
		float mDampeningConstant;
		float mRestLength;
		float mCurrentLength;
		glm::vec3 mPosition;
		glm::vec3 mVelocity;
	
		GLfloat *mVertexPositions;
		GLfloat *mVertexColors;
		GLint *mVertexIndices;
		int mNumVertices;
};

#endif
