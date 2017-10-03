#ifndef ClothParticle_hpp
#define ClothParticle_hpp

#include <atlas/utils/Geometry.hpp>
#include <set>
#include <vector>
#include "Mass.hpp"

class ClothParticle;

class ClothCoil : atlas::utils::Geometry
{
    public:
        ClothCoil();	
        ~ClothCoil();

        void setSpringConstant(float springConstant);
        float getSpringConstant() const;

        void setDampeningConstant(float dampeningConstant);
        float getDampeningConstant() const;

        void setEndParticles(std::pair<ClothParticle *, ClothParticle *> const &endParticles);
        void setEndParticles(ClothParticle *endA, ClothParticle *endB);        
        std::pair<ClothParticle *, ClothParticle *> getEndParticles() const;

        void setRestLength(float restLength);		
        float getRestLength() const;

        void updateGeometry(atlas::core::Time<> const &t) override;    
        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;  
        
        void setModel(glm::mat4 const &model);		
        
        void applyTransformations(); 
        
        void checkStretch();
        
    private:
        float mSpringConstant;
		float mDampeningConstant;
		float mRestLength;
        std::pair<ClothParticle *, ClothParticle *> mEndParticles;

        GLuint mVao;
        GLuint mPositionBuffer, mColorBuffer, mIndexBuffer;
        
        static const GLfloat POSITIONS[][3];
		static const GLfloat COLORS[][3];
		static const GLint INDICES[][3];
};

class ClothParticle : public atlas::utils::Geometry
{
    public:

        ClothParticle();

        void computeGeometry(atlas::core::Time<> const &t);

        void updateGeometry(atlas::core::Time<> const &t) override;
        void renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view) override;       
        
        void applyTransformations();        
        
        void setMass(float mass);
        float getMass() const;

        void setPosition(glm::vec3 const &position);
        glm::vec3 getPosition() const;

        void setVelocity(glm::vec3 const &velocity);
		glm::vec3 getVelocity() const;

		void setAcceleration(glm::vec3 const &acceleration);
        glm::vec3 getAcceleration() const;
        
        void setDampeningConstant(float dampeningConstant);
        float getDampeningConstant() const;

        void setFixed(bool fixed);
        bool isFixed();

        void addForce(glm::vec3 const &force);

        void setModel(glm::mat4 const &model);	
        
    private:
        std::set<ClothParticle *> mAttachedTo;

        std::vector<std::shared_ptr<ClothCoil>> mCoils;

        float mMass;
        glm::vec3 mPosition, mVelocity, mAcceleration;
        glm::vec3 mTempPosition;

        glm::vec3 computeVelocity(float deltaTime, glm::vec3 const &velocity);
        glm::vec3 computeVelocity(float deltaTime);        
		glm::vec3 computeAcceleration(float deltaTime, glm::vec3 const &velocity);		
        glm::vec3 computeAcceleration(glm::vec3 const &position, glm::vec3 const &velocity);
        
        bool mPositionFixed;

        glm::vec3 mNetForce;
        glm::vec3 mNormalForce;

        float mDampeningConstant;

        GLuint mVao;
        GLuint mPositionBuffer, mColorBuffer, mIndexBuffer;
        
        static const GLfloat POSITIONS[][3];
		static const GLfloat COLORS[][3];
        static const GLint INDICES[][1];        
};

#endif