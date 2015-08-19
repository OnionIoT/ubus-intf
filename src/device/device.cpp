#include <device/device.h>

Device::Device(void)
{
	// not in debug mode by default
	bDebugMode	= false;

	// set pointers to null
	debugResponse	= NULL;
}

Device::~Device(void)
{
	// deallocate any memory
	if (debugResponse != NULL)		delete[] debugResponse;
}

//// public functions
int Device::Process(char* function, char* json)
{
	int status		= 0;

	// parse the json
	jsonDoc.Parse(json);
	
	// call the private process function
	status 	= _Process(function);


	return (status);
}

void Device::SetDebugMode(bool input)
{
	bDebugMode	= input;
}

//// protected functions
void Device::SetDebugSystemCommandResp	(char input[])
{
	// allocate memory
	debugResponse	= new char[1024];

	strcpy(debugResponse, input);
}

void Device::SystemCommandExec(char* cmd)
{
	if (bDebugMode) {
		printf("Running: %s", cmd);
	}
	else	{
		system(cmd);
	}
}

void Device::SystemCommandRead(char* cmd, char* response)
{
	if (bDebugMode) {
		printf("Running: %s", cmd);

		strcpy(response, debugResponse);
	}
	else	{
		// run the specified command and read the system output
		FILE* pipe = popen(cmd, "r");
	    if (!pipe) return;
	    
	    char buffer[128];
	    std::string result = "";
	    
	    while(!feof(pipe)) {
	    	if(fgets(buffer, 128, pipe) != NULL)
	    		result += buffer;
	    }
	    
	    pclose(pipe);
	    strcpy(response, result.c_str() );
	}
}

// json functions
void Device::JsonPrint(void)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	jsonOut.Accept(writer);

	std::string str = buffer.GetString();

	printf("%s\n", str.c_str() );
}

void Device::JsonAddMember(char* key, rapidjson::Value element)
{
	// add the member to jsonOut
	jsonOut.AddMember	(	rapidjson::Value(key, jsonOut.GetAllocator()).Move(), 
							element, 
							jsonOut.GetAllocator() 
						);
}

// returns 1 if successfully parsed
int Device::JsonGetBool(char *key, bool *output)
{
	int 	status	= 0;

	// check if element with this key exists
	if ( jsonDoc.HasMember( key )	) {
		// check for the bool type
		if ( jsonDoc[key].IsBool() )	{
			status		= 1;
			(*output) 	= jsonDoc[key].GetBool();
		}
		// check for the string type
		else if ( jsonDoc[key].IsString() ) 	{
			if ( strcmp("true", jsonDoc[key].GetString() ) == 0 )	{
				status		= 1;
				(*output) 	= true;
			}
			else if ( strcmp("false", jsonDoc[key].GetString() ) == 0 )	{
				status		= 1;
				(*output) 	= false;
			}
		}
	}

	return (status);
}

int Device::JsonGetInt(char *key, int *output)
{
	int 	status	= 0;

	// check if element with this key exists
	if ( jsonDoc.HasMember( key )	) {
		// check for the bool type
		if ( jsonDoc[key].IsInt() )	{
			status		= 1;
			(*output) 	= jsonDoc[key].GetInt();
		}
		// check for the string type
		else if ( jsonDoc[key].IsString() ) 	{
			status 		= 1;
			(*output)	= atoi ( jsonDoc[key].GetString() );
		}
	}

	return (status);
}

int Device::JsonGetDouble(char *key, double *output)
{
	int 	status	= 0;

	// check if element with this key exists
	if ( jsonDoc.HasMember( key )	) {
		// check for the bool type
		if ( jsonDoc[key].IsInt() )	{
			status		= 1;
			(*output) 	= jsonDoc[key].GetDouble();
		}
		// check for the string type
		else if ( jsonDoc[key].IsString() ) 	{
			status 		= 1;
			(*output)	= atof ( jsonDoc[key].GetString() );
		}
	}

	return (status);
}


bool Device::JsonReadBool(char *key)
{
	bool 	ret;

	if ( JsonGetBool(key, &ret) )	{
		return (ret);
	}

	return (false);
}

int Device::JsonReadInt(char *key)
{
	int 	ret;

	if ( JsonGetInt(key, &ret) )	{
		return (ret);
	}

	return (0);
}

double Device::JsonReadDouble(char *key)
{
	double 	ret;

	if ( JsonGetDouble(key, &ret) )	{
		return (ret);
	}

	return (0);
}






