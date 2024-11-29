#pragma once

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "shape.h"
#include "abstract_modelwrap.h"
#include "camera.h"
#include <set>

// 前向声明 PhysicsSystem 类
class PhysicsSystem;

// 刚体
class RigidBody
{
protected:
	int id = 0;

	// 位置
	glm::vec3 position = glm::vec3(0.0f);
	// 速度
	glm::vec3 velocity = glm::vec3(0.0f);
	// 加速度
	glm::vec3 acceleration = glm::vec3(0.0f);

	PhysicsSystem* physicsSystem = nullptr;

	// 缩放
	float scale = 1.0f;

public:
	// 模型
	AbstractModelWrap* model = nullptr;

	// 所处的碰撞层
	std::set<int> collisionLayer;
	// 面向的碰撞层
	std::set<int> collisionLayerMask;

	// 碰撞箱
	Shape* collisionBox = nullptr;
	
	// 变换后的碰撞箱子
	Shape* transformedCollisionBox = nullptr;

	RigidBody()
	{
	}

	RigidBody(AbstractModelWrap* model, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration, std::set<int> collisionLayer, std::set<int> collisionLayerMask, Shape* collisionBox)
	{
		this->model = model;
		this->position = position;
		this->velocity = velocity;
		this->acceleration = acceleration;
		this->collisionLayer = collisionLayer;
		this->collisionLayerMask = collisionLayerMask;
		this->collisionBox = collisionBox;
	}

	RigidBody(AbstractModelWrap* model, glm::vec3 position, glm::vec3 velocity, glm::vec3 acceleration)
	{
		this->model = model;
		this->position = position;
		this->velocity = velocity;
		this->acceleration = acceleration;
	}

	RigidBody(AbstractModelWrap* model) {
		this->model = model;
		//this->position = model->getPosition();
		this->position = glm::vec3(0.0f);
		this->velocity = glm::vec3(0.0f);
		this->acceleration = glm::vec3(0.0f);
	}

	RigidBody(RigidBody& rigidBody)
	{
		this->id = rigidBody.id;
		this->model = rigidBody.model;
		this->position = rigidBody.position;
		this->velocity = rigidBody.velocity;
		this->acceleration = rigidBody.acceleration;
		this->collisionLayer = rigidBody.collisionLayer;
		this->collisionLayerMask = rigidBody.collisionLayerMask;
		this->collisionBox = rigidBody.collisionBox;
		this->transformedCollisionBox = rigidBody.transformedCollisionBox;
	}

	~RigidBody(){
		//delete transformedCollisionBox;
	}
	
	int getId()
	{
		return id;
	}

	void setId(int id)
	{
		this->id = id;
	}

	void setModel(AbstractModelWrap* model)
	{
		this->model = model;
	}

	void setPosition(glm::vec3 position)
	{
		this->position = position;
	}

	void setVelocity(glm::vec3 velocity)
	{
		this->velocity = velocity;
	}

	void setAcceleration(glm::vec3 acceleration)
	{
		this->acceleration = acceleration;
	}

	void setCollisionLayer(std::set<int> collisionLayer)
	{
		this->collisionLayer = collisionLayer;
	}

	void setCollisionLayerMask(std::set<int> collisionLayerMask)
	{
		this->collisionLayerMask = collisionLayerMask;
	}

	void addCollisionLayer(int collisionLayer)
	{
		this->collisionLayer.insert(collisionLayer);
	}

	void addCollisionLayerMask(int collisionLayerMask)
	{
		this->collisionLayerMask.insert(collisionLayerMask);
	}

	void setCollisionBox(Shape* collisionBox)
	{
		this->collisionBox = collisionBox;
		if (collisionBox != nullptr)
			this->transformedCollisionBox = collisionBox->clone();
	}

	void setPhysicsSystem(PhysicsSystem* physicsSystem)
	{
		this->physicsSystem = physicsSystem;
	}

    virtual void update(float deltaTime)
	{
		velocity += acceleration * deltaTime;
		position += velocity * deltaTime;
		updateTransformedCollisionBox();
	}

	virtual void draw(Camera camera)
	{
		model->setPosition(position);
		model->scale(this->scale);
		model->draw(camera);
	}
	
	// 碰撞后的处理，子类会重载这个方法
	virtual void onCollision(RigidBody* other)
	{
		// 默认情况下，碰撞后不做任何处理
	}

	void updateTransformedCollisionBox()
	{
		if (transformedCollisionBox != nullptr)
			transformedCollisionBox->transform(position);
	}

	Shape* getCollisionBox()
	{
		return collisionBox;
	}
};

#endif // RIGIDBODY_H