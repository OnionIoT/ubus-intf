#include <ubus_intf.h>


static const char * device_types[] = {
	"expled",
	"gpio",
};

int main(int argc, char* argv[])
{
	int status	= 0;

	char 	device 	[1024];
	char	function[1024];
	char	json	[1024];

	rapidjson::Document jsonDoc;
	
	// parse the command line arguments
	for (int i = 1; i < argc; i++) 	{
		if ( strcmp(argv[i], "-device") == 0 )	{
			// get the device name
			strcpy( device, argv[++i] );
			printf("parsing device: %s\n", device);
		}
		else if ( strcmp(argv[i], "-function") == 0 )	{
			// get the function name
			strcpy( function, argv[++i] );
			printf("parsing function: %s\n", function);
		}
		else if ( strcmp(argv[i], "-json") == 0 )	{
			// get the json
			strcpy( json, argv[++i] );
			printf("parsing json: %s\n", json);
		}
	}

	// check device against list of existing devices
	/* TODO */
	
	// parse the json
	jsonDoc.Parse(json);

	// loop through the json object
	if ( jsonDoc.IsObject() ) {
		printf("Looping through json object\n");
		for (rapidjson::Value::ConstMemberIterator itr = jsonDoc.MemberBegin(); itr != jsonDoc.MemberEnd(); ++itr)	{
	    	printf( "Value of member '%s' is '%s'\n", itr->name.GetString(), itr->value.GetString() );
	    }
	}

	return 0;
}