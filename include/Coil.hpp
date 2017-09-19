#ifndef Coil_hpp
#define Coil_hpp

#include <atlas/utils/Geometry.hpp>

class Coil : public atlas::utils::Geometry
{
	public:
		Coil();
		Coil(glm::mat4 const &t);
	
		~Coil();
	
		void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;
		void updateGeometry(atlas::core::Time<> const &t) override;
	
	private:
		GLuint mVao;
		GLuint mPositionBuffer, mNormalBuffer, mColorBuffer, mIndexBuffer;
	
		static const GLfloat POSITIONS[][3];
		static const GLfloat NORMALS[][3];
		static const GLfloat COLORS[][3];
		static const GLint INDICES[][3];
};

#endif
