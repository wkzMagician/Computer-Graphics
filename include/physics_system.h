#pragma once

#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "rigidbody.h"
#include <map>
#include <string>
#include <vector>

class RigidBody;

class ActionBuffer {
public:
	std::string action;
};

class AddRigidBodyAction : public ActionBuffer {
public:
	shared_ptr<RigidBody> rigidBody;
};

class RemoveRigidBodyAction : public ActionBuffer {
public:
	int id;
};

class PhysicsSystem {
	int idCounter = 0;

protected:
	// 刚体列表
	std::map<int, shared_ptr<RigidBody>> rigidBodies;

	// 碰撞层
	// collisionLayer[i] 表示第 i 层的碰撞层
	std::map<int, std::set<int>> collisionLayer;
	// 碰撞层掩码
	// collisionLayerMask[i] 表示第 i 层的碰撞层掩码
	std::map<int, std::set<int>> collisionLayerMask;

	// 碰撞总层数
	const int COLLISION_LAYER_COUNT = 8;

	// buffer
	std::vector<ActionBuffer*> actionBuffer;

public:
	PhysicsSystem() {

	}

	~PhysicsSystem() {

	}

	void update(float deltaTime) {
		for (auto it = rigidBodies.begin(); it != rigidBodies.end(); it++) {
			it->second->update(deltaTime);
		}
	}

	void checkCollision() {
		for (auto rb = rigidBodies.begin(); rb != rigidBodies.end(); rb++) {
			Shape* shape1 = rb->second->transformedCollisionBox;
			//Shape* shape1 = rb->second->collisionBox;
			Shape* shape2 = nullptr;

			//std::cout << "rb id: " << rb->second->getId() << std::endl;

			for (auto it = rb->second->collisionLayerMask.begin(); it != rb->second->collisionLayerMask.end(); it++) {
				//std::cout << "layer: " << *it << std::endl;
				for (int id : collisionLayer[*it]) {
					//std::cout << "id: " << id << std::endl;
					if (rb->second->getId() == id) {
						continue;
					}

					shared_ptr<RigidBody> rb2 = rigidBodies[id];
					Shape* shape2 = rb2->transformedCollisionBox;
					//Shape* shape2 = rb2->collisionBox;
					//std::cout << "rb2 id: " << rb2->getId() << std::endl;
					//std::cout << "shape1: " << static_cast<int>(shape1->type) << std::endl;
					//std::cout << "shape2: " << static_cast<int>(shape2->type) << std::endl;
					/*if (shape1->type == ShapeType::POINT) {
						Point* point = (Point*)shape1;
						std::cout << "point: " << point->position.x << " " << point->position.y << " " << point->position.z << std::endl;
					}*/
					if (shape1->isIntersect(shape2)) {
						//std::cout << "collision" << std::endl;
						rb->second->onCollision(rb2.get());
					}
				}
			}
		}
	}

	void draw(Camera camera) {
		for (auto it = rigidBodies.begin(); it != rigidBodies.end(); it++) {
			it->second->draw(camera);
		}
	}

	// 整合的更新函数
	// update + checkCollision + draw
	void updateAll(float deltaTime, Camera camera) {
		update(deltaTime);
		doBuffer();
		checkCollision();
		doBuffer();
		draw(camera);
	}

	void AddRigidBody(shared_ptr<RigidBody> rigidBody) {
		AddRigidBodyAction* action = new AddRigidBodyAction();
		action->action = "add";
		action->rigidBody = rigidBody;
		actionBuffer.push_back(action);
	}

	void RemoveRigidBody(int id) {
		RemoveRigidBodyAction* action = new RemoveRigidBodyAction();
		action->action = "remove";
		action->id = id;
		actionBuffer.push_back(action);
	}
	

	void doBuffer() {
		if (actionBuffer.size() == 0) {
			return;
		}

		for (ActionBuffer* action : actionBuffer) {
			if (action->action == "add") {
				AddRigidBodyAction* addRigidBodyAction = (AddRigidBodyAction*)action;
				addRigidBody(addRigidBodyAction->rigidBody);
			}
			else if (action->action == "remove") {
				RemoveRigidBodyAction* removeRigidBodyAction = (RemoveRigidBodyAction*)action;
				removeRigidBody(removeRigidBodyAction->id);
			}
		}

		// free
		for (ActionBuffer* action : actionBuffer) {
			delete action;
		}
		
		actionBuffer.clear();
	}

protected:
	void addRigidBody(shared_ptr<RigidBody> rigidBody) {
		int id = idCounter++;
		rigidBodies[id] = rigidBody;
		rigidBody->setPhysicsSystem(this);
		rigidBody->setId(id);

		for (int layer : rigidBody->collisionLayer) {
			collisionLayer[layer].insert(id);
		}

		for (int layer : rigidBody->collisionLayerMask) {
			collisionLayerMask[layer].insert(id);
		}
	}

	void removeRigidBody(int id) {
		for (auto it = collisionLayer.begin(); it != collisionLayer.end(); it++) {
			it->second.erase(id);
		}

		for (auto it = collisionLayerMask.begin(); it != collisionLayerMask.end(); it++) {
			it->second.erase(id);
		}

		rigidBodies[id] = nullptr;

		rigidBodies.erase(id);
	}
};

#endif // !PHYSICS_SYSTEM_H