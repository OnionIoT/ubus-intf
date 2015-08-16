#ifndef _JSON_HELPER_H_
#define _JSON_HELPER_H_


#include <rapidjson/document.h>

#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <string>

#include <vector>
#include <map>


#include <module.h>


class JsonHelper : public Module {
public:
	JsonHelper(void);
	~JsonHelper(void);

	void 			Read 			(char* json);
	int 			FindElement		(std::string);
	
	std::string 	GetElement		(std::string key);

private:
	// private members
	std::map<std::string, std::string> 	elements;

	std::map<std::string, std::string>	stringElements;
	std::map<std::string, int>			intElements;
	std::map<std::string, bool>			boolElements;
};


#endif 	// _JSON_HELPER_H_