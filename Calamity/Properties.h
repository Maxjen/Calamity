#pragma once

#include <string>

using std::wstring;
using std::string;

namespace clm {

class Property {
protected:
	explicit Property() : bHasChanged(false) {}

	bool bHasChanged;
public:
	virtual bool hasChanged() { return bHasChanged; }
	virtual void setUnchanged() { bHasChanged = false; }

	virtual bool isValid(wstring valueString) { return true; }
	virtual wstring correctValue(wstring wValue) { return wValue; }

	virtual wstring getValueAsString() { return L""; }
	virtual void setValueFromString(wstring valueString) {}
};

class FloatProperty : public Property {
public:
	explicit FloatProperty();

	bool isValid(wstring wValue);
	wstring correctValue(wstring wValue);

	wstring getValueAsString();
	void setValueFromString(wstring wValue);

	void updateValue(float value);
	float getValue();
private:
	float value;
};

class IntProperty : public Property {
public:
	explicit IntProperty();

	bool isValid(wstring wValue);
	wstring correctValue(wstring wValue);

	wstring getValueAsString();
	void setValueFromString(wstring wValue);

	void updateValue(int value);
	int getValue();
private:
	int value;
};

class StringProperty : public Property {
public:
	explicit StringProperty();

	bool isValid(wstring wValue);
	wstring correctValue(wstring wValue);

	wstring getValueAsString();
	void setValueFromString(wstring wValue);

	void updateValue(wstring value);
private:
	wstring value;
};

class BoolProperty : public Property {
public:
	explicit BoolProperty();

	bool isValid(wstring wValue);
	wstring correctValue(wstring wValue);

	wstring getValueAsString();
	void setValueFromString(wstring wValue);

	void updateValue(bool value);
	bool getValue();
private:
	bool value;
};

class Properties {
public:
	virtual void pushPropertyChange(Property* property, wstring wOldValue, wstring wNewValue) = 0;
};

}
