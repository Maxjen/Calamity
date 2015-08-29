#include "BoneSelection.h"

BoneSelection::BoneSelection(GameManager* gameManager, BoneManager* boneManager) {
	this->gameManager = gameManager;
	this->boneManager = boneManager;
	boneProperties = nullptr;
	mainSelectedBone = -1;
}

void BoneSelection::setBoneProperties(BoneProperties* boneProperties) {
	this->boneProperties = boneProperties;
}

void BoneSelection::updateProperties() {
	if (boneProperties)
		boneProperties->updateProperties();
}

void BoneSelection::selectBone(unsigned int boneId, int boneVertices) {
	if (boneVertices != 0) {
		if (selectedBones.find(boneId) != selectedBones.end()) {
			selectedBones[boneId] ^= boneVertices;
		}
		else
			selectedBones[boneId] = boneVertices;

		BoneObject* b = (BoneObject*)gameManager->getObject(boneId);

		if (boneVertices == 1)
			b->toggleRootSelection();
		else if (boneVertices == 2)
			b->toggleTipSelection();
		else if (boneVertices == 3) {
			b->toggleTipSelection();
			b->toggleRootSelection();
		}

		if (selectedBones[boneId] == 0) {
			if (mainSelectedBone == boneId)
				setMainSelectedBone(-1);
			selectedBones.erase(boneId);
		}

		b->updateBoneOverlayColor();
	}
}

void BoneSelection::selectBoneAbsolute(unsigned int boneId, int boneVertices) {
	int before = 0;
	if (selectedBones.find(boneId) != selectedBones.end())
		before = selectedBones[boneId];

	selectedBones[boneId] = boneVertices;

	BoneObject* b = (BoneObject*)gameManager->getObject(boneId);

	int difference = before ^ boneVertices;
	if (difference == 1)
		b->toggleRootSelection();
	else if (difference == 2)
		b->toggleTipSelection();
	else if (difference == 3) {
		b->toggleTipSelection();
		b->toggleRootSelection();
	}

	if (boneVertices == 0) {
		if (mainSelectedBone == boneId)
			setMainSelectedBone(-1);
		selectedBones.erase(boneId);
	}

	b->updateBoneOverlayColor();
}

void BoneSelection::setMainSelectedBone(unsigned int mainSelectedBone) {
	if (selectedBones.find(mainSelectedBone) != selectedBones.end() || mainSelectedBone == (unsigned int)-1) {
		if (this->mainSelectedBone != (unsigned int)-1) {
			BoneObject* oldMainSelectedBone = (BoneObject*)gameManager->getObject(this->mainSelectedBone);
			oldMainSelectedBone->setMainSelectedObject(false);
			oldMainSelectedBone->updateBoneOverlayColor();
		}

		this->mainSelectedBone = mainSelectedBone;
		boneManager->setSelectedBoneId(mainSelectedBone);

		if (mainSelectedBone != (unsigned int)-1) {
			BoneObject* newMainSelectedBone = (BoneObject*)gameManager->getObject(this->mainSelectedBone);
			newMainSelectedBone->setMainSelectedObject(true);
			newMainSelectedBone->updateBoneOverlayColor();
		}

		if (boneProperties)
			boneProperties->setBoneId(mainSelectedBone);

		/*if (boneObjectProperties) {
			mapObjectProperties->updateProperties();

			if (boxLayout) {
				boxLayout->clearBox(boxId);
				//if (selectedSprites.size() == 1) {
				if (mainSelectedSprite != (unsigned int)-1) {
					InputGroup* objectTransformInput = new InputGroup(spriteManager, fontId, 150, true);
					boxLayout->setBoxWidget(boxId, objectTransformInput);
					objectTransformInput->addRow();
					objectTransformInput->addInput(0, L"X", CLM_FLOAT, mapObjectProperties, mapObjectProperties->getXProperty());
					objectTransformInput->addInput(0, L"Y", CLM_FLOAT, mapObjectProperties, mapObjectProperties->getYProperty());
					objectTransformInput->addRow();
					objectTransformInput->addInput(1, L"Scale", CLM_FLOAT, mapObjectProperties, mapObjectProperties->getScaleProperty());
					objectTransformInput->addRow();
					objectTransformInput->addInput(2, L"Rotation", CLM_FLOAT, mapObjectProperties, mapObjectProperties->getRotationProperty());
					objectTransformInput->initialize();
				}
			}
		}*/
	}
}

unordered_map<unsigned int, int> BoneSelection::getSelectedBones() {
	return selectedBones;
}

unsigned int BoneSelection::getMainSelectedBone() {
	return mainSelectedBone;
}

GameManager* BoneSelection::getGameManager() {
	return gameManager;
}

bool BoneSelection::moreThanOneVertex() {
	return selectedBones.size() > 1 || (selectedBones.size() == 1 && (selectedBones.begin()->second == 3));
}
