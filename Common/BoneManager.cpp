#include "BoneManager.h"

BoneManager::BoneManager(Renderer* renderer, GameManager* gameManager) :
	renderer(renderer),
	gameManager(gameManager),
	bones(),
	usedBones(),
	cZOrder(0),
	selectedBoneId(-1)
{}

BoneManager::~BoneManager() {
	/*for (auto& it : layers)
		delete it.second;
	for (auto& it : objects)
		delete it.second;*/
}

unsigned int BoneManager::getSelectedBoneId() {
	return selectedBoneId;
}

void BoneManager::setSelectedBoneId(unsigned int selectedBoneId) {
	//if (layers.find(selectedLayerId) != layers.end() && usedLayers.find(layers[selectedLayerId].zOrder) != usedLayers.end())
	this->selectedBoneId = selectedBoneId;
}

void BoneManager::selectLowerBone() {
	if (selectedBoneId != (unsigned int)-1) {
		map<unsigned int, unsigned int>::iterator it = usedBones.find(bones[selectedBoneId].zOrder);
		if (it != usedBones.end() && it != usedBones.begin()) {
			--it;
			selectedBoneId = it->second;
		}
		else
			selectedBoneId = -1;
	}
}

unsigned int BoneManager::addBone(wstring boneName, unsigned int boneId) {
	Bone& bone = bones[boneId];
	bone.zOrder = cZOrder;
	bone.boneName = boneName;

	usedBones[cZOrder] = boneId;

	++cZOrder;

	return boneId;
}

// TODO: actually remove objects from layers
void BoneManager::removeBone(unsigned int boneId) {
	if (bones.find(boneId) != bones.end() && usedBones.find(bones[boneId].zOrder) != usedBones.end()) {
		usedBones.erase(bones[boneId].zOrder);
	}
	else
		printf("There is no bone with id %u\n", boneId);
}

void BoneManager::restoreBone(unsigned int boneId) {
	if (bones.find(boneId) != bones.end() && usedBones.find(bones[boneId].zOrder) == usedBones.end()) {
		usedBones[bones[boneId].zOrder] = boneId;
	}
	else
		printf("Can't restore bone with id %u\n", boneId);
}

void BoneManager::moveBoneUp(unsigned int boneId) {
	if (bones.find(boneId) != bones.end() && usedBones.find(bones[boneId].zOrder) != usedBones.end()) {
		map<unsigned int, unsigned int>::iterator cBone = usedBones.find(bones[boneId].zOrder);
		map<unsigned int, unsigned int>::iterator uBone = cBone;
		if (cBone != --usedBones.end()) {
			++uBone;
			{
				// swapp zOrder in bones
				unsigned int zOrderTmp = cBone->first;
				bones[cBone->second].zOrder = uBone->first;
				bones[uBone->second].zOrder = zOrderTmp;

				// swapp values (boneId) in usedBones
				unsigned int boneIdTmp = cBone->second;
				cBone->second = uBone->second;
				uBone->second = boneIdTmp;
			}
		}
	}
	else
		printf("There is no bone with id %u\n", boneId);
}

void BoneManager::moveBoneDown(unsigned int boneId) {
	if (bones.find(boneId) != bones.end() && usedBones.find(bones[boneId].zOrder) != usedBones.end()) {
		map<unsigned int, unsigned int>::iterator cBone = usedBones.find(bones[boneId].zOrder);
		map<unsigned int, unsigned int>::iterator lBone = cBone;
		if (cBone != usedBones.begin()) {
			--lBone;
			{
				// swapp zOrder in bones
				unsigned int zOrderTmp = cBone->first;
				bones[cBone->second].zOrder = lBone->first;
				bones[lBone->second].zOrder = zOrderTmp;

				// swapp values (boneId) in usedBones
				unsigned int boneIdTmp = cBone->second;
				cBone->second = lBone->second;
				lBone->second = boneIdTmp;
			}
		}
	}
	else
		printf("There is no bone with id %u\n", boneId);
}

wstring BoneManager::getBoneName(unsigned int boneId) {
	if (bones.find(boneId) != bones.end() && usedBones.find(bones[boneId].zOrder) != usedBones.end()) {
		return bones[boneId].boneName;
	}
	else {
		printf("There is no bone with id %u\n", boneId);
		return 0;
	}
}

void BoneManager::setBoneName(unsigned int boneId, wstring boneName) {
	if (bones.find(boneId) != bones.end() && usedBones.find(bones[boneId].zOrder) != usedBones.end()) {
		bones[boneId].boneName = boneName;
	}
	else
		printf("There is no bone with id %u\n", boneId);
}

list<unsigned int> BoneManager::getBoneIds() {
	list<unsigned int> result;
	for (auto& it : usedBones)
		result.push_back(it.second);
	result.reverse();
	return result;
}

/*void BoneManager::prepareLayers(float viewPosX, float viewPosY) {
	for (auto& it : usedLayers)
		gameManager->prepareLayer(it.second, viewPosX, viewPosY);
}

void BoneManager::drawLayers() {
	for (auto& it : usedLayers)
		gameManager->drawLayer(it.second);
}

void BoneManager::drawOverlays() {
	for (auto& it : usedLayers)
		gameManager->drawOverlay(it.second);
}

void BoneManager::clearLayers() {
	for (auto& it : usedLayers)
		gameManager->clearLayer(it.second);
}*/
