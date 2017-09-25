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

		float getCurrentLength() const;

		void setRestLength(float restLength);		
		float getRestLength() const;

		void setFixedPosition(glm::vec3 const &position);
		glm::vec3 getFixedPosition() const;

		void setModel(glm::mat4 const &model);
	
	private:
		GLuint mVao;
		GLuint mPositionBuffer, mNormalBuffer, mColorBuffer, mIndexBuffer;

		GLfloat *mVertexPositions;
		GLfloat *mVertexColors;
		GLint *mVertexIndices;
		int mNumVertices;

		float mSpringConstant;
		float mDampeningConstant;
		float mRestLength;
		glm::vec3 mFixedPosition;
};

#endif
