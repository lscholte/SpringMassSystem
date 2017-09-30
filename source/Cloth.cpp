#include "Cloth.hpp"
#include "Shader.hpp"
#include "Asset.hpp"
#include <iostream>
#include <atlas/utils/GUI.hpp>
#include <stb/stb_image.h>

#include "Mass.hpp"


Cloth::Cloth() :
    mMass(0.01f)
{
    constexpr float width = 10;	
    constexpr float height = 10;
    constexpr float particlesPerUnitLength = 2;
    mNumParticlesInWidth = width * particlesPerUnitLength;
    mNumParticlesInHeight = height * particlesPerUnitLength;
    
    mNumParticles = mNumParticlesInWidth * mNumParticlesInHeight;
    // const float massPerParticle = mMass / mNumParticles;
    const float massPerParticle = 0.001;

    mVertexPositions = std::vector<glm::vec3>(mNumParticles);
    mVertexNormals = std::vector<glm::vec3>(mNumParticles);
    mVertexColors = std::vector<glm::vec3>(mNumParticles);
    mVertexTextureCoords = std::vector<glm::vec2>(mNumParticles);
    mVertexIndices = std::vector<GLint>((mNumParticlesInWidth-1)*(mNumParticlesInHeight-1)*6);

    mParticles = std::vector<std::unique_ptr<ClothParticle>>(mNumParticles);

    int currentParticle = 0;
    for(int i = 0; i < mNumParticlesInHeight; ++i)
    {
        for(int j = 0; j < mNumParticlesInWidth; ++j)
        {
            const int subscriptOffset = i*mNumParticlesInWidth + 3*j;    
            
            
            std::unique_ptr<ClothParticle> particle = std::make_unique<ClothParticle>();
            particle->setPosition(glm::vec3(j/particlesPerUnitLength, 0.0f, i/particlesPerUnitLength));
            particle->setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
            particle->setMass(massPerParticle);

            if(
                // (i == 0 && j == 0) ||
                (i == mNumParticlesInHeight-1 && j == 0) ||
                // (i == 0 && j == mNumParticlesInWidth-1) ||
                (i == mNumParticlesInHeight-1 && j == mNumParticlesInWidth-1)
            )
            {
                particle->setFixed(true);
            }

            particle->applyTransformations();

            mParticles[currentParticle] = std::move(particle);
            ++currentParticle;
        }
    }    

    for(int i = 0; i < mNumParticlesInHeight; ++i)
    {
        for(int j = 0; j < mNumParticlesInWidth; ++j)
        {
            const int current = i*mNumParticlesInWidth + j;
            ClothParticle *currentParticle = mParticles[current].get();

            const float structuralSpringConstant = 10.0f;
            const float structuralDampeningConstant = 0.003f;
            const float shearSpringConstant = 0.5f;
            const float shearDampeningConstant = 0.0f;
            const float bendingSpringConstant = 1.0f;
            const float bendingDampeningConstant = 0.0f;

            //Add Structural Springs
            const int left = current - 1;
            const int right = current + 1;
            const int top = current - mNumParticlesInHeight;
            const int bottom = current + mNumParticlesInHeight;
            const int topLeft = top - 1;
            const int topRight = top + 1;
            const int bottomLeft = bottom - 1;
            const int bottomRight = bottom + 1;
            const int rightRight = right + 1;
            const int bottomBottom = bottom + mNumParticlesInHeight;

            //Set the structural springs
            if(j < mNumParticlesInWidth-1)
            {
                ClothParticle *otherParticle = mParticles[right].get();
                std::unique_ptr<ClothCoil> coil = std::make_unique<ClothCoil>();
                coil->setRestLength(glm::length(currentParticle->getPosition() - otherParticle->getPosition()));
                coil->setEndParticles(currentParticle, otherParticle);                
                coil->setSpringConstant(structuralSpringConstant);
                coil->setDampeningConstant(structuralDampeningConstant);
                mStructuralCoils.push_back(std::move(coil));                
            }
            if(i < mNumParticlesInHeight-1)
            {
                ClothParticle *otherParticle = mParticles[bottom].get();
                std::unique_ptr<ClothCoil> coil = std::make_unique<ClothCoil>();
                coil->setRestLength(glm::length(currentParticle->getPosition() - otherParticle->getPosition()));
                coil->setEndParticles(currentParticle, otherParticle);                
                coil->setSpringConstant(structuralSpringConstant);
                coil->setDampeningConstant(structuralDampeningConstant);
                mStructuralCoils.push_back(std::move(coil));                
            }

            //Set the shear springs
            if(i > 0 && j < mNumParticlesInWidth-1)
            {
                ClothParticle *otherParticle = mParticles[topRight].get();
                std::unique_ptr<ClothCoil> coil = std::make_unique<ClothCoil>();
                coil->setRestLength(glm::length(currentParticle->getPosition() - otherParticle->getPosition()));
                coil->setEndParticles(currentParticle, otherParticle);                
                coil->setSpringConstant(shearSpringConstant);
                coil->setDampeningConstant(shearDampeningConstant);
                mShearCoils.push_back(std::move(coil));                
            }
            if(i < mNumParticlesInHeight-1 && j < mNumParticlesInWidth-1)
            {
                ClothParticle *otherParticle = mParticles[bottomRight].get();
                std::unique_ptr<ClothCoil> coil = std::make_unique<ClothCoil>();
                coil->setRestLength(glm::length(currentParticle->getPosition() - otherParticle->getPosition()));
                coil->setEndParticles(currentParticle, otherParticle);                
                coil->setSpringConstant(shearSpringConstant);
                coil->setDampeningConstant(shearDampeningConstant);
                mShearCoils.push_back(std::move(coil));                
            }


            //Add Bending Springs
            if(j < mNumParticlesInWidth-2)
            {
                ClothParticle *otherParticle = mParticles[rightRight].get();
                std::unique_ptr<ClothCoil> coil = std::make_unique<ClothCoil>();
                coil->setRestLength(glm::length(currentParticle->getPosition() - otherParticle->getPosition()));
                coil->setEndParticles(currentParticle, otherParticle);                
                coil->setSpringConstant(bendingSpringConstant);
                coil->setDampeningConstant(bendingDampeningConstant);
                mBendCoils.push_back(std::move(coil));                
            }
            if(i < mNumParticlesInHeight-2)
            {
                ClothParticle *otherParticle = mParticles[bottomBottom].get();
                std::unique_ptr<ClothCoil> coil = std::make_unique<ClothCoil>();
                coil->setRestLength(glm::length(currentParticle->getPosition() - otherParticle->getPosition()));
                coil->setEndParticles(currentParticle, otherParticle);                
                coil->setSpringConstant(bendingSpringConstant);
                coil->setDampeningConstant(bendingDampeningConstant);
                mBendCoils.push_back(std::move(coil));                
            }       
        }
    }

    int k = 0;
    for(int i = 0; i < mNumParticlesInHeight - 1; ++i)
    {
        for(int j = 0; j < mNumParticlesInWidth; ++j)
        {
            mVertexIndices[k] = i * mNumParticlesInWidth + j;
            mVertexIndices[k+1] = (i+1) * mNumParticlesInWidth + j;
            k += 2;
        }
    }

    glGenVertexArrays(1, &mVao);
    glGenBuffers(1, &mPositionBuffer);
    glGenBuffers(1, &mNormalBuffer);        
    glGenBuffers(1, &mColorBuffer);
    glGenBuffers(1, &mIndexBuffer);
    glGenBuffers(1, &mTextureBuffer);
    
    // glBindVertexArray(mVao);
    
    // glBindVertexArray(0);

    int imageWidth, imageHeight, imageComp;    
    stbi_set_flip_vertically_on_load(1);    
    stbi_uc* image = stbi_load((generated::Asset::getAssetDirectory() + "/texture.jpg").c_str(), &imageWidth, &imageHeight, &imageComp, 3);
    stbi_set_flip_vertically_on_load(0);
    
    glGenTextures(1, &mTextureId);
    glActiveTexture(GL_TEXTURE0);    
    glBindTexture(GL_TEXTURE_2D, mTextureId);
    if(imageComp == 3)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);        
    }
    else if(imageComp == 4)
    {   
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);        
    }
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);        

    stbi_image_free(image);


	std::vector<atlas::gl::ShaderUnit> shaderUnits
	{
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.vert", GL_VERTEX_SHADER),
		atlas::gl::ShaderUnit(generated::Shader::getShaderDirectory() + "/scene.frag", GL_FRAGMENT_SHADER)
	};
	
	mShaders.push_back(atlas::gl::Shader(shaderUnits));
	
	mShaders[0].compileShaders();
    mShaders[0].linkShaders();

    // for(auto &coil : mCoils)
    // {
    //     coil->setSpringConstant(1.0f);
    // }

}

Cloth::~Cloth()
{
}


void Cloth::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{	
    for(int i = 0; i < mNumParticlesInHeight; ++i)
    {
        for(int j = 0; j < mNumParticlesInWidth; ++j)
        {
            const int current = i*mNumParticlesInWidth + j;
            mVertexPositions[current] = mParticles[current]->getPosition();
            //TODO: Normal
                //         glm::vec3 normal = glm::normalize(glm::cross(mVertexPositions[index+2] -  mVertexPositions[index], mVertexPositions[index+1] - mVertexPositions[index]));
    //         mVertexNormals[index] = normal;
    //         mVertexNormals[index+1] = normal;
    //         mVertexNormals[index+2] = normal;

            mVertexTextureCoords[current] = glm::vec2((float) j/(mNumParticlesInWidth-1), (float) i/(mNumParticlesInHeight-1));   
        }
    }

    int index = 0;
    for(int i = 0; i < mNumParticlesInHeight-1; ++i)
    {
        for(int j = 0; j < mNumParticlesInWidth-1; ++j)
        {
            const int current = i*mNumParticlesInWidth + j;                        
            mVertexIndices[index] = current;
            mVertexIndices[index+1] = current + mNumParticlesInWidth;
            mVertexIndices[index+2] = current + mNumParticlesInWidth + 1;

            mVertexIndices[index+3] = current;
            mVertexIndices[index+4] = current + mNumParticlesInWidth + 1;
            mVertexIndices[index+5] = current + 1;

            index += 6;
        }
    }

    glBindVertexArray(mVao);
    
    glBindBuffer(GL_ARRAY_BUFFER, mPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, 3*mVertexPositions.size()*sizeof(GLfloat), &mVertexPositions[0].x, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
    // glBufferData(GL_ARRAY_BUFFER, 3*mVertexNormals.size()*sizeof(GLfloat), &mVertexNormals[0].x, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    // glBufferData(GL_ARRAY_BUFFER, 3*mVertexColors.size()*sizeof(GLfloat), &mVertexColors[0].x, GL_STATIC_DRAW);
    // glEnableVertexAttribArray(2);
    // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, mTextureBuffer);
    glBufferData(GL_ARRAY_BUFFER, 2*mVertexTextureCoords.size()*sizeof(GLfloat), &mVertexTextureCoords[0].x, GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

    glBindVertexArray(0);

	mShaders[0].enableShaders();
	
	glm::mat4 modelViewProjection = projection * view * mModel;

    const GLint MODEL_VIEW_PROJECTION_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "ModelViewProjection");
    const GLint USE_DIFFUSE_MAP_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "UseDiffuseMap");        
    const GLint DIFFUSE_MAP_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "DiffuseMap");    
    // const GLint MODEL_UNIFORM_LOCATION = glGetUniformLocation(mShaders[0].getShaderProgram(), "Model");
	
	glUniformMatrix4fv(MODEL_VIEW_PROJECTION_UNIFORM_LOCATION, 1, GL_FALSE, &modelViewProjection[0][0]);
    // glUniformMatrix4fv(MODEL_UNIFORM_LOCATION, 1, GL_FALSE, &mModel[0][0]);
    
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(DIFFUSE_MAP_UNIFORM_LOCATION, 0);
    glUniform1i(USE_DIFFUSE_MAP_UNIFORM_LOCATION, true);    
    glBindTexture(GL_TEXTURE_2D, mTextureId);    
    

	// //If I don't do this, my faces will be inside out
	// //because I specified the cube vertices in clockwise order
    glFrontFace(GL_CCW);

	glBindVertexArray(mVao);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mVertexIndices.size()*sizeof(GLint), &mVertexIndices[0], GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, mVertexIndices.size(), GL_UNSIGNED_INT, (void *) 0);

	glBindVertexArray(0);


    mShaders[0].disableShaders();




    // for(auto &coil : mStructuralCoils)
    // {
    //     coil->applyTransformations();
    //     coil->renderGeometry(projection, view);
    // }
    // for(auto &coil : mShearCoils)
    // {
    //     coil->applyTransformations();
    //     coil->renderGeometry(projection, view);
    // }
    // for(auto &coil : mBendCoils)
    // {
    //     coil->applyTransformations();
    //     coil->renderGeometry(projection, view);
    // }
}

void Cloth::updateGeometry(atlas::core::Time<> const &t)
{
    //For each coil in the cloth, update its geometry
    //which consists of computing the forces acting
    //on the cloth particles attached to both ends
    //of the coil

    atlas::core::Time<> time = t;
    time.deltaTime = t.deltaTime / 15.0;

    for(int i = 0; i < 15; ++i)
    {
        for(auto &coil : mStructuralCoils)
        { 
            coil->updateGeometry(time);
        }
        for(auto &coil : mShearCoils)
        { 
            coil->updateGeometry(time);
        }
        for(auto &coil : mBendCoils)
        { 
            coil->updateGeometry(time);
        }

        //Now update the positions and velocities
        //of the cloth particles
        for(auto &particle : mParticles)
        {
            particle->updateGeometry(time);
        }

        for(int i = 0; i < 8; ++i)
        {
            // std::random_shuffle(mCoils.begin(), mCoils.end());
            for(auto &coil : mStructuralCoils)
            { 
                coil->checkStretch();
            }
            for(auto &coil : mShearCoils)
            { 
                coil->checkStretch();
            }
        }
    }
}

void Cloth::drawGui()
{
	// // float ks = mCoils[0]->getSpringConstant();
    // float kd = mParticles[0]->getDampeningConstant();
	
	// ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_FirstUseEver);
	
	// ImGui::Begin("Cloth Options");
	// // ImGui::SliderFloat("Spring Constant", &ks, 0.0f, 50.0f);
	// ImGui::SliderFloat("Dampening Constant", &kd, 0.0f, 50.0f);
	// ImGui::End();

    // // for(auto &coil : mCoils)
    // // {
    // //     coil->setSpringConstant(ks);
    // // }

    // for(auto &particle : mParticles)
    // {
    //     particle->setDampeningConstant(kd);
    // }
}