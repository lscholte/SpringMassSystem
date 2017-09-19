#ifndef BlankScene_hpp
#define BlankScene_hpp

#include <atlas/utils/Scene.hpp>

class BlankScene : public atlas::utils::Scene
{
	public:
		BlankScene();
		~BlankScene();
	
		void renderScene() override;
	
	private:
		atlas::gl::Shader mShader;
};

#endif
