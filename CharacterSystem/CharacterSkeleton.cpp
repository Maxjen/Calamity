#include "CharacterSkeleton.h"

CharacterSkeleton::IKChain::IKChain() :
	ikChain(),
	endEffector(),
	meshTransform(),
	relativeTargetPosition(glm::vec3(0, 0, 0)),
	targetPosition(relativeTargetPosition)
{}

void CharacterSkeleton::IKChain::addBone(Bone3D* bone) {
	ikChain.push_back(bone);
}

void CharacterSkeleton::IKChain::setMeshTransform(glm::mat4 meshTransform) {
	this->meshTransform = meshTransform;
	targetPosition = glm::vec3(meshTransform * glm::vec4(relativeTargetPosition, 1.0f));
}

void CharacterSkeleton::IKChain::setRelativeTargetPosition(glm::vec3 relativeTargetPosition) {
	this->relativeTargetPosition = relativeTargetPosition;
}

glm::vec3 CharacterSkeleton::IKChain::getTargetPosition() {
	return targetPosition;
}

glm::vec3 CharacterSkeleton::IKChain::getEndEffector() {
	endEffector = glm::vec3(ikChain.back()->globalTransformation * glm::vec4(0, 0, 0, 1));
	return endEffector;
}

void CharacterSkeleton::IKChain::solveIK() {
	if (ikChain.size() > 0) {
		endEffector = glm::vec3(ikChain.back()->globalTransformation * glm::vec4(0, 0, 0, 1));
		glm::vec3 e = targetPosition - endEffector;
		unsigned int stepCount = 0;
		while (glm::length(e) > 0.001f && stepCount < 30) {
			ikStep(e);
			endEffector = glm::vec3(ikChain.back()->globalTransformation * glm::vec4(0, 0, 0, 1));

			if (endEffector != endEffector)
				printf("%u %f %f %f\n", stepCount, endEffector.x, endEffector.y, endEffector.z);

			e = targetPosition - endEffector;
			++stepCount;
		}
	}

	for (unsigned int i = 0; i < ikChain.size(); ++i) {
		ikChain[i]->finalTransformation = ikChain[i]->globalTransformation * ikChain[i]->boneOffset;
		Bone3D* nextInChain = (i < ikChain.size() - 1) ? ikChain[i + 1] : nullptr;

		for (auto& it : ikChain[i]->children) {
			if (it != nextInChain)
				updateChildTransforms(it);
		}
	}
}

void CharacterSkeleton::IKChain::ikStep(const glm::vec3& e) {
	vector<glm::vec3> J;
	for (unsigned int i = 0; i < ikChain.size(); ++i) {
		glm::vec3 jointPos(ikChain[i]->globalTransformation * glm::vec4(0, 0, 0, 1));
		glm::vec3 j;
		j = glm::vec3(ikChain[i]->globalTransformation * glm::vec4(1, 0, 0, 0));
		j = glm::cross(j, endEffector - jointPos);
		J.push_back(j);
		j = glm::vec3(ikChain[i]->globalTransformation * glm::vec4(0, 0, 1, 0));
		j = glm::cross(j, endEffector - jointPos);
		J.push_back(j);
	}

	// TODO: change to mat3
	vector<glm::vec3> J_JT;
	for (unsigned int i = 0; i < 3; ++i) {
		glm::vec3 rowI;
		for (unsigned int j = 0; j < 3; ++j) {
			float c = 0;
			for (unsigned int k = 0; k < J.size(); ++k)
				c += J[k][i] * J[k][j];
			rowI[j] = c;
		}
		J_JT.push_back(rowI);
	}

	glm::vec3 J_JT_e;
	for (unsigned int i = 0; i < 3; ++i)
		J_JT_e[i] = glm::dot(J_JT[i], e);

	float alpha = glm::dot(e, J_JT_e) / glm::dot(J_JT_e, J_JT_e);

	unsigned int cRowJT = 0;
	for (unsigned int i = 0; i < ikChain.size(); ++i) {
		float xRot = alpha * glm::dot(J[cRowJT], e);
		++cRowJT;
		float zRot = alpha * glm::dot(J[cRowJT], e);
		++cRowJT;
		glm::quat rotDelta(glm::vec3(xRot, 0, zRot));

		ikChain[i]->rotation = ikChain[i]->rotation * rotDelta;

		glm::mat4 scalingM = glm::scale(glm::mat4(), ikChain[i]->scaling);
		glm::mat4 rotationM = glm::mat4_cast(ikChain[i]->rotation);
		glm::mat4 translationM = glm::translate(glm::mat4(), ikChain[i]->translation);

		glm::mat4 nodeTransformation = translationM * rotationM * scalingM;
		//ikChain[i]->nodeTransformation = nodeTransformation;
		glm::mat4 parentTransform;
		if (ikChain[i]->parent)
			parentTransform = ikChain[i]->parent->globalTransformation;
		ikChain[i]->globalTransformation = parentTransform * nodeTransformation;
	}

	/*IKLink* currentLink = linkHierarchy;
	vector<glm::vec3> J;
	while (currentLink) {
		glm::vec3 jointPos(currentLink->globalTransformation * glm::vec4(0, 0, 0, 1));

		//glm::vec3 j = glm::cross(glm::vec3(0, 0, 1), endEffector - jointPos);
		//J.push_back(j);

		glm::vec3 j;
		j = glm::vec3(currentLink->globalTransformation * glm::vec4(1, 0, 0, 0));
		j = glm::cross(j, endEffector - jointPos);
		J.push_back(j);
		j = glm::vec3(currentLink->globalTransformation * glm::vec4(0, 0, 1, 0));
		j = glm::cross(j, endEffector - jointPos);
		J.push_back(j);

		currentLink = currentLink->child;
	}

	// TODO: change to mat3
	vector<glm::vec3> J_JT;
	for (unsigned int i = 0; i < 3; ++i) {
		glm::vec3 rowI;
		for (unsigned int j = 0; j < 3; ++j) {
			float c = 0;
			for (unsigned int k = 0; k < J.size(); ++k)
				c += J[k][i] * J[k][j];
			rowI[j] = c;
		}
		J_JT.push_back(rowI);
	}

	glm::vec3 e = targetPosition - endEffector;

	glm::vec3 J_JT_e;
	for (unsigned int i = 0; i < 3; ++i)
		J_JT_e[i] = glm::dot(J_JT[i], e);

	float alpha = glm::dot(e, J_JT_e) / glm::dot(J_JT_e, J_JT_e);

	currentLink = linkHierarchy;
	unsigned int cRowJT = 0;
	while (currentLink) {
		//currentLink->angle += alpha * glm::dot(J[cRowJT], e);
		//++cRowJT;

		float xRot = alpha * glm::dot(J[cRowJT], e);
		++cRowJT;
		float zRot = alpha * glm::dot(J[cRowJT], e);
		++cRowJT;
		glm::quat rotDelta(glm::vec3(xRot, 0, zRot));
		currentLink->rotation = currentLink->rotation * rotDelta;

		currentLink = currentLink->child;
	}*/
}

void CharacterSkeleton::IKChain::updateChildTransforms(Bone3D* bone) {
	bone->globalTransformation = bone->parent->globalTransformation * bone->nodeTransformation;
	bone->finalTransformation = bone->globalTransformation * bone->boneOffset;

	for (auto& it : bone->children)
		updateChildTransforms(it);
}

CharacterSkeleton::CharacterSkeleton(CharacterEditor3DProperties* characterEditor3DProperties) :
	//globalTransformInverse(),
	position(0, 0, 0),
	rotY(0),
	rootTransform(),
	boneHierarchy(nullptr),
	bonesByName(),
	animations(),
	blendComponents(),
	characterEditor3DProperties(characterEditor3DProperties),
	bRagdoll(false)
{}

CharacterSkeleton::~CharacterSkeleton() {
	for (auto& it : constraints) {
		if (it.constraint)
			delete it.constraint;
	}
	if (boneHierarchy)
		delete boneHierarchy;
}

bool CharacterSkeleton::loadSkeleton(const std::string& fileName) {
	Mesh skeletonMesh;
	if (skeletonMesh.loadSkeleton(fileName)) {
		boneHierarchy = skeletonMesh.extractBoneHierarchy();
		bonesByName = skeletonMesh.getBonesByName();
		animations = skeletonMesh.getAnimations();

		/*map<string, Animation>::iterator it;
		printf("numAnimations: %u\n", animations.size());
		for (it = animations.begin(); it != animations.end(); ++it)
			printf("ANIMATIONNAME: %s\n", it->first.c_str());*/
		return true;
	}
	else
		return false;
	//clear();

	//Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	/*scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (scene) {
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
				string boneName(mesh->mBones[i]->mName.data);

				if (bonesByName.find(boneName) == bonesByName.end()) {
					Bone3D* b = new Bone3D();
					b->hasOffsetMatrix = true;
					aiMatrix4x4 offsetMatrixTmp = mesh->mBones[i]->mOffsetMatrix;
					b->boneOffset = glm::transpose(glm::make_mat4(&offsetMatrixTmp.a1));
					bonesByName[boneName] = b;
				}
			}
		}

		aiNode* rootBone = getRootBone(scene->mRootNode);
		boneHierarchy = createBoneHierarchy(rootBone, nullptr);

		unsigned int i = 0;
		for (auto& it: bonesByName) {
			it.second->boneId = i;
			++i;
		}

		return true;
	}
	else {
		printf("Error parsing '%s': '%s'\n", fileName.c_str(), importer.GetErrorString());
		return false;
	}*/
}

Bone3D* CharacterSkeleton::getBoneHierarchy() {
	return boneHierarchy;
}

map<string, Bone3D*>* CharacterSkeleton::getBonesByName() {
	return &bonesByName;
}

void CharacterSkeleton::setPosition(glm::vec3 position) {
	this->position = position;
}

void CharacterSkeleton::setRotY(float rotY) {
	this->rotY = rotY;
}

void CharacterSkeleton::calculateRootTransform() {
	rootTransform = glm::translate(glm::mat4(), position - glm::vec3(0, 0.05f, 0));
	rootTransform = glm::rotate(rootTransform, rotY, glm::vec3(0, 1, 0));
	rootTransform = glm::rotate(rootTransform, -90.0f, glm::vec3(1, 0, 0));
}

void CharacterSkeleton::clearBlendComponents() {
	blendComponents.clear();
}

unsigned int CharacterSkeleton::addBlendComponent(const std::string& animationName, float animationStatus, float weight) {
	if (animations.find(animationName) != animations.end()) {
		AnimationBlendComponent newBlendComponent;
		newBlendComponent.animationName = animationName;
		newBlendComponent.animationStatus = animationStatus;
		newBlendComponent.weight = weight;
		blendComponents.push_back(newBlendComponent);
		return blendComponents.size() - 1;
	}
	else {
		printf("Can't add blend component for animation %s.\n", animationName.c_str());
		return -1;
	}
}

void CharacterSkeleton::setBlendComponentWeight(unsigned int blendComponentId, float weight) {
	if (blendComponentId < blendComponents.size())
		blendComponents[blendComponentId].weight = weight;
	else
		printf("No blend component with id %u.\n", blendComponentId);
}

float CharacterSkeleton::getBlendComponentStatus(unsigned int blendComponentId) {
	if (blendComponentId < blendComponents.size())
		return blendComponents[blendComponentId].animationStatus;
	else {
		printf("No blend component with id %u.\n", blendComponentId);
		return 0;
	}
}

void CharacterSkeleton::setBlendComponentStatus(unsigned int blendComponentId, float animationStatus) {
	if (blendComponentId < blendComponents.size())
		blendComponents[blendComponentId].animationStatus = fmod(animationStatus, 1.0f);
	else
		printf("No blend component with id %u.\n", blendComponentId);
}

void CharacterSkeleton::updateBones() {
	if (animations.size() > 0) {
		/*Animation& anim = animations.begin()->second;
		float timeInTicks = timeInSeconds * anim.getTicksPerSecond();
		float animationTime = fmod(timeInTicks, anim.getDuration());*/

		updateBoneTransforms(boneHierarchy, rootTransform);
	}
}

void CharacterSkeleton::updateIKs() {
	if (!bRagdoll) {
		for (auto& it : ikChains) {
			it.setMeshTransform(rootTransform);
			it.solveIK();
		}
		setRigidBodyTransforms(boneHierarchy);
	}
}

void CharacterSkeleton::boneTransform(vector<glm::mat4>& transforms) {
	/*for (auto& it : clothComponents)
		it.second.update();*/

	transforms.resize(bonesByName.size());
	unsigned int i = 0;
	for (auto& it : bonesByName) {
		transforms[i] = it.second->finalTransformation;
		++i;
	}

	/*if (animations.size() > 0) {
		transforms.resize(bonesByName.size());
		unsigned int i = 0;
		for (auto& it : bonesByName) {
			transforms[i] = it.second->finalTransformation;
			++i;
		}
	}
	else {
		transforms.resize(bonesByName.size());
		for (auto& it : transforms)
			it = glm::mat4();
	}*/

	/*if (scene->HasAnimations()) {
		float ticksPerSecond = (float)(scene->mAnimations[0]->mTicksPerSecond != 0 ? scene->mAnimations[0]->mTicksPerSecond : 25.0f);
		float timeInTicks = timeInSeconds * ticksPerSecond;
		float animationTime = fmod(timeInTicks, (float)scene->mAnimations[0]->mDuration);

		readNodeHierarchy(animationTime, scene->mRootNode, glm::mat4());

		transforms.resize(bonesByName.size());
		unsigned int i = 0;
		for (auto& it : bonesByName) {
			transforms[i] = it.second->finalTransformation;
			++i;
		}
	}
	else {
		transforms.resize(bonesByName.size());
		for (auto& it : transforms)
			it = glm::mat4();
	}*/
}

void CharacterSkeleton::renderBones(DebugDraw* debugDraw) {
	for (auto& it: bonesByName) {
		if (it.second->parent) {
			glm::vec4 start(0, 0, 0, 1);
			glm::vec4 end(0, 0, 0, 1);

			start = it.second->parent->globalTransformation * start;
			end = it.second->globalTransformation * end;
			//end = glm::vec4(0, 0, 0, 0);

			debugDraw->drawLine(btVector3(start.x, start.y, start.z), btVector3(end.x, end.y, end.z), btVector3(1, 1, 1));
		}
	}

	for (unsigned int i = 0; i < ikChains.size(); ++i) {
		glm::vec3 targetPosition = ikChains[i].getTargetPosition();
		debugDraw->drawPoint(targetPosition.x, targetPosition.y, targetPosition.z, 255, 0, 0, 255);
	}
	debugDraw->drawPoint(debugGround1.x, debugGround1.y, debugGround1.z, 255, 255, 0, 255);
	debugDraw->drawPoint(debugGroundOffset1.x, debugGroundOffset1.y, debugGroundOffset1.z, 0, 255, 0, 255);
	debugDraw->drawPoint(debugGround2.x, debugGround2.y, debugGround2.z, 255, 255, 0, 255);
	debugDraw->drawPoint(debugGroundOffset2.x, debugGroundOffset2.y, debugGroundOffset2.z, 0, 255, 0, 255);

	/*glm::vec4 v1(0, 0, 0, 1);
	v1 = glm::inverse(bonesByName["upper_arm.R"]->globalTransformation) * bonesByName["forearm.R"]->globalTransformation * v1;
	glm::vec3 v2(v1.x, v1.y, v1.z);
	v2 = v2 - bonesByName["upper_arm.R"]->rigidBodyOffset;

	//c.pivotInA = btVector3(v2.x, v2.y, v2.z);
	printf("pivotInAasdf %f %f %f\n", v1.x, v1.y, v1.z);

	debugDraw->drawLine(btVector3(0, 0, 0), btVector3(v1.x, v1.y, v1.z), btVector3(1, 1, 0));*/
}

void CharacterSkeleton::renderJoints(DebugDraw* debugDraw) {
	/*btTransform transformA;
	glm::mat4 a = glm::inverse(bonesByName[it.boneA]->globalTransformation) * bonesByName[it.boneB]->globalTransformation;
	a = glm::translate(a, -bonesByName[it.boneA]->rigidBodyOffset);
	//a = glm::rotate(a, (float)90, glm::vec3(0, 0, 1));
	transformA.setFromOpenGLMatrix(glm::value_ptr(a));

	btTransform transformB;
	glm::mat4 b = glm::translate(glm::mat4(), -bonesByName[it.boneB]->rigidBodyOffset);
	//b = glm::rotate(b, (float)90, glm::vec3(0, 0, 1));
	transformB.setFromOpenGLMatrix(glm::value_ptr(b));*/

	for (auto& it : constraints) {
		{
			/*glm::vec4 jointOffset4(0, 0, 0, 1);
			jointOffset4 = glm::inverse(bonesByName[it.boneA]->globalTransformation) * bonesByName[it.boneB]->globalTransformation * jointOffset4;
			glm::vec3 jointOffset(jointOffset4.x, jointOffset4.y, jointOffset4.z);
			jointOffset -= bonesByName[it.boneA]->rigidBodyOffset;*/

			/*glm::vec4 origin(0, 0, 0, 1);
			glm::vec4 xAxis(0.1f, 0, 0, 1);
			glm::vec4 yAxis(0, 0.1f, 0, 1);
			glm::vec4 zAxis(0, 0, 0.1f, 1);*/

			//btTransform rigidBodyTransform = bonesByName[it.boneA]->rigidBody->getCenterOfMassTransform();
			//glm::mat4 jointTransform;
			//rigidBodyTransform.getOpenGLMatrix(glm::value_ptr(jointTransform));
			//jointTransform = glm::translate(jointTransform, jointOffset);
			//jointTransform = glm::rotate(jointTransform, (float)-90, glm::vec3(0, 0, 1));

			/*btTransform rigidBodyTransform = bonesByName[it.boneA]->rigidBody->getCenterOfMassTransform();
			glm::mat4 jointTransform;
			rigidBodyTransform.getOpenGLMatrix(glm::value_ptr(jointTransform));
			jointTransform = glm::translate(jointTransform, -bonesByName[it.boneA]->rigidBodyOffset);
			jointTransform = jointTransform * glm::inverse(bonesByName[it.boneA]->globalTransformation) * bonesByName[it.boneB]->globalTransformation;
			jointTransform = glm::rotate(jointTransform, (float)90, glm::vec3(0, 0, 1));

			origin = jointTransform * origin;
			xAxis = jointTransform * xAxis;
			yAxis = jointTransform * yAxis;
			zAxis = jointTransform * zAxis;*/

			/*glm::mat4 jointTransform;
			frameB.getOpenGLMatrix(glm::value_ptr(jointTransform));
			origin = jointTransform * origin;
			xAxis = jointTransform * xAxis;
			yAxis = jointTransform * yAxis;
			zAxis = jointTransform * zAxis;*/

			/*debugDraw->drawLine(btVector3(origin.x, origin.y, origin.z), btVector3(xAxis.x, xAxis.y, xAxis.z), btVector3(1, 0, 0));
			debugDraw->drawLine(btVector3(origin.x, origin.y, origin.z), btVector3(yAxis.x, yAxis.y, yAxis.z), btVector3(0, 1, 0));
			debugDraw->drawLine(btVector3(origin.x, origin.y, origin.z), btVector3(zAxis.x, zAxis.y, zAxis.z), btVector3(0, 0, 1));*/

			if (it.constraint->getConstraintType() == D6_CONSTRAINT_TYPE) {
				btGeneric6DofConstraint* d6 = (btGeneric6DofConstraint*)it.constraint;
				d6->calculateTransforms();
				btTransform d6Transform = d6->getCalculatedTransformB();
				btVector3 pivot = d6Transform.getOrigin();
				btMatrix3x3 basis = d6Transform.getBasis();
				btVector3 x = pivot + basis.getColumn(0) * 0.1f;
				btVector3 y = pivot + basis.getColumn(1) * 0.1f;
				btVector3 z = pivot + basis.getColumn(2) * 0.1f;

				debugDraw->drawLine(pivot, x, btVector3(1, 0, 0));
				debugDraw->drawLine(pivot, y, btVector3(0, 1, 0));
				debugDraw->drawLine(pivot, z, btVector3(0, 0, 1));

			}
			else if (it.constraint->getConstraintType() == HINGE_CONSTRAINT_TYPE) {
				btHingeConstraint* hinge = (btHingeConstraint*)it.constraint;

				btTransform hingeTransform = hinge->getRigidBodyB().getCenterOfMassTransform() * hinge->getBFrame();

				btVector3 pivot = hingeTransform * btVector3(0, 0, 0);
				btVector3 x = hingeTransform * btVector3(0.1f, 0, 0);
				btVector3 y = hingeTransform * btVector3(0, 0.1f, 0);
				btVector3 z = hingeTransform * btVector3(0, 0, 0.1f);

				debugDraw->drawLine(pivot, x, btVector3(0.3f, 0, 0));
				debugDraw->drawLine(pivot, y, btVector3(0, 0.3f, 0));
				debugDraw->drawLine(pivot, z, btVector3(1, 1, 0));
			}
			else if (it.constraint->getConstraintType() == CONETWIST_CONSTRAINT_TYPE) {
				btConeTwistConstraint* coneTwist = (btConeTwistConstraint*)it.constraint;

				btTransform coneTwistTransform = coneTwist->getRigidBodyB().getCenterOfMassTransform() * coneTwist->getBFrame();

				btVector3 pivot = coneTwistTransform * btVector3(0, 0, 0);
				btVector3 x = coneTwistTransform * btVector3(0.1f, 0, 0);
				btVector3 y = coneTwistTransform * btVector3(0, 0.1f, 0);
				btVector3 z = coneTwistTransform * btVector3(0, 0, 0.1f);

				debugDraw->drawLine(pivot, x, btVector3(0, 1, 1));
				debugDraw->drawLine(pivot, y, btVector3(0, 0.3f, 0));
				debugDraw->drawLine(pivot, z, btVector3(0, 0, 0.3f));
			}
		}

		/*{
			glm::vec4 origin(0, 0, 0, 1);
			glm::vec4 xAxis(0.1f, 0, 0, 1);
			glm::vec4 yAxis(0, 0.1f, 0, 1);
			glm::vec4 zAxis(0, 0, 0.1f, 1);

			btTransform rigidBodyTransform = bonesByName[it.boneB]->rigidBody->getCenterOfMassTransform();
			glm::mat4 jointTransform;
			rigidBodyTransform.getOpenGLMatrix(glm::value_ptr(jointTransform));
			jointTransform = glm::translate(jointTransform, -bonesByName[it.boneB]->rigidBodyOffset);
			jointTransform = glm::rotate(jointTransform, (float)90, glm::vec3(0, 0, 1));

			origin = jointTransform * origin;
			xAxis = jointTransform * xAxis;
			yAxis = jointTransform * yAxis;
			zAxis = jointTransform * zAxis;

			debugDraw->drawLine(btVector3(origin.x, origin.y, origin.z), btVector3(xAxis.x, xAxis.y, xAxis.z), btVector3(1, 0, 0));
			debugDraw->drawLine(btVector3(origin.x, origin.y, origin.z), btVector3(yAxis.x, yAxis.y, yAxis.z), btVector3(0, 1, 0));
			debugDraw->drawLine(btVector3(origin.x, origin.y, origin.z), btVector3(zAxis.x, zAxis.y, zAxis.z), btVector3(0, 0, 1));
		}*/
	}
}

void CharacterSkeleton::updateBoneTransforms(Bone3D* bone, const glm::mat4& parentTransform) {
	string boneName = bone->boneName;

	/*if (bRagdoll) {
		nodeTransformation = bone->nodeTransformation;
	}*/
	//else {
		if (!bRagdoll && !blendComponents.empty()) {
			// assuming all bones have animations
			// TODO: fix for leaf bones that are added by blender with fbx-export "Add Leaf Bones"-option

			Animation& firstAnimation = animations[blendComponents[0].animationName];
			glm::vec3 scaling(0, 0, 0);
			glm::quat rotation = firstAnimation.interpolateRotation(blendComponents[0].animationStatus * firstAnimation.getDuration(), boneName);
			glm::vec3 translation(0, 0, 0);
			float weightAcc = blendComponents[0].weight;

			for (unsigned int i = 0; i < blendComponents.size(); ++i) {
				Animation& anim = animations[blendComponents[i].animationName];
				float cAnimationTime = blendComponents[i].animationStatus * anim.getDuration();

				scaling += anim.interpolateScaling(cAnimationTime, boneName) * blendComponents[i].weight;

				if (i != 0) {
					glm::quat cRotation = anim.interpolateRotation(cAnimationTime, boneName);
					rotation = glm::slerp(rotation, cRotation, 1.0f - weightAcc / (weightAcc + blendComponents[i].weight));
					weightAcc += blendComponents[i].weight;
				}

				translation += anim.interpolatePosition(cAnimationTime, boneName) * blendComponents[i].weight;
			}
			bone->scaling = scaling;
			glm::mat4 scalingM = glm::scale(glm::mat4(), bone->scaling);

			bone->rotation = rotation;
			glm::mat4 rotationM = glm::mat4_cast(bone->rotation);

			bone->translation = translation;
			glm::mat4 translationM = glm::translate(glm::mat4(), bone->translation);

			bone->nodeTransformation = translationM * rotationM * scalingM;
		}
		/*else {
			// TODO: remove else-case
			float animationTime = 0;

			Animation& anim = animations.begin()->second;
			//Animation& anim = animations["AnimStack::metarig|Run"];
			if (anim.hasBoneAnimation(boneName)) {
				bone->scaling = anim.interpolateScaling(animationTime, boneName);
				glm::mat4 scalingM = glm::scale(glm::mat4(), bone->scaling);

				//aiQuaternion rotation;
				//calcInterpolatedRotation(rotation, animationTime, nodeAnim);
				//aiMatrix3x3 rotationMTmp = (rotation.GetMatrix());
				//glm::mat3 rotationTmp = glm::transpose(glm::make_mat3(&rotationMTmp.a1));
				//glm::mat4 rotationM = glm::mat4();
				//rotationM[0][0] = rotationTmp[0][0];
				//rotationM[0][1] = rotationTmp[0][1];
				//rotationM[0][2] = rotationTmp[0][2];
				//rotationM[1][0] = rotationTmp[1][0];
				//rotationM[1][1] = rotationTmp[1][1];
				//rotationM[1][2] = rotationTmp[1][2];
				//rotationM[2][0] = rotationTmp[2][0];
				//rotationM[2][1] = rotationTmp[2][1];
				//rotationM[2][2] = rotationTmp[2][2];
				bone->rotation = anim.interpolateRotation(animationTime, boneName);
				glm::mat4 rotationM = glm::mat4_cast(bone->rotation);

				bone->translation = anim.interpolatePosition(animationTime, boneName);
				glm::mat4 translationM = glm::translate(glm::mat4(), bone->translation);

				nodeTransformation = translationM * rotationM * scalingM;
			}
			else
				nodeTransformation = bone->nodeTransformation;
		}

		bone->nodeTransformation = nodeTransformation;
	}*/

	//glm::mat4 globalTransformation = parentTransform * bone->nodeTransformation;

	if (bRagdoll && bone->bHasRigidBody) {
		btTransform transform = bone->rigidBody->getCenterOfMassTransform();
		glm::mat4 rigidBodyTransform;
		transform.getOpenGLMatrix(glm::value_ptr(rigidBodyTransform));
		bone->globalTransformation = glm::translate(rigidBodyTransform, -bone->rigidBodyOffset);
	}
	else
		bone->globalTransformation = parentTransform * bone->nodeTransformation;

	bone->finalTransformation = bone->globalTransformation * bone->boneOffset;

	for (auto& it : bone->children)
		updateBoneTransforms(it, bone->globalTransformation);
}

void CharacterSkeleton::setRigidBodyTransforms(Bone3D* bone) {
	if (bone->bHasRigidBody) {
		glm::mat4 rigidBodyTransformation = bone->globalTransformation;
		rigidBodyTransformation = glm::translate(rigidBodyTransformation, bone->rigidBodyOffset);
		btTransform transform;
		transform.setFromOpenGLMatrix(glm::value_ptr(rigidBodyTransformation));
		bone->rigidBody->setCenterOfMassTransform(transform);

		//ATTRIBUTE_ALIGNED16(glm::value_ptr(bla));
		//glm::mat4 ATTRIBUTE_ALIGNED16(bla = bonesByName[nodeName]->finalTransformation);
		/*transform.setIdentity();
		transform.getOpenGLMatrix(glm::value_ptr(bla));*/
	}

	for (auto& it : bone->children)
		setRigidBodyTransforms(it);
}

/*void CharacterSkeleton::addTestRigidBody(btDiscreteDynamicsWorld* dynamicsWorld, const string& boneName, float weightThreshold) {
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
	bonesByName[boneName]->addRigidBody(dynamicsWorld, radius, height, offset);
}*/

void CharacterSkeleton::add6DofConstraint(btDiscreteDynamicsWorld* dynamicsWorld, const string& boneName1, const string& boneName2,
									  glm::vec3 lowerLimit, glm::vec3 upperLimit) {
	if (boneName1 != boneName2 && bonesByName.find(boneName1) != bonesByName.end() && bonesByName.find(boneName2) != bonesByName.end() &&
			bonesByName[boneName1]->bHasRigidBody && bonesByName[boneName2]->bHasRigidBody) {
		Constraint c;
		c.constraint = nullptr;
		c.boneA = boneName1;
		c.boneB = boneName2;

		glm::mat4 tmp;

		btTransform transformA = bonesByName[boneName1]->rigidBody->getCenterOfMassTransform();
		transformA.getOpenGLMatrix(glm::value_ptr(tmp));
		tmp = glm::inverse(tmp) * bonesByName[boneName2]->globalTransformation;
		transformA.setFromOpenGLMatrix(glm::value_ptr(tmp));

		btTransform transformB = bonesByName[boneName2]->rigidBody->getCenterOfMassTransform();
		transformB.getOpenGLMatrix(glm::value_ptr(tmp));
		tmp = glm::inverse(tmp) * bonesByName[boneName2]->globalTransformation;
		transformB.setFromOpenGLMatrix(glm::value_ptr(tmp));

		/*btTransform transformA;
		glm::mat4 a = glm::translate(glm::mat4(), -bonesByName[boneName1]->rigidBodyOffset) * glm::inverse(bonesByName[boneName1]->globalTransformation) * bonesByName[boneName2]->globalTransformation;
		//a = glm::rotate(a, (float)90, glm::vec3(0, 0, 1));
		transformA.setFromOpenGLMatrix(glm::value_ptr(a));

		btTransform transformB;
		glm::mat4 b = glm::translate(glm::mat4(), -bonesByName[boneName2]->rigidBodyOffset);
		//b = glm::rotate(b, (float)90, glm::vec3(0, 0, 1));
		transformB.setFromOpenGLMatrix(glm::value_ptr(b));*/


		//c.constraint = new btConeTwistConstraint(*bonesByName[boneName1]->rigidBody, *bonesByName[boneName2]->rigidBody, transformA, transformB);
		btGeneric6DofConstraint* d6 = new btGeneric6DofConstraint(*bonesByName[boneName1]->rigidBody, *bonesByName[boneName2]->rigidBody, transformA, transformB, true);
		//c.constraint->setLimit(span1, span2, twist);
		d6->setAngularLowerLimit(btVector3(lowerLimit.x, lowerLimit.y, lowerLimit.z));
		d6->setAngularUpperLimit(btVector3(upperLimit.x, upperLimit.y, upperLimit.z));
		d6->setEnabled(false);
		c.constraint = d6;

		dynamicsWorld->addConstraint(c.constraint, true);

		constraints.push_back(c);
	}
}

void CharacterSkeleton::addConeTwistConstraint(btDiscreteDynamicsWorld* dynamicsWorld, const string& boneName1, const string& boneName2,
									  float span1, float span2, float twist) {
	if (boneName1 != boneName2 && bonesByName.find(boneName1) != bonesByName.end() && bonesByName.find(boneName2) != bonesByName.end() &&
			bonesByName[boneName1]->bHasRigidBody && bonesByName[boneName2]->bHasRigidBody) {
		Constraint c;
		c.constraint = nullptr;
		c.boneA = boneName1;
		c.boneB = boneName2;

		glm::mat4 tmp;

		btTransform transformA = bonesByName[boneName1]->rigidBody->getCenterOfMassTransform();
		transformA.getOpenGLMatrix(glm::value_ptr(tmp));
		tmp = glm::inverse(tmp) * bonesByName[boneName2]->globalTransformation;
		tmp = glm::rotate(tmp, (float)90, glm::vec3(0, 0, 1));
		transformA.setFromOpenGLMatrix(glm::value_ptr(tmp));

		btTransform transformB = bonesByName[boneName2]->rigidBody->getCenterOfMassTransform();
		transformB.getOpenGLMatrix(glm::value_ptr(tmp));
		tmp = glm::inverse(tmp) * bonesByName[boneName2]->globalTransformation;
		tmp = glm::rotate(tmp, (float)90, glm::vec3(0, 0, 1));
		transformB.setFromOpenGLMatrix(glm::value_ptr(tmp));

		/*btTransform transformA;
		glm::mat4 a = glm::translate(glm::mat4(), -bonesByName[boneName1]->rigidBodyOffset) * glm::inverse(bonesByName[boneName1]->globalTransformation) * bonesByName[boneName2]->globalTransformation;
		a = glm::rotate(a, (float)90, glm::vec3(0, 0, 1));
		transformA.setFromOpenGLMatrix(glm::value_ptr(a));

		btTransform transformB;
		glm::mat4 b = glm::translate(glm::mat4(), -bonesByName[boneName2]->rigidBodyOffset);
		b = glm::rotate(b, (float)90, glm::vec3(0, 0, 1));
		transformB.setFromOpenGLMatrix(glm::value_ptr(b));*/


		btConeTwistConstraint* coneTwist = new btConeTwistConstraint(*bonesByName[boneName1]->rigidBody, *bonesByName[boneName2]->rigidBody, transformA, transformB);
		coneTwist->setLimit(span1, span2, twist);
		coneTwist->setEnabled(false);
		c.constraint = coneTwist;

		dynamicsWorld->addConstraint(c.constraint, true);

		//c.constraint->setEnabled(true);


		/*glm::vec4 v1(0, 0, 0, 1);
		v1 = glm::inverse(bonesByName[boneName1]->globalTransformation) * bonesByName[boneName2]->globalTransformation * v1;
		glm::vec3 v2(v1.x, v1.y, v1.z);
		//v2 -= bonesByName[boneName1]->rigidBodyOffset;
		c.pivotInA = btVector3(v2.x, v2.y, v2.z);
		printf("pivotInA %f %f %f\n", v2.x, v2.y, v2.z);*/

		//c.pivotInB = btVector3(-bonesByName[boneName2]->rigidBodyOffset.x, -bonesByName[boneName2]->rigidBodyOffset.y, -bonesByName[boneName2]->rigidBodyOffset.z);
		constraints.push_back(c);




	}
}

void CharacterSkeleton::addHingeConstraint(btDiscreteDynamicsWorld* dynamicsWorld, const string& boneName1, const string& boneName2,
									  float low, float high) {
	if (boneName1 != boneName2 && bonesByName.find(boneName1) != bonesByName.end() && bonesByName.find(boneName2) != bonesByName.end() &&
			bonesByName[boneName1]->bHasRigidBody && bonesByName[boneName2]->bHasRigidBody) {
		Constraint c;
		c.constraint = nullptr;
		c.boneA = boneName1;
		c.boneB = boneName2;

		glm::mat4 tmp;

		btTransform transformA = bonesByName[boneName1]->rigidBody->getCenterOfMassTransform();
		transformA.getOpenGLMatrix(glm::value_ptr(tmp));
		tmp = glm::inverse(tmp) * bonesByName[boneName2]->globalTransformation;
		tmp = glm::rotate(tmp, (float)90, glm::vec3(0, 1, 0));
		transformA.setFromOpenGLMatrix(glm::value_ptr(tmp));

		btTransform transformB = bonesByName[boneName2]->rigidBody->getCenterOfMassTransform();
		transformB.getOpenGLMatrix(glm::value_ptr(tmp));
		tmp = glm::inverse(tmp) * bonesByName[boneName2]->globalTransformation;
		tmp = glm::rotate(tmp, (float)90, glm::vec3(0, 1, 0));
		transformB.setFromOpenGLMatrix(glm::value_ptr(tmp));

		/*btTransform transformA;
		glm::mat4 a = glm::translate(glm::mat4(), -bonesByName[boneName1]->rigidBodyOffset) * glm::inverse(bonesByName[boneName1]->globalTransformation) * bonesByName[boneName2]->globalTransformation;
		a = glm::rotate(a, (float)90, glm::vec3(0, 0, 1));
		transformA.setFromOpenGLMatrix(glm::value_ptr(a));

		btTransform transformB;
		glm::mat4 b = glm::translate(glm::mat4(), -bonesByName[boneName2]->rigidBodyOffset);
		b = glm::rotate(b, (float)90, glm::vec3(0, 0, 1));
		transformB.setFromOpenGLMatrix(glm::value_ptr(b));*/


		btHingeConstraint* hinge = new btHingeConstraint(*bonesByName[boneName1]->rigidBody, *bonesByName[boneName2]->rigidBody, transformA, transformB);
		hinge->setLimit(low, high);
		hinge->setEnabled(false);
		c.constraint = hinge;

		dynamicsWorld->addConstraint(c.constraint, true);

		constraints.push_back(c);
	}
}

void CharacterSkeleton::setRagdollMode(bool ragdollMode, btDiscreteDynamicsWorld* dynamicsWorld) {
	if (!bRagdoll && ragdollMode) {
		bRagdoll = true;
		for (auto& it : bonesByName) {
			if (it.second->bHasRigidBody) {
				dynamicsWorld->removeRigidBody(it.second->rigidBody.get());

				btScalar mass = 1;
				btVector3 inertia(0, 0, 0);
				it.second->collisionShape.get()->calculateLocalInertia(mass, inertia);
				it.second->rigidBody->setMassProps(mass, inertia);
				it.second->rigidBody->updateInertiaTensor();
				it.second->rigidBody->setDamping(0.05, 0.85);
				it.second->rigidBody->setDeactivationTime(0.8);
				it.second->rigidBody->setSleepingThresholds(1.6, 2.5);
				it.second->rigidBody->activate();

				dynamicsWorld->addRigidBody(it.second->rigidBody.get());


				/*dynamicsWorld->removeRigidBody(it.second->rigidBody.get());

				btTransform transform = it.second->rigidBody->getCenterOfMassTransform();
				btDefaultMotionState* motionState = new btDefaultMotionState(transform);
				btScalar mass = 1;
				btVector3 inertia(0, 0, 0);
				it.second->collisionShape.get()->calculateLocalInertia(mass, inertia);
				btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, it.second->collisionShape.get(), inertia);
				it.second->rigidBody.reset(new btRigidBody(constructionInfo));
				it.second->rigidBody->setDamping(0.05, 0.85);
				it.second->rigidBody->setDeactivationTime(0.8);
				it.second->rigidBody->setSleepingThresholds(1.6, 2.5);

				dynamicsWorld->addRigidBody(it.second->rigidBody.get());*/


				/*btRigidBody bla;
				bla.setMassProps(mass, inertia);*/

				//btRigidBody::btRigidBodyConstructionInfo constructionInfo(0, motionState, collisionShape.get(), btVector3(0, 0, 0));



				/*it.second->rigidBody->setMassProps(1, btVector3(0, 0, 0));
				it.second->rigidBody->updateInertiaTensor();
				it.second->rigidBody->activate();*/




			}
		}
		for (auto& it : constraints) {
			/*glm::vec4 v1(0, 0, 0, 1);
			v1 = glm::inverse(bonesByName[it.boneA]->globalTransformation) * bonesByName[it.boneB]->globalTransformation * v1;
			glm::vec3 v2(v1.x, v1.y, v1.z);
			v2 -= bonesByName[it.boneA]->rigidBodyOffset;

			btVector3 pivotInA(v2.x, v2.y, v2.z);
			btVector3 pivotInB = btVector3(-bonesByName[it.boneB]->rigidBodyOffset.x, -bonesByName[it.boneB]->rigidBodyOffset.y, -bonesByName[it.boneB]->rigidBodyOffset.z);

			//printf("%s pivotInA %f %f %f\n", bonesByName[it.boneA]->boneName.c_str(), v2.x, v2.y, v2.z);
			//it.constraint = new btPoint2PointConstraint(*bonesByName[it.boneA]->rigidBody, *bonesByName[it.boneB]->rigidBody, pivotInA, it.pivotInB);

			btTransform transformA, rotationA;
			transformA.setIdentity();
			transformA.setOrigin(pivotInA);
			rotationA.setIdentity();
			rotationA.setRotation(btQuaternion(0, 0, -M_PI_2));
			transformA = transformA * rotationA;*/

			/*btTransform transformB, rotationB;
			transformB.setIdentity();
			transformB.setOrigin(pivotInB);
			rotationB.setIdentity();
			rotationB.setRotation(btQuaternion(0, 0, M_PI_2));
			transformB = transformB * rotationB;*/



			/*btTransform transformA;
			glm::mat4 a = glm::translate(glm::mat4(), -bonesByName[it.boneA]->rigidBodyOffset) * glm::inverse(bonesByName[it.boneA]->globalTransformation) * bonesByName[it.boneB]->globalTransformation;
			a = glm::rotate(a, (float)90, glm::vec3(0, 0, 1));
			a = glm::rotate(a, it.span1Offset, glm::vec3(0, 1, 0));
			a = glm::rotate(a, it.span2Offset, glm::vec3(0, 0, 1));
			transformA.setFromOpenGLMatrix(glm::value_ptr(a));

			btTransform transformB;
			glm::mat4 b = glm::translate(glm::mat4(), -bonesByName[it.boneB]->rigidBodyOffset);
			b = glm::rotate(b, (float)90, glm::vec3(0, 0, 1));
			b = glm::rotate(b, it.span1Offset, glm::vec3(0, 1, 0));
			b = glm::rotate(b, it.span2Offset, glm::vec3(0, 0, 1));
			transformB.setFromOpenGLMatrix(glm::value_ptr(b));


			it.constraint = new btConeTwistConstraint(*bonesByName[it.boneA]->rigidBody, *bonesByName[it.boneB]->rigidBody, transformA, transformB);
			it.constraint->setLimit(it.span1, it.span2, it.twist);

			dynamicsWorld->addConstraint(it.constraint, true);*/

			it.constraint->setEnabled(true);
		}
		/*for (auto& it : clothComponents)
			it.second.setMaxDistanceScale(1.0f);*/
	}
}

btRigidBody* CharacterSkeleton::getChestRigidBody() {
	return bonesByName["chest"]->rigidBody.get();
}

void CharacterSkeleton::setFootLTarget(glm::vec3 footLTarget) {
	if (bonesByName.find("foot.L") != bonesByName.end()) {
		glm::vec3 newTarget = glm::vec3(glm::inverse(rootTransform) * glm::vec4(footLTarget, 1.0f));
		//printf("leftHitTransformed %f %f %f\n", newTarget.x, newTarget.y, newTarget.z);
		ikChains[0].setRelativeTargetPosition(newTarget);
		//ikChains[0].setRelativeTargetPosition(footLTarget);
	}
}

void CharacterSkeleton::setFootRTarget(glm::vec3 footRTarget) {
	if (bonesByName.find("foot.L") != bonesByName.end()) {
		glm::vec3 newTarget = glm::vec3(glm::inverse(rootTransform) * glm::vec4(footRTarget, 1.0f));
		//printf("rightHitTransformed %f %f %f\n\n", newTarget.x, newTarget.y, newTarget.z);
		ikChains[1].setRelativeTargetPosition(newTarget);
		//ikChains[1].setRelativeTargetPosition(footRTarget);
	}
}

unsigned int CharacterSkeleton::addIKChain(const std::string& boneName, unsigned int length) {
	if (bonesByName.find(boneName) != bonesByName.end()) {
		IKChain newChain;

		vector<Bone3D*> ikChainReverse;
		Bone3D* cBone = bonesByName[boneName];
		unsigned int remainingLength = length;
		while (remainingLength > 0 && cBone) {
			ikChainReverse.push_back(cBone);
			cBone = cBone->parent;
			--remainingLength;
		}
		for (unsigned int i = ikChainReverse.size(); i != 0; --i)
			newChain.addBone(ikChainReverse[i - 1]);
		ikChains.push_back(newChain);
		return ikChains.size() - 1;
	}
	else
		printf("No bone with name %s. No IKChain created.\n", boneName.c_str());
}

glm::vec3 CharacterSkeleton::getIKEndEffectorPosition(unsigned int ikId) {
	if (ikId < ikChains.size())
		return ikChains[ikId].getEndEffector();
	else {
		printf("No IKChain with id %u. Can't get end effector position.\n", ikId);
		return glm::vec3(0, 0, 0);
	}
}

void CharacterSkeleton::setIKTargetPosition(unsigned int ikId, glm::vec3 targetPosition) {
	if (ikId < ikChains.size()) {
		glm::vec3 newTarget = glm::vec3(glm::inverse(rootTransform) * glm::vec4(targetPosition, 1.0f));
		ikChains[ikId].setRelativeTargetPosition(newTarget);
	}
	else
		printf("No IKChain with id %u. Can't get set target position.\n", ikId);
}

void CharacterSkeleton::setDebugPoints(glm::vec3 debugGround1, glm::vec3 debugGround2, glm::vec3 debugGroundOffset1, glm::vec3 debugGroundOffset2) {
	this->debugGround1 = debugGround1;
	this->debugGround2 = debugGround2;
	this->debugGroundOffset1 = debugGroundOffset1;
	this->debugGroundOffset2 = debugGroundOffset2;
}
