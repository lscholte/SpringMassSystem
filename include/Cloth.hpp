#ifndef Cloth_hpp
#define Cloth_hpp

#include "ClothParticle.hpp"

#include <atlas/utils/Geometry.hpp>
#include <vector>

class Cloth : public atlas::utils::Geometry
{
    public:
        Cloth();
        ~Cloth();

        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;
        void updateGeometry(atlas::core::Time<> const &t) override;
        void drawGui() override;		        

        void setMass(float mass);
        float getMass();

    private:
        std::vector<std::unique_ptr<ClothParticle>> mParticles;
        std::vector<std::unique_ptr<ClothCoil>> mStructuralCoils;
        std::vector<std::unique_ptr<ClothCoil>> mShearCoils;
        std::vector<std::unique_ptr<ClothCoil>> mBendCoils;
        

        GLuint mVao;
        GLuint mPositionBuffer, mNormalBuffer, mColorBuffer, mTextureBuffer, mIndexBuffer;
        GLuint mTextureId;

        std::vector<glm::vec3> mVertexPositions;
        std::vector<glm::vec3> mVertexNormals;
        std::vector<glm::vec3> mVertexColors;
        std::vector<glm::vec2> mVertexTextureCoords;
        std::vector<GLint> mVertexIndices;
        
        int mNumParticles;
        int mNumParticlesInWidth, mNumParticlesInHeight;

        float mMass;
};

#endif