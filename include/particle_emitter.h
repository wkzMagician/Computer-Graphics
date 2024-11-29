#pragma once

#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "particle.h"
#include "physics_system.h"
#include "shape.h"
// ����ָ��
#include <memory>

class ParticleEmitter
{
	PhysicsSystem* physicsSystem;

	Particle particle;

	// ������λ��
	glm::vec3 position;

	bool isEternal = false;

	// ��������������
	float life = -1.0f;

	// ����������������
	float particleLife = 10.0f;

	// �����������ٶ�
	float emitSpeed = 0.0f;

	// �����������ٶ������Χ
	float emitSpeedRange = 0.0f;

	// ���������䷽��
	glm::vec3 emitDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	// ������������ٶ�
	glm::vec3 emitAcceleration = glm::vec3(0.0f);

	// ������������ٶ������Χ
	glm::vec3 emitAccelerationRange = glm::vec3(0.0f);

	// ������������
	float emitInterval = 1.0f;

	// �����������������Χ
	float emitIntervalRange = 0.0f;

	// ���䷶Χ
	Shape* emitShape = nullptr;

	// �Ƿ�������䷽��
	bool randomEmitDirection = false;

	// ÿ�η��������
	int emitCount = 1;

public:

	ParticleEmitter(PhysicsSystem* physicsSystem, Particle particle, glm::vec3 position, float particleLife = 1.0f, float life = -1.0f) {
		this->physicsSystem = physicsSystem;
		this->particle = particle;
		this->position = position;
		this->life = life;
		this->isEternal = life < 0.0f;
		this->particleLife = particleLife;
	}

	ParticleEmitter(){

	}

	void setEmitSpeed(float emitSpeed) {
		this->emitSpeed = emitSpeed;
	}

	void setEmitSpeedRange(float emitSpeedRange) {
		this->emitSpeedRange = emitSpeedRange;
	}

	void setEmitDirection(glm::vec3 emitDirection) {
		this->emitDirection = emitDirection;
	}

	void setEmitAcceleration(glm::vec3 emitAcceleration) {
		this->emitAcceleration = emitAcceleration;
	}

	void setEmitInterval(float emitInterval) {
		this->emitInterval = emitInterval;
	}
	
	void setParticleLife(float particleLife) {
		this->particleLife = particleLife;
	}

	void setEmitShape(Shape* emitShape) {
		this->emitShape = emitShape;
	}

	void setRandomEmitDirection(bool randomEmitDirection) {
		this->randomEmitDirection = randomEmitDirection;
	}

	void setEmitCount(int emitCount) {
		this->emitCount = emitCount;
	}

	void initParticle(Particle* particle) {
		particle->setPosition(genEmitPosition());
		particle->setLife(particleLife);
		if (randomEmitDirection) {
			particle->setVelocity(getRandomEmitDirection() * (emitSpeed + emitSpeedRange * (rand() % 1000 / 1000.0f)));
		}
		else {
			particle->setVelocity(emitDirection * (emitSpeed + emitSpeedRange * (rand() % 1000 / 1000.0f)));
		}

		particle->setAcceleration(emitAcceleration + emitAccelerationRange * glm::vec3(rand() % 1000 / 1000.0f, rand() % 1000 / 1000.0f, rand() % 1000 / 1000.0f));
		particle->setCollisionLayer(this->particle.collisionLayer);
		particle->setCollisionLayerMask(this->particle.collisionLayerMask);
		particle->setCollisionBox(this->particle.getCollisionBox());

		particle->setDecay(this->particle.isDecay);
		particle->setDecayRate(this->particle.decayRate);
		particle->setFade(this->particle.isFade);
	}

	void emit(float deltaTime) {
		if (!isEternal) {
			life -= deltaTime;
			if (life <= 0.0f)
				delete this;
		}

		emitInterval -= deltaTime;
		if (emitInterval <= 0.0f) {

			for (int i = 0; i < emitCount; i++) {
				// ����ָ��
				std::shared_ptr<Particle> newParticle(new Particle(particle));
				initParticle(newParticle.get());

				physicsSystem->AddRigidBody(newParticle);
			}

			emitInterval = emitInterval + emitIntervalRange * (rand() % 1000 / 1000.0f);
		}
	}

	glm::vec3 genEmitPosition() {
		return emitShape->getRandomPoint() + position;
	}

	glm::vec3 getRandomEmitDirection() {
		// ����ļн� a,b
		float a = rand() % 1000 / 1000.0f * 2 * 3.1415926f;
		float b = rand() % 1000 / 1000.0f * 3.1415926f;

		// �������
		return glm::vec3(sin(b) * cos(a), sin(b) * sin(a), cos(b));
	}

	~ParticleEmitter() {

	}
};

#endif // !PARTICLE_EMITTER_H