#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <GL/glew.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

#include <glm/gtx/transform.hpp>

#include "btBulletDynamicsCommon.h"

#include "DebugDraw.h"
#include "CharacterEditor3DProperties.h"

using std::string;
using std::vector;
using std::map;
using std::set;
using std::unique_ptr;

using namespace clm;

class CharacterEditor3DProperties;

class CharacterSkeleton {
private:
	struct Constraint {
		btTypedConstraint* constraint;
		string boneA, boneB;
	};

	class IKChain {
	private:
		void ikStep(const glm::vec3 &e);
		void updateChildTransforms(Bone3D* bone);

		vector<Bone3D*> ikChain;

		glm::vec3 endEffector;
		glm::mat4 meshTransform;
		glm::vec3 relativeTargetPosition;
		glm::vec3 targetPosition;
	public:
		IKChain();
		void addBone(Bone3D* bone);

		void setMeshTransform(glm::mat4 meshTransform);
		void setRelativeTargetPosition(glm::vec3 relativeTargetPosition);
		glm::vec3 getTargetPosition();
		glm::vec3 getEndEffector();

		void solveIK();
	};

	struct AnimationBlendComponent {
		string animationName;
		float animationStatus;
		float weight;
	};

	void updateBoneTransforms(Bone3D* bone, const glm::mat4& parentTransform);
	void setRigidBodyTransforms(Bone3D* bone);

	/*vector<MeshEntry> entries;
	vector<Texture*> textures;*/

	//glm::mat4 globalTransformInverse;

	glm::vec3 position;
	float rotY;
	glm::mat4 rootTransform;

	Bone3D* boneHierarchy;
	map<string, Bone3D*> bonesByName;
	map<string, Animation> animations;

	vector<AnimationBlendComponent> blendComponents;

	vector<IKChain> ikChains;
	glm::vec3 debugGround1, debugGround2, debugGroundOffset1, debugGroundOffset2;

	CharacterEditor3DProperties* characterEditor3DProperties;

	vector<Constraint> constraints;
	bool bRagdoll;

	//tmp
	glm::vec3 footLPos, footRPos;
public:
	explicit CharacterSkeleton(CharacterEditor3DProperties* characterEditor3DProperties);
	~CharacterSkeleton();

	bool loadSkeleton(const string& fileName);

	Bone3D* getBoneHierarchy();
	map<string, Bone3D*>* getBonesByName();

	void setPosition(glm::vec3 position);
	void setRotY(float rotY);
	void calculateRootTransform();

	void clearBlendComponents();
	unsigned int addBlendComponent(const string& animationName, float animationStatus, float weight);
	void setBlendComponentWeight(unsigned int blendComponentId, float weight);
	float getBlendComponentStatus(unsigned int blendComponentId);
	void setBlendComponentStatus(unsigned int blendComponentId, float animationStatus);

	void updateBones();
	void updateIKs();
	void boneTransform(vector<glm::mat4>& transforms);

	void renderBones(DebugDraw* debugDraw);
	void renderJoints(DebugDraw* debugDraw);

	//void addTestRigidBody(btDiscreteDynamicsWorld* dynamicsWorld, const string& boneName, float weightThreshold);
	void add6DofConstraint(btDiscreteDynamicsWorld* dynamicsWorld, const string& boneName1, const string& boneName2,
						   glm::vec3 lowerLimit, glm::vec3 upperLimit);
	void addConeTwistConstraint(btDiscreteDynamicsWorld* dynamicsWorld, const string& boneName1, const string& boneName2,
						   float span1 = 2 * M_PI, float span2 = 2 * M_PI, float twist = 2 * M_PI);
	void addHingeConstraint(btDiscreteDynamicsWorld* dynamicsWorld, const string& boneName1, const string& boneName2,
							float low, float high);
	void setRagdollMode(bool ragdollMode, btDiscreteDynamicsWorld* dynamicsWorld);
	btRigidBody* getChestRigidBody();

	void setFootLTarget(glm::vec3 footLTarget);
	void setFootRTarget(glm::vec3 footRTarget);
	unsigned int addIKChain(const string& boneName, unsigned int length);
	glm::vec3 getIKEndEffectorPosition(unsigned int ikId);
	void setIKTargetPosition(unsigned int ikId, glm::vec3 targetPosition);

	void setDebugPoints(glm::vec3 debugGround1, glm::vec3 debugGround2, glm::vec3 debugGroundOffset1, glm::vec3 debugGroundOffset2);
};
