#include <exp_led.h>

ExpLed::ExpLed(void)
{
	// populate pin array
	pins[EXP_LED_COLOR_ID_R]	= EXP_LED_COLOR_R_PIN_ID;
	pins[EXP_LED_COLOR_ID_G]	= EXP_LED_COLOR_R_PIN_ID;
	pins[EXP_LED_COLOR_ID_B]	= EXP_LED_COLOR_R_PIN_ID;

	// enable verbosity for now
	jsonHelperObj.SetVerbosity(true);
}

ExpLed::~ExpLed(void)
{
	// nothing for now
}

//// public functions
int ExpLed::Process(char* function, char* json)
{
	int status		= 0;

	// parse the json
	jsonHelperObj.Read(json);
	
	// check which function is to be used
	if ( strcmp(function, "set_color") == 0 )	{
		status = FunctionSetColor();
	}
	else if ( strcmp(function, "set") == 0 )	{
		status = FunctionSet();
	}
	else if (strcmp(function, "status") == 0 )	{
		status = FunctionStatus();
	}


	return (status);
}


//// private functions 

int ExpLed::FunctionSetColor (void)
{
	int status 	= 0;

	std::string	colorValue;
	std::string	ledValue;

	int			pinId;
	std::string	pinVal;
	char		*pinCmd;

	// Check that correct keys have been passed
	if ( 	jsonHelperObj.FindElement("value") &&
			jsonHelperObj.FindElement("color")
		)	
	{
		// read the json values
		colorValue	= jsonHelperObj.GetElement("color");
		ledValue	= jsonHelperObj.GetElement("value");

		if (bVerboseMode) printf("FOUND color (%s) and value (%s) elements!\n", colorValue.c_str(), ledValue.c_str());

		// select the pin based on the color
		if( strcmp(colorValue.c_str(), "red") == 0 )	{
			pinId 	= EXP_LED_COLOR_R_PIN_ID;
		}
		else if( strcmp(colorValue.c_str(), "green") == 0 )	{
			pinId 	= EXP_LED_COLOR_G_PIN_ID;
		}
		else if( strcmp(colorValue.c_str(), "blue") == 0 )	{
			pinId 	= EXP_LED_COLOR_B_PIN_ID;
		}

		// select the pin command based on the value
		pinVal	= EXP_LED_DISABLE_LED;
		if ( strcmp(ledValue.c_str(), "true") == 0 )	{
			pinVal 	= EXP_LED_ENABLE_LED;
		}

		// generate the system command
		pinCmd	= new char [1024];
		sprintf(pinCmd, "%s %s %d\n", EXP_LED_CTRL_CMD, pinVal.c_str(), pinId);

		// execute the command
		SystemCommand(pinCmd);

		delete[]	pinCmd;
	}

	return (status);
}

int ExpLed::FunctionSet (void)
{
	int status 	= 0;

	std::string	hexValue;

	// Check that correct key has been passed
	if ( jsonHelperObj.FindElement("value") )	{
		hexValue 	= jsonHelperObj.GetElement("value");

		if (bVerboseMode) printf("FOUND value element, data is '%s'!\n", hexValue.c_str() );
	}


	return (status);
}


int ExpLed::FunctionStatus (void)
{
	int status 	= 0;

	return (status);
}



