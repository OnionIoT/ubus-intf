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

// public functions

int Device::Process(char* function, char* json)
{
	int status		= 0;

	// parse the json
	jsonDoc.Parse(json);
	
	// call the private process function
	status 	= _Process(function);


	return (status);
}

void Device::PrintJsonObj(void)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	jsonOut.Accept(writer);

	std::string str = buffer.GetString();

	printf("%s\n", str.c_str() );
}


void Device::SetDebugMode(bool input)
{
	bDebugMode	= input;
}

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


