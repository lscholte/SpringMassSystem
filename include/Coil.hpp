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

		float getRestLength() const;
	
	private:
		GLuint mVao;
		GLuint mPositionBuffer, mNormalBuffer, mColorBuffer, mIndexBuffer;

		float mSpringConstant;
		const float mRestLength;
		float mCurrentLength;
	
		static const GLfloat POSITIONS[][3];
		static const GLfloat NORMALS[][3];
		static const GLfloat COLORS[][3];
		static const GLint INDICES[][3];
};

#endif
