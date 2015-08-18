#include <device.h>

Device::Device(void)
{
	// not in debug mode by default
	bDebugMode	= false;
}

Device::~Device(void)
{
	// nothing for now
}

// public functions
void Device::SetDebugMode(bool input)
{
	bDebugMode	= input;
}

void Device::SystemCommand(char* cmd)
{
	if (bDebugMode) {
		printf("Running: %s", cmd);
	}
	else	{
		system(cmd);
	}
}

int Device::Process(char* function, char* json)
{
	int status		= 0;

	// parse the json
	jsonDoc.Parse(json);
	
	// call the private process function
	if ( jsonDoc.IsObject() )	{
		status 	= _Process(function);
	}


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



