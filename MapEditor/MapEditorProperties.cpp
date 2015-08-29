#include "MapEditorProperties.h"

MapEditorProperties::MapEditorProperties(SpriteManager* spriteManager, MapEditorSelection* selection) :
	spriteManager(spriteManager),
	selection(selection),
	window(nullptr),
	fontId(-1),
	cTextureFileName(""),
	cObjectFileName(""),
	editorMode(),
	previousEditorMode(),
	eyeOffset(),
	anaglyph3D()
{
	editorMode.updateValue(POLYGON_MODE);

	char buffer[3];
	snprintf(buffer, 3, "%d", OBJECT_MODE);
	string sValue = buffer;
	previousEditorMode.assign(sValue.begin(), sValue.end());

	eyeOffset.updateValue(8.0f);
}

void MapEditorProperties::pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue) {
	if (property == &editorMode) {
		if (wNewValue.compare(wOldValue) != 0)
			previousEditorMode = wOldValue;
	}

	property->setValueFromString(wNewValue);
	applyChange();
}

void MapEditorProperties::applyChange() {
	if (editorMode.hasChanged()) {
		editorMode.setUnchanged();
		if (editorMode.getValue() == POLYGON_MODE)
			loadPolygonPanel();
		else if (editorMode.getValue() == OBJECT_MODE)
			loadObjectPanel();
		return;
	}
	if (eyeOffset.hasChanged()) {
		eyeOffset.setUnchanged();
		return;
	}
	if (anaglyph3D.hasChanged()) {
		anaglyph3D.setUnchanged();
		return;
	}
}



void MapEditorProperties::setWindow(Window* window) {
	this->window = window;
}

void MapEditorProperties::setFontId(unsigned int fontId) {
	this->fontId = fontId;
}

void MapEditorProperties::loadPolygonPanel() {
	window->clear();

	VerticalBoxLayout* boxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	window->setChild(boxLayout);

	ListView* textureList = new ListView(spriteManager, 100, 100, true, true);
	unsigned int textureListBox = boxLayout->addBox();
	boxLayout->setBoxWidget(textureListBox, textureList);

	{
		DIR *dp;
		struct dirent *dirp;
		//dp  = opendir((string("/home/max/projects/mt/build/").append(folderName)).c_str());
		dp  = opendir(string("Data/Textures").c_str());
		if (dp == NULL)
			printf("Unable to open Data/Textures");

		list<string> fileNames;
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_name[0] != '.') {
				string fileName = dirp->d_name;
				fileNames.push_back(fileName);

				//printf("%s\n", dirp->d_name);
				/*string currentFile = folderName;
				currentFile.append(dirp->d_name);
				string label = dirp->d_name;
				if(label.substr(label.length()-extension.length()) == extension) {
					label = label.substr(0, label.length()-extension.length());

					//newElement->texture = textureCache->get(currentTexture.c_str());
					newElement->label = new mtLabel(label.c_str(), font, true);
					newElement->value = currentFile;
					addListElement(newElement);
				}*/
			}
		}
		closedir(dp);
		fileNames.sort();
		for (auto& it : fileNames) {
			ObjectItem* item = new ObjectItem(spriteManager, &cTextureFileName, it.c_str(), fontId);
			textureList->addItem(item);
		}
	}

	unsigned int objectPropertiesBox = boxLayout->addBox();
	selection->setBoxWidget(boxLayout, objectPropertiesBox, fontId);
}

void MapEditorProperties::loadObjectPanel() {
	window->clear();

	VerticalBoxLayout* boxLayout = new VerticalBoxLayout(spriteManager, 0, 0, true, true);
	window->setChild(boxLayout);

	ListView* objectList = new ListView(spriteManager, 100, 100, true, true);
	unsigned int objectListBox = boxLayout->addBox();
	boxLayout->setBoxWidget(objectListBox, objectList);

	{
		DIR *dp;
		struct dirent *dirp;
		//dp  = opendir((string("/home/max/projects/mt/build/").append(folderName)).c_str());
		dp  = opendir(string("Data/Objects").c_str());
		if (dp == NULL)
			printf("Unable to open Data/Objects");

		list<string> fileNames;
		while ((dirp = readdir(dp)) != NULL) {
			if (dirp->d_name[0] != '.') {
				string fileName = dirp->d_name;
				fileNames.push_back(fileName);

				//printf("%s\n", dirp->d_name);
				/*string currentFile = folderName;
				currentFile.append(dirp->d_name);
				string label = dirp->d_name;
				if(label.substr(label.length()-extension.length()) == extension) {
					label = label.substr(0, label.length()-extension.length());

					//newElement->texture = textureCache->get(currentTexture.c_str());
					newElement->label = new mtLabel(label.c_str(), font, true);
					newElement->value = currentFile;
					addListElement(newElement);
				}*/
			}
		}
		closedir(dp);
		fileNames.sort();
		for (auto& it : fileNames) {
			ObjectItem* item = new ObjectItem(spriteManager, &cObjectFileName, it.c_str(), fontId);
			objectList->addItem(item);
		}
	}

	unsigned int objectPropertiesBox = boxLayout->addBox();
	selection->setBoxWidget(boxLayout, objectPropertiesBox, fontId);
}

IntProperty* MapEditorProperties::getEditorModeProperty() {
	return &editorMode;
}

wstring MapEditorProperties::getPreviousEditorMode() {
	return previousEditorMode;
}

FloatProperty* MapEditorProperties::getEyeOffsetProperty() {
	return &eyeOffset;
}

BoolProperty* MapEditorProperties::getAnaglyph3DProperty() {
	return &anaglyph3D;
}

string MapEditorProperties::getCTextureFileName() {
	return cTextureFileName;
}

string MapEditorProperties::getCObjectFileName() {
	return cObjectFileName;
}
