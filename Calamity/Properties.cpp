#include "Properties.h"

namespace clm {

FloatProperty::FloatProperty() :
	value(0)
{}

bool FloatProperty::isValid(wstring wValue) {
	try {
		stof(wValue);
		if (getValueAsString().compare(correctValue(wValue)) != 0)
			return true;
		else
			return false;
		/*if (fValue != value)
			return true;
		else
			return false;*/
	}
	catch(...) {
		return false;
	}
}

wstring FloatProperty::correctValue(wstring wValue) {
	float fValue;
	fValue = stof(wValue);

	char buffer[10];
	snprintf(buffer, 10, "%.2f", fValue);
	string sValue = buffer;

	wstring correctedString;
	correctedString.assign(sValue.begin(), sValue.end());
	return correctedString;
}

wstring FloatProperty::getValueAsString() {
	char buffer[10];
	snprintf(buffer, 10, "%.2f", value);
	string sValue = buffer;
	wstring wValue;
	wValue.assign(sValue.begin(), sValue.end());
	return wValue;
}

void FloatProperty::setValueFromString(wstring wValue) {
	float fValue;
	fValue = stof(wValue);

	char buffer[10];
	snprintf(buffer, 10, "%.2f", fValue);
	string sValue = buffer;

	value = stof(sValue);

	bHasChanged = true;
}

void FloatProperty::updateValue(float value) {
	this->value = value;
}

float FloatProperty::getValue() {
	return value;
}

IntProperty::IntProperty() :
	value(0)
{}

bool IntProperty::isValid(wstring wValue) {
	try {
		stoi(wValue);
		if (getValueAsString().compare(correctValue(wValue)) != 0)
			return true;
		else
			return false;
	}
	catch(...) {
		return false;
	}
}

wstring IntProperty::correctValue(wstring wValue) {
	int iValue;
	iValue = stoi(wValue);

	char buffer[10];
	snprintf(buffer, 10, "%d", iValue);
	string sValue = buffer;

	wstring correctedString;
	correctedString.assign(sValue.begin(), sValue.end());
	return correctedString;
}

wstring IntProperty::getValueAsString() {
	char buffer[10];
	snprintf(buffer, 10, "%d", value);
	string sValue = buffer;
	wstring wValue;
	wValue.assign(sValue.begin(), sValue.end());
	return wValue;
}

void IntProperty::setValueFromString(wstring wValue) {
	int iValue;
	iValue = stoi(wValue);

	char buffer[10];
	snprintf(buffer, 10, "%d", iValue);
	string sValue = buffer;

	value = stoi(sValue);

	bHasChanged = true;
}

void IntProperty::updateValue(int value) {
	this->value = value;
}

int IntProperty::getValue() {
	return value;
}

StringProperty::StringProperty() :
	value()
{}

bool StringProperty::isValid(wstring wValue) {
	return wValue.size() > 0;
}

wstring StringProperty::correctValue(wstring wValue) {
	return wValue;
}

wstring StringProperty::getValueAsString() {
	return value;
}

void StringProperty::setValueFromString(wstring wValue) {
	value = wValue;

	bHasChanged = true;
}

void StringProperty::updateValue(wstring value) {
	this->value = value;
}

BoolProperty::BoolProperty() :
	value(false)
{}

bool BoolProperty::isValid(wstring wValue) {
	if (wValue.compare(L"true") == 0 || wValue.compare(L"false") == 0)
		return true;
	else
		return false;
}

wstring BoolProperty::correctValue(wstring wValue) {
	if (wValue.compare(L"true") == 0 || wValue.compare(L"false") == 0)
		return wValue;
	else
		return L"false";
}

wstring BoolProperty::getValueAsString() {
	wstring wValue;
	if (value)
		wValue.assign(L"true");
	else
		wValue.assign(L"false");
	return wValue;
}

void BoolProperty::setValueFromString(wstring wValue) {
	if (wValue.compare(L"true") == 0)
		value = true;
	else
		value = false;

	bHasChanged = true;
}

void BoolProperty::updateValue(bool value) {
	this->value = value;
}

bool BoolProperty::getValue() {
	return value;
}

}
