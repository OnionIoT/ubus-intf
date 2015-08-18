#include <ubus_intf.h>

static const char * device_types[] = {
	"expled",
	"gpio",
};

int main(int argc, char* argv[])
{
	int status	= 0;

	char 	*device;
	char	*function;
	char	*json;

	rapidjson::Document jsonDoc;

	ExpLed 	expLedObj;


	//// initialization
	// allocate memory for the pointers
	device 		= new char[1024];
	function 	= new char[1024];
	json 		= new char[1024];

	// class initialization
	expLedObj.SetVerbosity(UBUS_INTF_VERBOSE);
	expLedObj.SetDebugMode(UBUS_INTF_DEBUG);
	
	// parse the command line arguments
	for (int i = 1; i < argc; i++) 	{
		if ( strcmp(argv[i], "-device") == 0 )	{
			// get the device name
			strcpy( device, argv[++i] );
			
			if (UBUS_INTF_VERBOSE) printf("parsing device: %s\n", device);
		}
		else if ( strcmp(argv[i], "-function") == 0 )	{
			// get the function name
			strcpy( function, argv[++i] );
			
			if (UBUS_INTF_VERBOSE) printf("parsing function: %s\n", function);
		}	
		else if ( strcmp(argv[i], "-json") == 0 )	{
			// get the json
			strcpy( json, argv[++i] );
			
			if (UBUS_INTF_VERBOSE) printf("parsing json: %s\n", json);
		}
	}

	// check device against list of existing devices
	/* TODO: make this cleaner */
	if (UBUS_INTF_VERBOSE) printf("Running process on ");
	if (strcmp( device, "expled") == 0)	{
		if (UBUS_INTF_VERBOSE) printf("expLed Object:\n\n");
		expLedObj.Process(function, json);
	}


	//// clean-up
	// free dynamically allocated memory
	delete[] device;
	delete[] function;
	delete[] json;


	return 0;
}