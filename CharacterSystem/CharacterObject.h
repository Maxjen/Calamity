#pragma once

#include <memory>

#include "btBulletDynamicsCommon.h"

#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include "GameObject3D.h"
#include "Camera.h"
#include "CharacterController.h"
#include "CharacterMeshComponent.h"
#include "CharacterClothComponent.h"
#include "CharacterSkeleton.h"

using std::unique_ptr;

class CharacterSkeleton;
class CharacterEditor3DProperties;
class CharacterClothComponent;

class CharacterObject : public GameObject3D {
private:
	enum AnimationState {
		NONE,
		IDLE,
		IDLE_WALK,
		WALK_RUN,
		RUN,
	};

	unsigned int idleComponentId, walkComponentId, runComponentId;
	float idleAnimationStatus, walkRunAnimationStatus;
	unsigned int leftFootIK, rightFootIK;

	Camera* camera;
	CharacterMeshShader* characterMeshShader;
	MeshShader* meshShader;
	unique_ptr<CharacterSkeleton> characterSkeleton;

	btSoftRigidDynamicsWorld* dynamicsWorld;

	CharacterEditor3DProperties* characterEditor3DProperties;

	unique_ptr<CharacterController> characterController;

	map<string, CharacterMeshComponent> characterMeshComponents;
	map<string, CharacterClothComponent> clothComponents;

	//unique_ptr<btCollisionShape> collisionShape;
	//unique_ptr<btDefaultMotionState> motionState;
	//unique_ptr<btRigidBody> rigidBody;

	//btKinematicCharacterController* characterController;


	/*btPairCachingGhostObject* ghostObject;
	btKinematicCharacterController* character;*/


	DebugDraw* debugDraw;

	// 0: Idle, 1: Idle/Walk, 2: Walk/Run, 3: Run
	//int animationState;
	AnimationState animationState;


	// TODO: remove bLoaded
	bool bLoaded;
	bool bSelected;

	bool bShowBones;
	bool bShowConstraints;
public:
	explicit CharacterObject(Camera* camera, CharacterMeshShader* characterMeshShader, MeshShader* meshShader, btSoftRigidDynamicsWorld* dynamicsWorld, DebugDraw* debugDraw, CharacterEditor3DProperties* characterEditor3DProperties);
	bool loadSkeleton(const string& fileName);

	bool addMeshComponent(const string& fileName); //TODO: maybe remove bLoaded and fix for wrong path
	void removeMeshComponent(const string& fileName);
	bool meshComponentLoaded(const string& fileName);

	bool addClothComponent(const string& fileName);
	void removeClothComponent(const string& fileName);
	bool clothComponentLoaded(const string& fileName);

	void reloadClothComponents();

	Bone3D* getBoneHierarchy();

	int getObjectType();
	void setPosition(float posX, float posY, float posZ);
	void setShowBones(bool bShowBones);
	void setShowConstraints(bool bShowConstraints);
	void toggleSelection();

	CharacterController* getCharacterController();

	void update(float deltaTime);
	void render();

	void addRigidBody(const string& boneName, float weightThreshold);
	void createRagdoll();
	void setRagdollMode(bool ragdollMode);
	btRigidBody* getChestRigidBody();
};
