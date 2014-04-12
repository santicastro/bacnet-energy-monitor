/*
 * Copyright (c) 2014 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef TESTAVERAGING_H_
#define TESTAVERAGING_H_

#include "cute.h"
#include "Averaging.h"
#include "AnalogValue.h"
extern "C"{
	#include "bacenum.h"
	#include "bacapp.h"
	#include "bacdevobjpropref.h"
}

class TestAveraging {
public:
	TestAveraging();
	~TestAveraging();
	//tests
	void nothing();
	void testCount();
	void testObject_Valid_Object_Instance_Number();
	void testReadPropertyObjectName();
	void testReadPropertyObjectID();
	void testReadPropertyObjectType();
	void testReadPropertyMinimumValue();
	void testReadPropertyMaximumValue();
	void testReadPropertyAttemptedSamples();
	void testReadPropertyValidSamples();
	void testReadPropertyAverageValue();
	void testReadWindowInterval();
	void testReadWindowSamples();
	void testReadNonExistingProperty();
	void testReadPropertyArrayIndexOfNonArrayObject();
	void testReadPropertyAverageValueAfterInsertingNewValue();
	void testReadProperyObjectPropertyReference();
	void testReadPropertyAverageValueAfterInsertingOutOfBoundsData();

private:
	AnalogValue* analog_value;
	Averaging* averaging;
	void createAPDU(BACNET_READ_PROPERTY_DATA& rpdata, BACNET_APPLICATION_DATA_VALUE& appDataValueIN,
						BACNET_PROPERTY_ID property);
};

#endif /* TESTAVERAGINGOBJECT_H_ */
