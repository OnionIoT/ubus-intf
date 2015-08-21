#include <device/exp_led.h>

ExpLed::ExpLed(void)
{
	// default values
	Reset();
}

ExpLed::~ExpLed(void)
{
	// nothing for now
}

void ExpLed::Reset(void)
{
	// populate pin array
	pins[EXP_LED_COLOR_ID_R]	= EXP_LED_COLOR_R_PIN_ID;
	pins[EXP_LED_COLOR_ID_G]	= EXP_LED_COLOR_G_PIN_ID;
	pins[EXP_LED_COLOR_ID_B]	= EXP_LED_COLOR_B_PIN_ID;
}


//// public functions
int ExpLed::_Process(char* function)
{
	int status		= EXIT_SUCCESS;

	// setup the gpio object based on the expled object settings 
	gpioObj.SetVerbosity(verbosityLevel);
	gpioObj.SetDebugMode(bDebugMode);


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
int ExpLed::_FunctionSet (void)
{
	int status 	= EXIT_SUCCESS;
	int colorVals[EXP_LED_COLOR_ID_NUM];

	// Check that json has been parsed
	if ( !jsonDoc.IsObject() )	{
		return 1;
	}


	// JSON - find input values for the colors
	status |= JsonGetInt(EXP_LED_COLOR_R_STRING, &(colorVals[EXP_LED_COLOR_ID_R]) );
	status |= JsonGetInt(EXP_LED_COLOR_G_STRING, &(colorVals[EXP_LED_COLOR_ID_G]) );
	status |= JsonGetInt(EXP_LED_COLOR_B_STRING, &(colorVals[EXP_LED_COLOR_ID_B]) );
	if (verbosityLevel > 0) printf("Setting RGB to %d%d%d\n", colorVals[EXP_LED_COLOR_ID_R], colorVals[EXP_LED_COLOR_ID_G], colorVals[EXP_LED_COLOR_ID_B] );

	// write to the gpios
	if (status == EXIT_SUCCESS) {
		status |= gpioObj.Write(pins[EXP_LED_COLOR_ID_R], !colorVals[EXP_LED_COLOR_ID_R]);
		status |= gpioObj.Write(pins[EXP_LED_COLOR_ID_G], !colorVals[EXP_LED_COLOR_ID_G]);
		status |= gpioObj.Write(pins[EXP_LED_COLOR_ID_B], !colorVals[EXP_LED_COLOR_ID_B]);
	}

	// generate the json return
	_GenerateOutJson(status);


	return (status);
}

int ExpLed::_FunctionStatus (void)
{
	int 	status 	= EXIT_SUCCESS;

	int 	colorVals[EXP_LED_COLOR_ID_NUM];


	// read the GPIO values
	status |= 	gpioObj.Read(pins[EXP_LED_COLOR_ID_R], colorVals[EXP_LED_COLOR_ID_R] );
	status |= 	gpioObj.Read(pins[EXP_LED_COLOR_ID_G], colorVals[EXP_LED_COLOR_ID_G] );
	status |= 	gpioObj.Read(pins[EXP_LED_COLOR_ID_B], colorVals[EXP_LED_COLOR_ID_B] );


	// setup the json object
	jsonOut.SetObject();

	// write the values to the json
	_GenerateJsonMember( EXP_LED_COLOR_R_STRING, !colorVals[EXP_LED_COLOR_ID_R] );
	_GenerateJsonMember( EXP_LED_COLOR_G_STRING, !colorVals[EXP_LED_COLOR_ID_G] );
	_GenerateJsonMember( EXP_LED_COLOR_B_STRING, !colorVals[EXP_LED_COLOR_ID_B] );

	// output the json object
	JsonPrint();


	return (status);
}

int ExpLed::_FunctionSetColor (void)
{
	int 	status 	= EXIT_SUCCESS;
	int 	pinNum;
	int 	colorValue;

	char* 	colorName = new char[1024];


	// Check that json has been parsed
	if ( !jsonDoc.IsObject() )	{
		return 1;
	}


	// JSON - find the color being used
	/* TO DO: change this to detect JSON{"color":"red","value":1} */
	if ( jsonDoc.HasMember(EXP_LED_COLOR_R_STRING) ) {
		strcpy(colorName, EXP_LED_COLOR_R_STRING);
		pinNum		= EXP_LED_COLOR_R_PIN_ID;
	}
	else if ( jsonDoc.HasMember(EXP_LED_COLOR_G_STRING) )	{
		strcpy(colorName, EXP_LED_COLOR_G_STRING);
		pinNum		= EXP_LED_COLOR_G_PIN_ID;
	}
	else if ( jsonDoc.HasMember(EXP_LED_COLOR_B_STRING) )	{
		strcpy(colorName, EXP_LED_COLOR_B_STRING);
		pinNum		= EXP_LED_COLOR_B_PIN_ID;
	}
	else {
		status = EXIT_FAILURE;
	}

	// JSON - find the value of the color
	status |= JsonGetInt(colorName, &colorValue );

	// write to the gpio
	if (status == EXIT_SUCCESS) {
		status |= gpioObj.Write(pinNum, !colorValue );
	}

	// generate the json return
	_GenerateOutJson(status);


	return (status);
}


//// json functions
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




