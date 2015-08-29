#pragma once

#include <dirent.h>

#include "../Calamity/Properties.h"
#include "MapEditorSelection.h"
#include "Gui/WindowManager.h"
#include "Gui/VerticalBoxLayout.h"
#include "Gui/VerticalLayout.h"
#include "Gui/HorizontalLayout.h"
#include "Gui/SegmentControl.h"
#include "Gui/ButtonGroup.h"
#include "Gui/InputGroup.h"
#include "Gui/LineEdit.h"
#include "Gui/Checkbox.h"
#include "Gui/ObjectItem.h"

using namespace clm;

enum MapEditorMode {
	POLYGON_MODE,
	OBJECT_MODE,
};

class MapEditorProperties : public Properties {
private:
	SpriteManager* spriteManager;
	MapEditorSelection* selection;
	clm::Window* window;
	unsigned int fontId;

	string cTextureFileName;
	string cObjectFileName;

	IntProperty editorMode;
	wstring previousEditorMode;

	FloatProperty eyeOffset;
	BoolProperty anaglyph3D;
public:
	explicit MapEditorProperties(SpriteManager* spriteManager, MapEditorSelection* selection);
	void pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue);
	void applyChange();

	void setWindow(clm::Window* window);
	void setFontId(unsigned int fontId);
	void loadPolygonPanel();
	void loadObjectPanel();

	IntProperty* getEditorModeProperty();
	wstring getPreviousEditorMode();
	FloatProperty* getEyeOffsetProperty();
	BoolProperty* getAnaglyph3DProperty();

	string getCTextureFileName();
	string getCObjectFileName();
};
