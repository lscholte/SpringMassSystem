#ifndef BlankScene_hpp
#define BlankScene_hpp

#include <atlas/utils/Scene.hpp>

class BlankScene : public atlas::utils::Scene
{
	public:
		BlankScene();
		~BlankScene();
	
		void renderScene() override;
		void updateScene(double time) override;
		void keyPressEvent(int key, int scancode, int action, int mods) override;		
		
	private:
		float mGravitationalConstant;
		float mTheta = 45.0f;
		float mRow = 0.0f;
};

#endif
