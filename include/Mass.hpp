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
	
		static const GLfloat POSITIONS[][3];
		static const GLfloat NORMALS[][3];
		static const GLfloat COLORS[][3];
		static const GLint INDICES[][3];
	
	private:
		GLuint mVao;
		GLuint mPositionBuffer, mNormalBuffer, mColorBuffer, mIndexBuffer;
		atlas::gl::Shader mShader;
	
};

#endif
