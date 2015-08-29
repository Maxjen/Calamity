#include "CharacterObject.h"

CharacterObject::CharacterObject(Camera* camera, CharacterMeshShader* characterMeshShader, MeshShader* meshShader, btSoftRigidDynamicsWorld* dynamicsWorld, DebugDraw* debugDraw, CharacterEditor3DProperties* characterEditor3DProperties) :
	idleComponentId(-1),
	walkComponentId(-1),
	runComponentId(-1),
	idleAnimationStatus(0),
	walkRunAnimationStatus(0),
	leftFootIK(-1),
	rightFootIK(-1),
	camera(camera),
	characterMeshShader(characterMeshShader),
	meshShader(meshShader),
	characterSkeleton(),
	dynamicsWorld(dynamicsWorld),
	characterEditor3DProperties(characterEditor3DProperties),
	characterController(),
	characterMeshComponents(),
	debugDraw(debugDraw),

	animationState(NONE),
	bLoaded(false),
	bSelected(false),
	bShowBones(false),
	bShowConstraints(false)
{
	characterSkeleton.reset(new CharacterSkeleton(characterEditor3DProperties));
	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);

	characterController.reset(new CharacterController(dynamicsWorld, position));
}

bool CharacterObject::loadSkeleton(const string& fileName) {
	bool result = characterSkeleton->loadSkeleton(fileName);

	leftFootIK = characterSkeleton->addIKChain("foot.L", 3);
	rightFootIK = characterSkeleton->addIKChain("foot.R", 3);

	return result;
	/*if (!bLoaded)
		bLoaded = characterMesh->loadSkeleton(fileName);
	return bLoaded;*/
}

bool CharacterObject::addMeshComponent(const string& fileName) {
	/*aiMatrix3x3 rotA;
	aiMatrix3x3t<float>::Rotation(3.14159265f, aiVector3D(-1.0f, 0, 0), rotA);
	printf("Assimp:\n%f %f %f\n%f %f %f\n%f %f %f\n", rotA.a1, rotA.a2, rotA.a3, rotA.b1, rotA.b2, rotA.b3, rotA.c1, rotA.c2, rotA.c3);

	glm::mat3 rotG3 = glm::make_mat3(&rotA.a1);
	printf("Assimp to GLM:\n%f %f %f\n%f %f %f\n%f %f %f\n",
			rotG3[0][0], rotG3[0][1], rotG3[0][2],
			rotG3[1][0], rotG3[1][1], rotG3[1][2],
			rotG3[2][0], rotG3[2][1], rotG3[2][2]);

	glm::mat4 rotG4 = glm::mat4();
	rotG4[0][0] = rotG3[0][0];virtual int getObjectType();
	rotG4[0][1] = rotG3[0][1];
	rotG4[0][2] = rotG3[0][2];
	rotG4[1][0] = rotG3[1][0];
	rotG4[1][1] = rotG3[1][1];
	rotG4[1][2] = rotG3[1][2];
	rotG4[2][0] = rotG3[2][0];
	rotG4[2][1] = rotG3[2][1];
	rotG4[2][2] = rotG3[2][2];
	printf("Assimp to GLM:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
			rotG4[0][0], rotG4[0][1], rotG4[0][2], rotG4[0][3],
			rotG4[1][0], rotG4[1][1], rotG4[1][2], rotG4[1][3],
			rotG4[2][0], rotG4[2][1], rotG4[2][2], rotG4[2][3],
			rotG4[3][0], rotG4[3][1], rotG4[3][2], rotG4[3][3]);

	glm::mat4 rotG = glm::mat4();
	rotG = glm::rotate(glm::mat4(), 180.0f, glm::vec3(1.0f, 0, 0));
	printf("GLM:\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
			rotG[0][0], rotG[0][1], rotG[0][2], rotG[0][3],
			rotG[1][0], rotG[1][1], rotG[1][2], rotG[1][3],
			rotG[2][0], rotG[2][1], rotG[2][2], rotG[2][3],
			rotG[3][0], rotG[3][1], rotG[3][2], rotG[3][3]);*/

	/*if (!bLoaded)
		bLoaded = characterMesh->loadMesh(fileName);
	return bLoaded;*/




	/*bLoaded = characterMesh->loadMesh(fileName);
	return bLoaded;*/


	bLoaded = true;

	if (characterMeshComponents.find(fileName) == characterMeshComponents.end()) {
		bool result = characterMeshComponents[fileName].init(fileName, characterSkeleton->getBonesByName(), characterMeshShader);
		if (!result) {
			characterMeshComponents.erase(fileName);
			printf("Couldn't init mesh component %s\n", fileName.c_str());
		}
		return result;
	}
	else {
		printf("File %s already loaded\n", fileName.c_str());
		return false;
	}
}

void CharacterObject::removeMeshComponent(const string& fileName) {
	//characterMesh->removeMesh(fileName);

	if (characterMeshComponents.find(fileName) != characterMeshComponents.end())
		characterMeshComponents.erase(fileName);
	else
		printf("No mesh component %s to remove.\n", fileName.c_str());
}

bool CharacterObject::meshComponentLoaded(const string& fileName) {
	//return characterMesh->meshLoaded(fileName);

	return characterMeshComponents.find(fileName) != characterMeshComponents.end();
}

bool CharacterObject::addClothComponent(const string& fileName) {
	//return characterMesh->addClothComponent(fileName, (btSoftRigidDynamicsWorld*)dynamicsWorld);

	if (clothComponents.find(fileName) == clothComponents.end()) {
		bool result = clothComponents[fileName].init(fileName, characterSkeleton->getBonesByName(), meshShader, characterEditor3DProperties, dynamicsWorld);
		if (!result) {
			clothComponents.erase(fileName);
			printf("Couldn't init cloth component %s\n", fileName.c_str());
		}
		return result;
	}
	else {
		printf("File %s already loaded\n", fileName.c_str());
		return false;
	}
}

void CharacterObject::removeClothComponent(const string& fileName) {
	//characterMesh->removeClothComponent(fileName);

	if (clothComponents.find(fileName) != clothComponents.end())
		clothComponents.erase(fileName);
	else
		printf("No cloth component %s to remove.\n", fileName.c_str());
}

bool CharacterObject::clothComponentLoaded(const string& fileName) {
	//return characterMesh->clothComponentLoaded(fileName);

	return clothComponents.find(fileName) != clothComponents.end();
}

void CharacterObject::reloadClothComponents() {
	//characterMesh->reloadClothComponents((btSoftRigidDynamicsWorld*)dynamicsWorld);

	vector<string> fileNames;
	for (auto& it : clothComponents)
		fileNames.push_back(it.first);
	clothComponents.clear();
	for (auto& it : fileNames)
		addClothComponent(it);
}

Bone3D* CharacterObject::getBoneHierarchy() {
	return characterSkeleton->getBoneHierarchy();
}

int CharacterObject::getObjectType() {
	return CHARACTER_OBJECT;
}

void CharacterObject::setPosition(float posX, float posY, float posZ) {
	position = glm::vec3(posX, posY, posZ);

	/*btRigidBody* r;
	r->getMotionState()->setWorldTransform();*/

	/*btTransform transform;
	rigidBody->getMotionState()->getWorldTransform(transform);
	transform.setOrigin(btVector3(posX, posY, posZ));
	rigidBody->getMotionState()->setWorldTransform(transform);*/

	/*btTransform transform = rigidBody->getCenterOfMassTransform();
	transform.setOrigin(btVector3(posX, posY, posZ));
	rigidBody->setCenterOfMassTransform(transform);

	dynamicsWorld->stepSimulation(1 / 60.f, 10);*/
}

void CharacterObject::setShowBones(bool bShowBones) {
	this->bShowBones = bShowBones;
}

void CharacterObject::setShowConstraints(bool bShowConstraints) {
	this->bShowConstraints = bShowConstraints;
}

void CharacterObject::toggleSelection() {
	bSelected = !bSelected;
}

CharacterController* CharacterObject::getCharacterController() {
	return characterController.get();
}

void CharacterObject::update(float deltaTime) {
	characterController->update(camera);
	position = characterController->getPosition();

	characterSkeleton->setPosition(position - glm::vec3(0, 0.5f, 0));
	characterSkeleton->setRotY(characterController->getRotY());
	characterSkeleton->calculateRootTransform();

	float vel = glm::length(characterController->getHorizontalVelocity());

	float walkSpeed = 3.0f;
	float runSpeed = 10.0f;

	if (vel == 0 && animationState != IDLE) {
		walkRunAnimationStatus = 0;
		characterSkeleton->clearBlendComponents();
		idleComponentId = characterSkeleton->addBlendComponent("AnimStack::metarig|Idle", idleAnimationStatus, 1.0f);
		animationState = IDLE;
	}
	else if (vel > 0 && vel <= walkSpeed && animationState != IDLE_WALK) {
		characterSkeleton->clearBlendComponents();
		idleComponentId = characterSkeleton->addBlendComponent("AnimStack::metarig|Idle", idleAnimationStatus, 0.5f);
		walkComponentId = characterSkeleton->addBlendComponent("AnimStack::metarig|Walk", walkRunAnimationStatus, 0.5f);
		animationState = IDLE_WALK;
	}
	else if (vel > walkSpeed && vel <= runSpeed && animationState != WALK_RUN) {
		characterSkeleton->clearBlendComponents();
		walkComponentId = characterSkeleton->addBlendComponent("AnimStack::metarig|Walk", walkRunAnimationStatus, 0.5f);
		runComponentId = characterSkeleton->addBlendComponent("AnimStack::metarig|Run", walkRunAnimationStatus, 0.5f);
		animationState = WALK_RUN;
	}
	else if (vel > runSpeed && animationState != RUN) {
		characterSkeleton->clearBlendComponents();
		runComponentId = characterSkeleton->addBlendComponent("AnimStack::metarig|Run", walkRunAnimationStatus, 1.0f);
		animationState = RUN;
	}

	idleAnimationStatus = fmod(idleAnimationStatus + 0.5f * deltaTime, 1.0f);
	if (animationState == IDLE) {
		characterSkeleton->setBlendComponentStatus(idleComponentId, idleAnimationStatus);
	}
	else {
		walkRunAnimationStatus = fmod(walkRunAnimationStatus + 0.2f * vel * deltaTime, 1.0f);
		if (animationState == IDLE_WALK) {
			characterSkeleton->setBlendComponentWeight(idleComponentId, 1.0f - vel / walkSpeed);
			characterSkeleton->setBlendComponentWeight(walkComponentId, vel / walkSpeed);
			characterSkeleton->setBlendComponentStatus(idleComponentId, idleAnimationStatus);
			characterSkeleton->setBlendComponentStatus(walkComponentId, walkRunAnimationStatus);
		}
		else if (animationState == WALK_RUN) {
			characterSkeleton->setBlendComponentWeight(walkComponentId, 1.0f - (vel - walkSpeed) / (runSpeed - walkSpeed));
			characterSkeleton->setBlendComponentWeight(runComponentId, (vel - walkSpeed) / (runSpeed - walkSpeed));
			characterSkeleton->setBlendComponentStatus(walkComponentId, walkRunAnimationStatus);
			characterSkeleton->setBlendComponentStatus(runComponentId, walkRunAnimationStatus);
		}
		else if (animationState == RUN) {
			characterSkeleton->setBlendComponentStatus(runComponentId, walkRunAnimationStatus);
		}
	}

	characterSkeleton->updateBones();

	{
		glm::vec3 foot;
		btVector3 start, end;
		float offsetY;

		glm::vec3 debugGround1, debugGround2, debugGroundOffset1, debugGroundOffset2;

		foot = characterSkeleton->getIKEndEffectorPosition(leftFootIK);
		offsetY = foot.y - (position.y - 0.5f);
		start = btVector3(foot.x, foot.y + 0.5f, foot.z);
		end = start + btVector3(0, -1.0f, 0);
		{
			btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
			rayCallback.m_collisionFilterGroup = COL_WORLD;
			rayCallback.m_collisionFilterMask = COL_WORLD;
			dynamicsWorld->rayTest(start, end, rayCallback);
			if (rayCallback.hasHit()) {
				debugGround1 = glm::vec3(rayCallback.m_hitPointWorld.getX(), rayCallback.m_hitPointWorld.getY(), rayCallback.m_hitPointWorld.getZ());
				btVector3 footTarget = rayCallback.m_hitPointWorld + btVector3(0, 0.06f, 0);
				debugGroundOffset1 = glm::vec3(footTarget.getX(), footTarget.getY(), footTarget.getZ());
				//printf("leftStart %f %f %f\n", start.getX(), start.getY(), start.getZ());
				//printf("leftHit %f %f %f\n", footTarget.getX(), footTarget.getY(), footTarget.getZ());
				characterSkeleton->setIKTargetPosition(leftFootIK, glm::vec3(footTarget.getX(), footTarget.getY() + offsetY, footTarget.getZ()));
			}
			else
				characterSkeleton->setIKTargetPosition(leftFootIK, glm::vec3(foot.x, foot.y, foot.z));
		}

		foot = characterSkeleton->getIKEndEffectorPosition(rightFootIK);
		offsetY = foot.y - (position.y - 0.5f);
		start = btVector3(foot.x, foot.y + 0.5f, foot.z);
		end = start + btVector3(0, -1.0f, 0);
		{
			btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
			rayCallback.m_collisionFilterGroup = COL_WORLD;
			rayCallback.m_collisionFilterMask = COL_WORLD;
			dynamicsWorld->rayTest(start, end, rayCallback);
			if (rayCallback.hasHit()) {
				debugGround2 = glm::vec3(rayCallback.m_hitPointWorld.getX(), rayCallback.m_hitPointWorld.getY(), rayCallback.m_hitPointWorld.getZ());
				btVector3 footTarget = rayCallback.m_hitPointWorld + btVector3(0, 0.06f, 0);
				debugGroundOffset2 = glm::vec3(footTarget.getX(), footTarget.getY(), footTarget.getZ());
				//printf("rightStart %f %f %f\n", start.getX(), start.getY(), start.getZ());
				//printf("rightHit %f %f %f\n", footTarget.getX(), footTarget.getY(), footTarget.getZ());
				characterSkeleton->setIKTargetPosition(rightFootIK, glm::vec3(footTarget.getX(), footTarget.getY() + offsetY, footTarget.getZ()));
			}
			else
				characterSkeleton->setIKTargetPosition(rightFootIK, glm::vec3(foot.x, foot.y, foot.z));
		}
		characterSkeleton->setDebugPoints(debugGround1, debugGround2, debugGroundOffset1, debugGroundOffset2);
	}

	characterSkeleton->updateIKs();

	for (auto& it : clothComponents)
		it.second.update();

	vector<glm::mat4> transforms;
	characterSkeleton->boneTransform(transforms);
	//characterMesh->boneTransform(0, transforms);
	for (unsigned int i = 0; i < transforms.size(); ++i) {
		characterMeshShader->bind();
		characterMeshShader->setBoneTransform(i, transforms[i]);
		characterMeshShader->unbind();
	}
}

void CharacterObject::render() {
	if (bLoaded) {
		/*glm::mat4 modelMatrix = glm::translate(glm::mat4(), position);
		modelMatrix = glm::scale(modelMatrix, scale);*/
		glm::mat4 modelMatrix = glm::scale(glm::mat4(), scale);

		glm::mat4 modelViewMatrix = camera->getViewMatrix() * modelMatrix;

		glm::mat4 modelInverseTranspose = glm::inverse(modelMatrix);
		modelInverseTranspose = glm::transpose(modelInverseTranspose);

		characterMeshShader->bind();
		characterMeshShader->setModelViewMatrix(modelViewMatrix);
		characterMeshShader->setModelMatrix(modelMatrix);
		characterMeshShader->setModelInverseTranspose(modelInverseTranspose);
		characterMeshShader->unbind();

		meshShader->bind();
		meshShader->setModelViewMatrix(modelViewMatrix);
		meshShader->setModelMatrix(glm::mat4());
		meshShader->setModelInverseTranspose(glm::mat4());
		meshShader->unbind();


		if (bSelected) {
			characterMeshShader->bind();
			for (auto& it : characterMeshComponents)
				it.second.render();
			characterMeshShader->unbind();

			meshShader->bind();
			for (auto& it : clothComponents)
				it.second.render();
			meshShader->unbind();

			/*//glEnable(GL_STENCIL_TEST);
			glClearStencil(1);
			glClear(GL_STENCIL_BUFFER_BIT);

			// Render the mesh into the stencil buffer.

			glEnable(GL_STENCIL_TEST);

			glStencilFunc(GL_ALWAYS, 1, -1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			characterMesh->render();

			// Render the thick wireframe version.

			glStencilFunc(GL_NOTEQUAL, 1, -1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			glLineWidth(3);
			glPolygonMode(GL_FRONT, GL_LINE);

			characterMesh->renderBlack();

			glPolygonMode(GL_FRONT, GL_FILL);

			glDisable(GL_STENCIL_TEST);*/
		}
		else {
			characterMeshShader->bind();
			for (auto& it : characterMeshComponents)
				it.second.render();
			characterMeshShader->unbind();

			meshShader->bind();
			for (auto& it : clothComponents)
				it.second.render();
			meshShader->unbind();
		}

		if (bShowBones)
			characterSkeleton->renderBones(debugDraw);
		if (bShowConstraints)
			characterSkeleton->renderJoints(debugDraw);
	}
}

void CharacterObject::addRigidBody(const string& boneName, float weightThreshold) {
	map<string, Bone3D*>* bonesByName = characterSkeleton->getBonesByName();
	glm::vec3 min, max;
	bool bMinMaxInitialized = false;
	for (auto& it : characterMeshComponents) {
		glm::vec3 cMin, cMax;
		it.second.getBoneAabb(boneName, weightThreshold, cMin, cMax);
		if (bMinMaxInitialized) {
			min.x = std::min(min.x, cMin.x);
			min.y = std::min(min.y, cMin.y);
			min.z = std::min(min.z, cMin.z);

			max.x = std::max(max.x, cMax.x);
			max.y = std::max(max.y, cMax.y);
			max.z = std::max(max.z, cMax.z);
		}
		else {
			min.x = cMin.x;
			min.y = cMin.y;
			min.z = cMin.z;
			max.x = cMax.x;
			max.y = cMax.y;
			max.z = cMax.z;
			bMinMaxInitialized = true;
		}
	}
	//printf("min %f %f %f\nmax %f %f %f\n", min.x, min.y, min.z, max.x, max.y, max.z);
	float radius = std::max(max.x - min.x, max.z - min.z) / 2;
	float height = max.y - min.y;
	glm::vec3 offset = (max + min) * 0.5f;
	(*bonesByName)[boneName]->addRigidBody(dynamicsWorld, radius, height, offset);

	/*if (boneName.compare("spine") == 0)
		printf("%f\n", height);*/
}

void CharacterObject::createRagdoll() {
	/*characterMesh->addTestRigidBody(dynamicsWorld, "hips", 0.5);
	//characterMesh->addTestRigidBody(dynamicsWorld, "spine", 0.5);
	characterMesh->addTestRigidBody(dynamicsWorld, "chest", 0.3);
	characterMesh->addTestRigidBody(dynamicsWorld, "neck", 0.5);
	characterMesh->addTestRigidBody(dynamicsWorld, "head", 0.5);

	characterMesh->addTestRigidBody(dynamicsWorld, "upper_arm.L", 0.7);
	characterMesh->addTestRigidBody(dynamicsWorld, "forearm.L", 0.5);
	//characterMesh->addTestRigidBody(dynamicsWorld, "hand.L", 0.3);
	characterMesh->addTestRigidBody(dynamicsWorld, "thigh.L", 0.5);
	characterMesh->addTestRigidBody(dynamicsWorld, "shin.L", 0.5);
	characterMesh->addTestRigidBody(dynamicsWorld, "foot.L", 0.5);

    characterMesh->addTestRigidBody(dynamicsWorld, "upper_arm.R", 0.7);
    characterMesh->addTestRigidBody(dynamicsWorld, "forearm.R", 0.5);
	//characterMesh->addTestRigidBody(dynamicsWorld, "hand.R", 0.3);
	characterMesh->addTestRigidBody(dynamicsWorld, "thigh.R", 0.5);
    characterMesh->addTestRigidBody(dynamicsWorld, "shin.R", 0.5);
	characterMesh->addTestRigidBody(dynamicsWorld, "foot.R", 0.5);*/

	addRigidBody("hips", 0.5f);
	//addRigidBody("spine", 0.5f);
	addRigidBody("chest", 0.3f);
	addRigidBody("neck", 0.5f);
	addRigidBody("head", 0.5f);

	addRigidBody("upper_arm.L", 0.7f);
	addRigidBody("forearm.L", 0.5f);
	addRigidBody("hand.L", 0.3f);
	addRigidBody("thigh.L", 0.5f);
	addRigidBody("shin.L", 0.5f);
	addRigidBody("foot.L", 0.5f);

	addRigidBody("upper_arm.R", 0.7f);
	addRigidBody("forearm.R", 0.5f);
	addRigidBody("hand.R", 0.3f);
	addRigidBody("thigh.R", 0.5f);
	addRigidBody("shin.R", 0.5f);
	addRigidBody("foot.R", 0.5f);

	/*characterSkeleton->addConeTwistConstraint(dynamicsWorld, "hips", "spine", M_PI_4, M_PI_4, M_PI_4);
	characterSkeleton->addConeTwistConstraint(dynamicsWorld, "spine", "chest", M_PI_4, M_PI_4, M_PI_4);*/
	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "hips", "chest", M_PI_4, M_PI_4, M_PI_4);

	characterSkeleton->addHingeConstraint(dynamicsWorld, "hips", "chest", -M_PI_4, M_PI_4);
	/*characterSkeleton->addHingeConstraint(dynamicsWorld, "hips", "spine", -M_PI_4, M_PI_4);
	characterSkeleton->addHingeConstraint(dynamicsWorld, "spine", "chest", -M_PI_4, M_PI_4);*/
	characterSkeleton->addConeTwistConstraint(dynamicsWorld, "chest", "neck", M_PI_4, M_PI_4, 0);
	characterSkeleton->addConeTwistConstraint(dynamicsWorld, "neck", "head", M_PI_4, M_PI_4, M_PI_2);

	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "chest", "upper_arm.L", 0, 0, M_PI_2, M_PI_2, M_PI_2);
	characterSkeleton->add6DofConstraint(dynamicsWorld, "chest", "upper_arm.L", glm::vec3(-3 * M_PI_4, 0, -M_PI_4), glm::vec3(M_PI_4, 0, M_PI_4));
	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "upper_arm.L", "forearm.L", 2 * M_PI, 2 * M_PI, 0);
	characterSkeleton->add6DofConstraint(dynamicsWorld, "upper_arm.L", "forearm.L", glm::vec3(-3 * M_PI_4, 0, 0), glm::vec3(0, 0, 0));
	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "forearm.L", "hand.L", M_PI_2, M_PI_2, M_PI);
	characterSkeleton->addConeTwistConstraint(dynamicsWorld, "forearm.L", "hand.L", 0, 0, 0);

	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "hips", "thigh.L", 2 * M_PI, M_PI_2, 0);
	characterSkeleton->add6DofConstraint(dynamicsWorld, "hips", "thigh.L", glm::vec3(-M_PI_2, 0, 0), glm::vec3(M_PI_4, 0, 0));
	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "thigh.L", "shin.L", 2 * M_PI, 2 * M_PI, 0);
	characterSkeleton->add6DofConstraint(dynamicsWorld, "thigh.L", "shin.L", glm::vec3(-3 * M_PI_4, 0, 0), glm::vec3(0, 0, 0));
	//characterSkeleton->addHingeConstraint(dynamicsWorld, "thigh.L", "shin.L", 0, 3 * M_PI_4);
	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "shin.L", "foot.L", M_PI_2, M_PI_2, M_PI_4);
	characterSkeleton->addHingeConstraint(dynamicsWorld, "shin.L", "foot.L", -M_PI_4, M_PI_4);

	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "chest", "upper_arm.R", 0, 0, M_PI_2, M_PI_2, M_PI_2);
	characterSkeleton->add6DofConstraint(dynamicsWorld, "chest", "upper_arm.R", glm::vec3(-3 * M_PI_4, 0, -M_PI_4), glm::vec3(M_PI_4, 0, M_PI_4));
	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "upper_arm.R", "forearm.R", 2 * M_PI, 2 * M_PI, 0);
	characterSkeleton->add6DofConstraint(dynamicsWorld, "upper_arm.R", "forearm.R", glm::vec3(-3 * M_PI_4, 0, 0), glm::vec3(0, 0, 0));
	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "forearm.R", "hand.R", M_PI_2, M_PI_2, M_PI);
	characterSkeleton->addConeTwistConstraint(dynamicsWorld, "forearm.R", "hand.R", 0, 0, 0);

	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "hips", "thigh.R", 2 * M_PI, M_PI_2, 0);
	characterSkeleton->add6DofConstraint(dynamicsWorld, "hips", "thigh.R", glm::vec3(-M_PI_2, 0, 0), glm::vec3(M_PI_4, 0, 0));
	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "thigh.R", "shin.R", 2 * M_PI, 2 * M_PI, 0);
	characterSkeleton->add6DofConstraint(dynamicsWorld, "thigh.R", "shin.R", glm::vec3(-3 * M_PI_4, 0, 0), glm::vec3(0, 0, 0));
	//characterSkeleton->addHingeConstraint(dynamicsWorld, "thigh.R", "shin.R", 0, 3 * M_PI_4);
	//characterSkeleton->addConeTwistConstraint(dynamicsWorld, "shin.R", "foot.R", M_PI_2, M_PI_2, M_PI_4);
	characterSkeleton->addHingeConstraint(dynamicsWorld, "shin.R", "foot.R", -M_PI_4, M_PI_4);

	//characterMesh->addIKChain("hand.L", 3, glm::vec3(0.5f, 1.5f, 0));
}

void CharacterObject::setRagdollMode(bool ragdollMode) {
	/*for (auto& it : clothComponents)
		it.second.setMaxDistanceScale(1.0f);*/

	characterSkeleton->setRagdollMode(ragdollMode, dynamicsWorld);
}

btRigidBody* CharacterObject::getChestRigidBody() {
	return characterSkeleton->getChestRigidBody();
}
