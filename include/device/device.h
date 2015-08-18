#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

#include <module.h>

class Device : public Module {
public:
	Device(void);
	~Device(void);

	int 			Process				(char* function, char* json);

	void 			PrintJsonObj		(void);

	void			SystemCommandExec	(char* cmd);
	void			SystemCommandRead	(char* cmd, char* response);

	void			SetDebugMode				(bool input);
	void 			SetDebugSystemCommandResp	(char input[]);

protected:
	// protected functions


	// protected members
	rapidjson::Document 	jsonDoc;
	rapidjson::Document 	jsonOut;

	bool					bDebugMode;

private:
	// private functions
	virtual int 	_Process		(char* function) = 0;

	// private members
	char*			debugResponse;
};

#endif	// _DEVICE_H_