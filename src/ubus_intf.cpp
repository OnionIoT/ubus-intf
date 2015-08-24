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
	Gpio 	gpioObj;

	int 	verbosity 		= UBUS_INTF_VERBOSE;
	int 	debugLevel		= UBUS_INTF_DEBUG;


	//// initialization
	// allocate memory for the pointers
	device 		= new char[1024];
	function 	= new char[1024];
	json 		= new char[1024];

	
	
	// parse the command line arguments
	if (UBUS_INTF_VERBOSE) printf("Parsing arguments:\n");
	for (int i = 1; i < argc; i++) 	{
		if ( strcmp(argv[i], "-device") == 0 )	{
			// get the device name
			strcpy( device, argv[++i] );
			
			if (UBUS_INTF_VERBOSE) printf("\tparsing device: %s\n", device);
		}
		else if ( strcmp(argv[i], "-function") == 0 )	{
			// get the function name
			strcpy( function, argv[++i] );
			
			if (UBUS_INTF_VERBOSE) printf("\tparsing function: %s\n", function);
		}	
		else if ( strcmp(argv[i], "-json") == 0 )	{
			// get the json
			strcpy( json, argv[++i] );
			
			if (UBUS_INTF_VERBOSE) printf("\tparsing json: %s\n", json);
		}
		else if ( strcmp(argv[i], "-verbose") == 0 )	{
			// change the verbosity setting
			verbosity 	= 1;
		}
		else if ( strcmp(argv[i], "-debug") == 0 )	{
			// change the debug setting
			debugLevel 	= 1;
		}
	}


	// class initialization
	expLedObj.SetVerbosity(verbosity);
	expLedObj.SetDebugMode(debugLevel);

	gpioObj.SetVerbosity(verbosity);
	gpioObj.SetDebugMode(debugLevel);


	// check device against list of existing devices
	/* TODO: make this cleaner */
	if (UBUS_INTF_VERBOSE) printf("Running process on ");
	if (strcmp( device, "expled") == 0)	{
		if (UBUS_INTF_VERBOSE) printf("expLed Object:\n\n");
		expLedObj.Process(function, json);
	}
	else if (strcmp( device, "gpio") == 0)	{
		if (UBUS_INTF_VERBOSE) printf("gpio Object:\n\n");
		gpioObj.Process(function, json);
	}


	//// clean-up
	// free dynamically allocated memory
	delete[] device;
	delete[] function;
	delete[] json;


	return 0;
}
