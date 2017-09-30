#include "AngularSpring.hpp"
#include <iostream>
#include <atlas/utils/GUI.hpp>

AngularSpring::AngularSpring() :
    mUseRK4(true),
    mAngleAcceleration(0.0f)
{
}

AngularSpring::~AngularSpring()
{
}

Coil& AngularSpring::getCoil()
{
	return mCoil;
}

void AngularSpring::useRK4(bool use)
{
	mUseRK4 = use;
}

void AngularSpring::setName(std::string name)
{
	mName = name;
}

void AngularSpring::setRestAngle(float angle)
{
    mRestAngle = angle;
}

void AngularSpring::setAngle(float angle)
{
    mAngle = angle;
}

void AngularSpring::setAngleSpeed(float angleSpeed)
{
    mAngleSpeed = angleSpeed;
}

void AngularSpring::renderGeometry(atlas::math::Matrix4 const &projection, atlas::math::Matrix4 const &view)
{
	mMass.renderGeometry(projection, view);
	mCoil.renderGeometry(projection, view);
}

void AngularSpring::updateGeometry(atlas::core::Time<> const &t)
{	


	float newAngle;
	float newAngleSpeed;
	if(mUseRK4)
	{
		//Compute the new position by Runge-Kutta Order 4.
		//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
		float v1 = t.deltaTime * computeAngleSpeed(0);
		float v2 = t.deltaTime * computeAngleSpeed(0.5*t.deltaTime);
		float v3 = t.deltaTime * computeAngleSpeed(0.5*t.deltaTime);
		float v4 = t.deltaTime * computeAngleSpeed(t.deltaTime);
		newAngle = mAngle + (1.0f / 6.0f) * (v1 + 2.0f*v2 + 2.0f*v3 + v4);

		//Compute the new velocity by Runge-Kutta Order 4
		//based on https://www.intmath.com/differential-equations/12-runge-kutta-rk4-des.php
		float a1 = t.deltaTime * computeAngleAccelerationFromTime(0, mAngleSpeed);
		float a2 = t.deltaTime * computeAngleAccelerationFromTime(0.5*t.deltaTime, mAngleSpeed + 0.5f*a1);
		float a3 = t.deltaTime * computeAngleAccelerationFromTime(0.5*t.deltaTime, mAngleSpeed + 0.5f*a2);
		float a4 = t.deltaTime * computeAngleAccelerationFromTime(t.deltaTime, mAngleSpeed + a3);
		newAngleSpeed = mAngleSpeed + (1.0f / 6.0f) * (a1 + 2.0f*a2 + 2.0f*a3 + a4);

		//Update acceleration so that we use it to compute velocity
		//next the next time we execute Runge-Kutta
		mAngleAcceleration = (newAngleSpeed - mAngleSpeed) / t.deltaTime;
	}
	else
	{
		//Compute new position and velocity by Euler integration
		newAngle = mAngle + mAngleSpeed*t.deltaTime;
		newAngleSpeed = mAngleSpeed + computeAngleAcceleration(mAngle, mAngleSpeed)*t.deltaTime;
		
	}

	//Update the mass's velocity and position states
	mAngle = newAngle;;
    mAngleSpeed = newAngleSpeed;

	this->applyTransformations();
}

void AngularSpring::applyTransformations()
{
	//Resets the model matrices so that we can re-apply scaling and/or translations to them
	mCoil.setModel(glm::mat4(1.0f));
    mMass.setModel(glm::mat4(1.0f));

    //Sets the mass to position based on the coil length and angle
    float r = mCoil.getRestLength();
    mMass.setPosition(glm::vec3(r*cos(mAngle), r*sin(mAngle), 0.0f) + mCoil.getFixedPosition());
	
	//Compute the axis of the coil and the length of the coil
	glm::vec3 coilAxis = mCoil.getFixedPosition() - mMass.getPosition();
	float coilLength = glm::length(coilAxis);

	//Scale the coil to its correct length
	mCoil.transformGeometry(glm::scale(glm::mat4(1.0f), glm::vec3(1.0, coilLength/mCoil.getRestLength(), 1.0)));
	
	if(coilLength > 0)
	{
		//Move the mass down by the length of the coil
		mMass.transformGeometry(glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -coilLength, 0.0)));

		//Convert the coil axis to a unit vector
		coilAxis /= coilLength;

		//If coil is perfectly vertical
		glm::vec3 w = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), coilAxis);
		if(w == glm::vec3(0.0f, 0.0f, 0.0f))
		{
			//We may need to flip the entire spring
			if(mMass.getPosition().y > 0)
			{
				this->transformGeometry(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f)));				
			}
		}

		//Otherwise we rotate the entire spring so that it is algined with its correct axis
		else
		{
			float theta = acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), coilAxis));
			this->transformGeometry(glm::rotate(glm::mat4(1.0f), theta, w));
		}
	}

	//Move the spring to its correct location
	this->transformGeometry(glm::translate(glm::mat4(1.0f), mCoil.getFixedPosition()));
}

float AngularSpring::computeAngleSpeed(float deltaTime, float angleSpeed)
{
    return angleSpeed + mAngleAcceleration*deltaTime;    
}

float AngularSpring::computeAngleSpeed(float deltaTime)
{
	return computeAngleSpeed(deltaTime, mAngleSpeed);
}	


float AngularSpring::computeAngleAccelerationFromTime(float deltaTime, float angleSpeed)
{
	float angle = mAngle + angleSpeed*deltaTime;
	return computeAngleAcceleration(angle, angleSpeed);
}

float AngularSpring::computeAngleAcceleration(float angle, float angleSpeed)
{
    float forceGravity = -9.81f * mMass.getMass() * cos(angle);

	float forceSpring = -mCoil.getSpringConstant() * (angle - mRestAngle);
	float forceDampening = -mCoil.getDampeningConstant() * angleSpeed;
    
    float netForce = forceSpring + forceDampening + forceGravity;

    return netForce / mMass.getMass();
}

void AngularSpring::drawGui()
{
	float ks = mCoil.getSpringConstant();
	float kd = mCoil.getDampeningConstant();
	
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiSetCond_FirstUseEver);
	
	ImGui::Begin((mName + " Options").c_str());
	ImGui::SliderFloat("Spring Constant", &ks, 0.0f, 50.0f);
	ImGui::SliderFloat("Dampening Constant", &kd, 0.0f, 50.0f);
	ImGui::End();

	mCoil.setSpringConstant(ks);
	mCoil.setDampeningConstant(kd);
}

void AngularSpring::transformGeometry(atlas::math::Matrix4 const& t)
{
	mCoil.transformGeometry(t);
	mMass.transformGeometry(t);	
}
