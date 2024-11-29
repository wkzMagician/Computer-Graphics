#pragma once

#ifndef PARTICLE_H
#define PARTICLE_H

#include "rigidbody.h"
#include "physics_system.h"

class Rigidbody;

class Particle : public RigidBody
{
protected:
	// ��������
	float life = 1.0f;

	// ԭʼ����
	float startLife = 2.0f;

public:
	// �Ƿ�˥����С
	bool isDecay = false;
	//˥������
	float decayRate = 1.0f;
	// �Ƿ���ɫ
	bool isFade = false;
	// ��ɫ����
	float mixRatio = 0.0f;

	Particle(AbstractModelWrap* model, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, std::set<int> collisionLayer, std::set<int> collisionLayerMask, Shape* collisionBox, float life) : RigidBody(model, position, velocity, acceleration, collisionLayer, collisionLayerMask, collisionBox)
	{
		this->life = life;
		this->startLife = life;
	}

	Particle(AbstractModelWrap* model, glm::vec3 position, glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f), float life = 1.0f) : RigidBody(model, position, velocity, acceleration)
	{
		this->life = life;
		this->startLife = life;
	}

	Particle(AbstractModelWrap* model, float life = 1.0f) : RigidBody(model)
	{
		this->life = life;
		this->startLife = life;
	}

	Particle() {

	}

	Particle(Particle& particle) : RigidBody(particle.model, particle.position, particle.velocity, particle.acceleration, particle.collisionLayer, particle.collisionLayerMask, particle.collisionBox)
	{
		this->life = particle.life;
		this->startLife = particle.startLife;
		this->isDecay = particle.isDecay;
		this->decayRate = particle.decayRate;
		this->isFade = particle.isFade;
		this->mixRatio = particle.mixRatio;
	}

	~Particle() {

	}

	void setLife(float life) {
		this->life = life;
		this->startLife = life;
	}

	void setFade(bool isFade) {
		this->isFade = isFade;
	}

	void setDecay(bool isDecay) {
		this->isDecay = isDecay;
	}

	void setDecayRate(float decayRate) {
		this->decayRate = decayRate;
	}


	virtual void update(float deltaTime) override {
		RigidBody::update(deltaTime);

		life -= deltaTime;
		if (life <= 0.0f)
			destroy();

		//// ������ɫ
		//// ���Բ�ֵ
		/*glm::vec4 tColor = startColor * (life / startLife) + finalcolor * (1.0f - life / startLife);*/
		//model->setShaderColor(tColor);

		if (isFade) {
			// ���Բ�ֵ
			float t = 1.0f - life / startLife;
			this->mixRatio = t;
		}

		// ��������
		// ָ��˥��
		if (isDecay) {
			// e^(-2*ln(ratio)/life * time)
			float time_passed = startLife - life;
			float x = 2.0f / startLife * time_passed * log(decayRate);
			float scale = exp(-x);
			//model->scale(decayRate);
			this->scale = scale;
		}
	}

	virtual void draw(Camera camera) override
	{
		model->setMixRatio(mixRatio);
		RigidBody::draw(camera);
	}

	virtual void onCollision(RigidBody* other) override {
		destroy();
	}
	
	void destroy() {
		// �ӳ������Ƴ�
		physicsSystem->RemoveRigidBody(id);
	}

};

#endif // !PARTICLE_H