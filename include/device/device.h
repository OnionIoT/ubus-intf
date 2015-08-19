#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
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

	void			SetDebugMode				(bool input);
	

protected:
	// protected functions
	void 			SetDebugSystemCommandResp	(char input[]);

	void			SystemCommandExec	(char* cmd);
	void			SystemCommandRead	(char* cmd, char* response);


	void 			JsonPrint		(void);
	void 			JsonAddMember	(char* key, rapidjson::Value element);

	int 			JsonGetBool		(char *key, bool *output);
	int 			JsonGetInt		(char *key, int *output);
	int 			JsonGetDouble	(char *key, double *output);

	bool 			JsonReadBool	(char *key);
	int 			JsonReadInt		(char *key);
	double			JsonReadDouble	(char *key);

	

	

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