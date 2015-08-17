#include <exp_led.h>

ExpLed::ExpLed(void)
{
	// populate pin array
	pins[EXP_LED_COLOR_ID_R]	= EXP_LED_COLOR_R_PIN_ID;
	pins[EXP_LED_COLOR_ID_G]	= EXP_LED_COLOR_R_PIN_ID;
	pins[EXP_LED_COLOR_ID_B]	= EXP_LED_COLOR_R_PIN_ID;
}

ExpLed::~ExpLed(void)
{
	// nothing for now
}

//// public functions
int ExpLed::_Process(char* function)
{
	int status		= 0;

	// check which function is to be used
	if ( strcmp(function, "set_color") == 0 )	{
		status = _FunctionSetColor();
	}
	else if ( strcmp(function, "set") == 0 )	{
		status = _FunctionSet();
	}
	else if (strcmp(function, "status") == 0 )	{
		status = _FunctionStatus();
	}


	return (status);
}


//// private functions 

int ExpLed::_FunctionSetColor (void)
{
	int status 	= 0;

	std::string	colorValue;
	std::string	ledValue;

	int			pinId;
	std::string	pinVal;
	char		*pinCmd;

	// Check that correct keys have been passed
	if ( 	jsonDoc.HasMember("value") && 
			jsonDoc.HasMember("color")
		)
	{
		// select the pin command based on the value
		if ( jsonDoc["value"].IsString() ) 	{
			pinVal	= EXP_LED_DISABLE_LED;
			if ( strcmp("true", jsonDoc["value"].GetString() ) == 0 )	{
				pinVal 	= EXP_LED_ENABLE_LED;
			}
		}
		else if ( jsonDoc["value"].IsBool() )	{
			pinVal	= ( jsonDoc["value"].GetBool() ? EXP_LED_ENABLE_LED : EXP_LED_DISABLE_LED);
		}

		// select the pin based on the color
		if ( jsonDoc["color"].IsString() )	{
			if( strcmp(EXP_LED_COLOR_R_STRING, jsonDoc["color"].GetString() ) == 0 )	{
				pinId 	= EXP_LED_COLOR_R_PIN_ID;
			}
			else if( strcmp(EXP_LED_COLOR_G_STRING, jsonDoc["color"].GetString() ) == 0 )	{
				pinId 	= EXP_LED_COLOR_G_PIN_ID;
			}
			else if( strcmp(EXP_LED_COLOR_B_STRING, jsonDoc["color"].GetString() ) == 0 )	{
				pinId 	= EXP_LED_COLOR_B_PIN_ID;
			}
		}


		// generate the system command
		pinCmd	= new char [1024];
		sprintf(pinCmd, "%s %s %d\n", EXP_LED_CTRL_CMD, pinVal.c_str(), pinId);

		// execute the command
		SystemCommand(pinCmd);

		delete[]	pinCmd;
	}
	else 
	{
		status 	= 1;
	}


	return (status);
}

int ExpLed::_FunctionSet (void)
{
	int status 	= 0;

	std::string	hexValue;

	// Check that correct key has been passed
	if ( jsonDoc.HasMember("value")  )	{
		

		//if (bVerboseMode) printf("FOUND value element, data is '%s'!\n", hexValue.c_str() );
	}


	return (status);
}


int ExpLed::_FunctionStatus (void)
{
	int status 	= 0;

	return (status);
}



