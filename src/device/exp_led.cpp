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
	int status		= EXIT_SUCCESS;

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

int ExpLed::_WriteGpio(int pin, int value)
{
	int status 	= EXIT_SUCCESS;

	if (verbosityLevel > 0)	printf("ExpLed: writing to pin %d, value %d\n", pin, value);

	// setup the gpio object based on the expled object settings 
	gpioObj.SetVerbosity(verbosityLevel);
	gpioObj.SetDebugMode(bDebugMode);

	// use the gpio class
	gpioObj.SetPinNumber(pin);
/*	if ((gpioObj._Init()) < 0)			return EXIT_FAILURE;
	if ((gpioObj._SetPin(value)) < 0)	return EXIT_FAILURE;
	if ((gpioObj._Exit()) < 0)			return EXIT_FAILURE;
*/
	return (status);
}

int ExpLed::_ReadGpio(int pin, int *value)
{
	int status 	= EXIT_SUCCESS;

	if (verbosityLevel > 0)	printf("ExpLed: reading pin %d\n", pin);

	// setup the gpio object based on the expled object settings 
	gpioObj.SetVerbosity(verbosityLevel);
	gpioObj.SetDebugMode(bDebugMode);

	// use the gpio class
	gpioObj.SetPinNumber(pin);
/*	if ((gpioObj._Init()) < 0)			return EXIT_FAILURE;
	if ((gpioObj._GetPin(value)) < 0)	return EXIT_FAILURE;
	if ((gpioObj._Exit()) < 0)			return EXIT_FAILURE;
*/
	return (status);
}
//// private functions 
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
	_GenerateOutJson(status);


	return (status);
}

int ExpLed::_FunctionStatus (void)
{
	int 	status 	= EXIT_SUCCESS;

	int 	colorVals[EXP_LED_COLOR_ID_NUM];

	const int 	pins[EXP_LED_COLOR_ID_NUM]	= 	{	
		EXP_LED_COLOR_R_PIN_ID,
		EXP_LED_COLOR_G_PIN_ID,
		EXP_LED_COLOR_B_PIN_ID
	};

	// read the GPIO values
	status |= 	_ReadGpio(pins[EXP_LED_COLOR_ID_R], &(colorVals[EXP_LED_COLOR_R_PIN_ID]) );
	status |= 	_ReadGpio(pins[EXP_LED_COLOR_ID_G], &(colorVals[EXP_LED_COLOR_G_PIN_ID]) );
	status |= 	_ReadGpio(pins[EXP_LED_COLOR_ID_B], &(colorVals[EXP_LED_COLOR_B_PIN_ID]) );


/*	// setup the json object
	jsonOut.SetObject();

	// write the values to the json
	_GenerateJsonMember( EXP_LED_COLOR_R_STRING, colorVals[EXP_LED_COLOR_R_PIN_ID] );
	_GenerateJsonMember( EXP_LED_COLOR_G_STRING, colorVals[EXP_LED_COLOR_G_PIN_ID] );
	_GenerateJsonMember( EXP_LED_COLOR_B_STRING, colorVals[EXP_LED_COLOR_B_PIN_ID] );

	// output the json object
	JsonPrint();*/


	return (status);
}


// json functions
void ExpLed::_GenerateOutJson (int inputStatus)
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

void ExpLed::_GenerateJsonMember(char* color, int value)
{
	rapidjson::Value 	element;

	// set the element value
	element.SetInt(value);

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



