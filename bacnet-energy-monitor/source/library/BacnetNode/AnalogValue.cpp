/*
 * Copyright (c) 2013 by Miguel Fernandes
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "AnalogValue.h"

AnalogValue::AnalogValue(const uint32_t objectID, const char* objectName, const char* description, BACNET_ENGINEERING_UNITS units):
	BacnetObject(objectID, OBJECT_ANALOG_VALUE, objectName){
	Present_Value = (float)-1.0;
	characterstring_init_ansi(&Description, description);
	bitstring_init(&Status_Flags);
	bitstring_set_bit(&Status_Flags, STATUS_FLAG_FAULT, false);
	bitstring_set_bit(&Status_Flags, STATUS_FLAG_IN_ALARM, false);
	bitstring_set_bit(&Status_Flags, STATUS_FLAG_OUT_OF_SERVICE, false);
	bitstring_set_bit(&Status_Flags, STATUS_FLAG_OVERRIDDEN, false);
	Event_State = EVENT_STATE_NORMAL;
	Reliability = RELIABILITY_NO_FAULT_DETECTED;
	Out_Of_Service = false;
	Units = units;
};

AnalogValue::~AnalogValue(){}

float AnalogValue::getPresentValue(){
	return Present_Value;
}
void AnalogValue::setPresentValue(float value){
	Present_Value = value;
}
const char* AnalogValue::getDescription(){
	return characterstring_value(&Description);
}

bool AnalogValue::setDescription(const char* description){
	return characterstring_init_ansi(&Description, description);
}

BACNET_BIT_STRING AnalogValue::getStatusFlags(){
	return Status_Flags;
}

BACNET_EVENT_STATE AnalogValue::getEventState(){
	return Event_State;
}

void AnalogValue::setEventState(BACNET_EVENT_STATE state){
	Event_State = state;
	if (Event_State != EVENT_STATE_NORMAL){
		bitstring_set_bit(&Status_Flags, STATUS_FLAG_IN_ALARM, true);
	}else{
		bitstring_set_bit(&Status_Flags, STATUS_FLAG_IN_ALARM, false);
	}
}

BACNET_RELIABILITY AnalogValue::getReliability(){
	return Reliability;
}

bool AnalogValue::setReliability(BACNET_RELIABILITY reliability){
	if (reliability == RELIABILITY_NO_FAULT_DETECTED){
		bitstring_set_bit(&Status_Flags, STATUS_FLAG_FAULT, false);
	}else if ((reliability == RELIABILITY_OVER_RANGE) | (reliability == RELIABILITY_UNDER_RANGE) |
			(reliability == RELIABILITY_UNRELIABLE_OTHER)){
		bitstring_set_bit(&Status_Flags, STATUS_FLAG_FAULT, true);
	}else{
		return false;
	}
	Reliability = reliability;
	return true;
}
bool AnalogValue::isOutOfService(){
	return Out_Of_Service;
}
BACNET_ENGINEERING_UNITS AnalogValue::getUnits(){
	return Units;
}

int AnalogValue::Object_Read_Property(BACNET_READ_PROPERTY_DATA * rpdata){
	int apdu_len = 0;   /* return value */
	//int len = 0;        /* apdu len intermediate value */
	uint8_t *apdu = NULL;
	//BACNET_BIT_STRING bit_string;

	if ((rpdata == NULL) || (rpdata->application_data == NULL) || (rpdata->application_data_len == 0)) {
		return 0;
	}
	apdu = rpdata->application_data;
	switch (rpdata->object_property) {
		case PROP_OBJECT_NAME:
			apdu_len = encode_application_character_string(&apdu[0], &Object_Name);
			break;
		case PROP_OBJECT_IDENTIFIER:
			apdu_len = encode_application_object_id(&apdu[0], getObjectIdentifier()->type, getObjectIdentifier()->instance);
			break;
		case PROP_OBJECT_TYPE:
			apdu_len = encode_application_enumerated(&apdu[0], getObjectIdentifier()->type);
			break;
		case PROP_PRESENT_VALUE:
			apdu_len =  encode_application_real(&apdu[0], getPresentValue());
			break;
		case PROP_DESCRIPTION:
			BACNET_CHARACTER_STRING char_string;
			characterstring_init_ansi(&char_string, Description.value);
			apdu_len = encode_application_character_string(&apdu[0], &char_string);
			break;
		case PROP_STATUS_FLAGS:
			apdu_len = encode_application_bitstring(&apdu[0], &Status_Flags);
			break;
		case PROP_EVENT_STATE:
			apdu_len = encode_application_enumerated(&apdu[0], Event_State);
			break;
		case PROP_RELIABILITY:
			apdu_len = encode_application_enumerated(&apdu[0], Reliability);
			break;
		case PROP_OUT_OF_SERVICE:
			apdu_len = encode_application_boolean(&apdu[0], Out_Of_Service);
			break;
		case PROP_UNITS:
			apdu_len = encode_application_enumerated(&apdu[0], Units);
			break;
		default:
			rpdata->error_class = ERROR_CLASS_PROPERTY;
			rpdata->error_code = ERROR_CODE_UNKNOWN_PROPERTY;
			apdu_len = BACNET_STATUS_ERROR;
			break;
	}
	return apdu_len;
}
