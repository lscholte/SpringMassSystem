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

		void setCurrentLength(float currentLength);
		float getCurrentLength() const;

		void setRestLength(float restLength);		
		float getRestLength() const;

		void setModel(glm::mat4 const &model);
	
	private:
		GLuint mVao;
		GLuint mPositionBuffer, mNormalBuffer, mColorBuffer, mIndexBuffer;

		float mSpringConstant;
		float mRestLength;
		float mCurrentLength;
	
		GLfloat *mVertexPositions;
		GLfloat *mVertexColors;
		GLint *mVertexIndices;
		int mNumVertices;

		// static const GLfloat POSITIONS[][3];
		// static const GLfloat NORMALS[][3];
		// static const GLfloat COLORS[][3];
		// static const GLint INDICES[][1];
};

#endif
