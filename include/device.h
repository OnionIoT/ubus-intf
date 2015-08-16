#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <jsonHelper.h>

#include <stdlib.h>
#include <cstdio>
#include <cstring>

#include <module.h>

class Device : public Module {
public:
	Device(void);
	~Device(void);

	virtual int 	Process			(char* function, char* json)	= 0;

	void			SetDebugMode	(bool input);
	void			SystemCommand	(char* cmd);

protected:
	// protected members
	JsonHelper 		jsonHelperObj;

	bool			bDebugMode;

};

#endif	// _DEVICE_H_