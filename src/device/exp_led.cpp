#include <device/exp_led.h>

ExpLed::ExpLed(void)
{
	// populate pin array
	pins[EXP_LED_COLOR_ID_R]	= EXP_LED_COLOR_R_PIN_ID;
	pins[EXP_LED_COLOR_ID_G]	= EXP_LED_COLOR_G_PIN_ID;
	pins[EXP_LED_COLOR_ID_B]	= EXP_LED_COLOR_B_PIN_ID;
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
	if ( strcmp(function, EXP_LED_FUNCTION_SET_COLOR) == 0 )	{
		status = _FunctionSetColor();
	}
	else if ( strcmp(function, EXP_LED_FUNCTION_SET) == 0 )	{
		status = _FunctionSet();
	}
	else if (strcmp(function, EXP_LED_FUNCTION_STATUS) == 0 )	{
		status = _FunctionStatus();
	}


	return (status);
}


//// private functions 
void ExpLed::_FunctionSetColorJson (int inputStatus)
{
	rapidjson::Value 	element;


	// setup the json object
	jsonOut.SetObject();

	// populate the value
	if (inputStatus == EXIT_SUCCESS)
	{
		element.SetString("true");
	}
	else {
		element.SetString("false");
	}

	// add element to the json object
	jsonOut.AddMember("success", element, jsonOut.GetAllocator() );

	// output the json object
	JsonPrint();
}

int ExpLed::_FunctionSet (void)
{
	int status 	= EXIT_SUCCESS;
	int colorVals[EXP_LED_COLOR_ID_NUM];

	const int 	pins[EXP_LED_COLOR_ID_NUM]	= 	{	
		EXP_LED_COLOR_R_PIN_ID,
		EXP_LED_COLOR_G_PIN_ID,
		EXP_LED_COLOR_B_PIN_ID
	};

	// Check that json has been parsed
	if ( !jsonDoc.IsObject() )	{
		return 1;
	}

	// JSON - find input values for the colors
	status |= JsonGetInt(EXP_LED_COLOR_R_STRING, &(colorVals[EXP_LED_COLOR_ID_R]) );
	status |= JsonGetInt(EXP_LED_COLOR_G_STRING, &(colorVals[EXP_LED_COLOR_ID_G]) );
	status |= JsonGetInt(EXP_LED_COLOR_B_STRING, &(colorVals[EXP_LED_COLOR_ID_B]) );

	// write to the gpios
	if (status == EXIT_SUCCESS) {
		status |= _WriteGpio(pins[EXP_LED_COLOR_ID_R], !colorVals[EXP_LED_COLOR_ID_R]);
		status |= _WriteGpio(pins[EXP_LED_COLOR_ID_G], !colorVals[EXP_LED_COLOR_ID_G]);
		status |= _WriteGpio(pins[EXP_LED_COLOR_ID_B], !colorVals[EXP_LED_COLOR_ID_B]);
	}

	// generate the json return
	_FunctionSetColorJson(status);


	return (status);
}

int ExpLed::_WriteGpio(int pin, int value)
{
	int status 	= EXIT_SUCCESS;

	// setup the gpio object based on the expled object settings 
	gpioObj.SetVerbosity(verbosityLevel);
	gpioObj.SetDebugMode(true);

	// use the gpio class
	gpioObj.SetPinNumber(pin);
	if ((gpioObj.Init()) < 0)			return EXIT_FAILURE;
	if ((gpioObj.SetPin(value)) < 0)	return EXIT_FAILURE;
	if ((gpioObj.Exit()) < 0)			return EXIT_FAILURE;

	return (status);
}

int ExpLed::_FunctionStatus (void)
{
	int 	status 	= 0;

	bool 	ledValue[EXP_LED_COLOR_ID_NUM];

	char*	pinCmd		= new char[1024];
	char*	pinValue	= new char[1024];

	/* TODO: move these to be available globally */
	const int 	pins[EXP_LED_COLOR_ID_NUM]	= 	{	
		EXP_LED_COLOR_R_PIN_ID,
		EXP_LED_COLOR_G_PIN_ID,
		EXP_LED_COLOR_B_PIN_ID
	};

	const char	*colors[EXP_LED_COLOR_ID_NUM]	= 	{
		EXP_LED_COLOR_R_STRING,
		EXP_LED_COLOR_G_STRING,
		EXP_LED_COLOR_B_STRING
	};



	// setup the json object
	jsonOut.SetObject();

	// loop through each of the colors
	for (int i = 0; i < EXP_LED_COLOR_ID_NUM; i++)	{
		// empty the text
		memset(&pinCmd[0], 0, sizeof(pinCmd));
		memset(&pinValue[0], 0, sizeof(pinValue));

		// generate the gpio read command
		sprintf	(pinCmd, "%s %s %d | %s | %s\n", 	
					EXP_LED_CTRL_CMD, 
					EXP_LED_CMD_GET_VALUE, 
					pins[i], 
					EXP_LED_CTRL_CMD_FIND_VAL,
					EXP_LED_CTRL_CMD_ISO_VAL
				);

		// run the command and get the response
		if (bDebugMode)		SetDebugSystemCommandResp("LOW");
		SystemCommandRead	(pinCmd, pinValue);

		if (verbosityLevel > 0)	printf("GPIO response is '%s'\n", pinValue);

		// parse the command
		ledValue[i]	= false;
		if ( strcmp(pinValue, EXP_LED_CMD_ON_VALUE) == 0 )	{
			ledValue[i]	= true;
		}
		else if ( strcmp(pinValue, EXP_LED_CMD_ON_VALUE) == 0 )	{
			ledValue[i]	= false;
		}

		// add this result to the json object
		_FunctionStatusJson( colors[i], ledValue[i] );
	}

	// output the json object
	JsonPrint();


	// clean-up
	delete[]	pinCmd;
	delete[]	pinValue;

	return (status);
}

void ExpLed::_FunctionStatusJson(const char* color, bool value)
{
	rapidjson::Value 	element;

	// set the element value
	element.SetBool(value);

	// add element to the json object
	jsonOut.AddMember	(	rapidjson::Value(color, jsonOut.GetAllocator()).Move(), 
							element, 
							jsonOut.GetAllocator() 
						);
}

int ExpLed::_FunctionSetColor (void)
{
	int status 	= 0;

	/*std::string	colorValue;
	std::string	ledValue;

	int			pinId;
	std::string	pinVal;
	char		*pinCmd;


	// Check that json has been parsed
	if ( !jsonDoc.IsObject() )	{
		return 1;
	}


	// Check that correct keys have been passed
	if ( 	jsonDoc.HasMember("value") && 
			jsonDoc.HasMember("color")
		)
	{
		// select the pin command based on the value
		if (verbosityLevel > 0) printf("Found value element: '%s', ", (JsonReadBool("value") ? "true" : "false") );
		pinVal	= ( JsonReadBool("value") ? EXP_LED_CMD_ENABLE_LED : EXP_LED_CMD_DISABLE_LED);

		// select the pin based on the color
		if ( jsonDoc["color"].IsString() )	{
			if (verbosityLevel > 0) printf("found color element: '%s' \n", jsonDoc["color"].GetString() );

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
		else {
			status = 1;
		}



		// make the system call
		if (status == 0)	{
			// generate the command
			pinCmd	= new char [1024];
			sprintf(pinCmd, "%s %s %d\n", EXP_LED_CTRL_CMD, pinVal.c_str(), pinId);

			// execute the command
			SystemCommandExec(pinCmd);

			delete[]	pinCmd;
		}
	}
	else 
	{
		status 	= 1;
	}

	// generate the json return
	_FunctionSetColorJson(status);*/


	return (status);
}



