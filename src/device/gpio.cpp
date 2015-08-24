#include <device/gpio.h>

Gpio::Gpio(void)
{
	Reset();
}

Gpio::~Gpio(void)
{
	// nothing for now
}

void Gpio::Reset(void)
{
	// setup default values
	gpioPin 	= 0;
	bRequest	= 0;
	bActiveLow	= 0;
}

//// public functions
int Gpio::SetPinNumber(int pinNum)
{
	gpioPin 	= pinNum;

	return (EXIT_SUCCESS);
}

int Gpio::Write(int pinNum, int value)
{
	int status 	= EXIT_SUCCESS;

	if (verbosityLevel > 0)	printf("Gpio: writing to pin %d, value %d\n", pinNum, value);

	// set the pin number
	SetPinNumber(pinNum);

	// initialize the pin
	if ((_Init()) < 0)			return EXIT_FAILURE;

	// write to the pin
	if ((_SetPin(value)) < 0)	return EXIT_FAILURE;

	// release the pin
	if ((_Exit()) < 0)			return EXIT_FAILURE;


	return (status);
}

int Gpio::Read(int pinNum, int &value)
{
	int status 	= EXIT_SUCCESS;

	if (verbosityLevel > 0)	printf("Gpio: reading pin %d\n", pinNum);

	// set the pin number
	SetPinNumber(pinNum);

	// initialize the pin
	if ((_Init()) < 0)			return EXIT_FAILURE;

	// read the pin
	if ((_GetPin(value)) < 0)	return EXIT_FAILURE;

	// release the pin
	if ((_Exit()) < 0)			return EXIT_FAILURE;


	return (status);
}


bool Gpio::GetActiveLow(void)
{
	_GetActiveLow();
	return (bActiveLow);
}

int Gpio::SetActiveLow(bool activeLow)
{
	return ( _SetActiveLow(activeLow) );
}


bool Gpio::GetDirectionInput(int pinNum)
{
	int bInputDir;

	// set the pin number
	SetPinNumber(pinNum);

	// find the pin direction
	_GetDirection(bInputDir);

	// return the direction: 0 - output; 1 - input
	return ((bool)bInputDir);
}

int Gpio::SetDirection(int pinNum, bool bInputDir)
{
	// set the pin number
	SetPinNumber(pinNum);

	// set the direction
	return ( _SetDirection((int)bInputDir)  );
}


/* private functions */

// _Process function
// Basic program flow:
//	- Find Pin Number from JSON
//	- Initialize
//		- check that pin is not in use
// 		- reserve the pin
//		- find if active-low
//	- Perform specified action
//	- Release the pin
int Gpio::_Process(char* function)
{
	int status		= EXIT_SUCCESS;


	// Check that json has been parsed
	if ( !jsonDoc.IsObject() )	{
		return EXIT_FAILURE;
	}

	// find the pin id
	if (JsonGetInt("pin", &gpioPin) < 0)	{
		if (verbosityLevel > 0) printf("invalid_gpio_id\n");
		return EXIT_FAILURE;
	}
	if (verbosityLevel > 0) printf("Using pin '%d'\n", gpioPin);


	// check which function is to be used
	if (status == EXIT_SUCCESS)	{
		if ( strcmp(function, GPIO_FUNCTION_SET) == 0 )	{
			status = _FunctionSet();
		}
		else if ( strcmp(function, GPIO_FUNCTION_CLEAR) == 0 )	{
			status = _FunctionClear();
		}
		else if (strcmp(function, GPIO_FUNCTION_SET_PIN) == 0 )	{
			status = _FunctionSetValue();
		}
		else if (strcmp(function, GPIO_FUNCTION_GET) == 0 )	{
			status = _FunctionGet();
		}
		else if (strcmp(function, GPIO_FUNCTION_GET_AL) == 0 )	{
			status = _FunctionGetActiveLow();
		}
		else if (strcmp(function, GPIO_FUNCTION_SET_AL) == 0 )	{
			status = _FunctionSetActiveLow();
		}
		else if (strcmp(function, GPIO_FUNCTION_GET_DIR) == 0 )	{
			status = _FunctionGetDirection();
		}
		else if (strcmp(function, GPIO_FUNCTION_SET_DIR) == 0 )	{
			status = _FunctionSetDirection();
		}
		else if (strcmp(function, GPIO_FUNCTION_STATUS) == 0 )	{
			status = _FunctionStatus();
		}
	}


	return (status);
}


//// functions to be called by _Process
// set the pin to logical 1
// 	if active-high:	pin HIGH
// 	if active-low:	pin LOW
int Gpio::_FunctionSet(void)
{
	int status 		= EXIT_SUCCESS;

	// set the pin
	status 	= Write(gpioPin, 1);

	// generate the json return
	_GenerateJsonSuccess(status);

	return (status);
}

// set the pin to logical 0
// 	if active-high:	pin LOW
// 	if active-low:	pin HIGH
int Gpio::_FunctionClear(void)
{
	int status 		= 0;

	// set the pin
	status 	= Write(gpioPin, 0);

	// generate the json return
	_GenerateJsonSuccess(status);

	return (status);
}

// set the pin to the json value 
// 	if active-high:	0 = pin LOW; 	1 = pin HIGH
// 	if active-low:	0 = pin HIGH; 	1 = pin LOW
int Gpio::_FunctionSetValue(void)
{
	int status 		= EXIT_SUCCESS;
	int value;

	// json - find the value
	if ((status = JsonGetInt("value", &value)) < 0)	{
		if (verbosityLevel > 0) printf("invalid json int for value\n");
		return EXIT_FAILURE;
	}

	// set the pin
	status 		= Write(gpioPin, value);

	// generate the json return
	_GenerateJsonSuccess(status);

	return (status);
}

int Gpio::_FunctionGet(void)
{
	int status 		= EXIT_SUCCESS;
	int value;

	status 	= Read(gpioPin, value);

	// generate the output json
	_GenerateJsonValue(value);

	return (status);
}

int Gpio::_FunctionGetActiveLow(void)
{
	int status 		= EXIT_SUCCESS;
	int value;

	// read the active low value
	_GetActiveLow();

	// generate the output json
	_GenerateJsonActiveLow();

	return (status);
}

int Gpio::_FunctionSetActiveLow(void)
{
	int 	status 		= EXIT_SUCCESS;
	bool 	activeLow;

	// json - find the activelow setting
	if ((status = JsonGetBool("activelow", &activeLow)) < 0)	{
		if (verbosityLevel > 0) printf("invalid json bool for activelow\n");
		return EXIT_FAILURE;
	}

	// set the active low value
	status = _SetActiveLow(activeLow);

	// generate the output json
	_GenerateJsonSuccess(status);

	return (status);
}

int Gpio::_FunctionGetDirection(void)
{
	int status 		= EXIT_SUCCESS;
	int bInputDirection;

	// read the pin direction
	_GetDirection(bInputDirection);

	// generate the output json
	_GenerateJsonDirection(bInputDirection);

	return (status);
}

int Gpio::_FunctionSetDirection(void)
{
	int 	status 			= EXIT_SUCCESS;
	int 	bInputDirection	= 0;

	std::string 	direction;

	// json - find the direction
	if ( jsonDoc.HasMember( "direction" )) {
		direction 	= jsonDoc["direction"].GetString();
	}

	if ( strcmp(direction.c_str(), "input") == 0) {
		bInputDirection 	= 1;
	}
	else if ( strcmp(direction.c_str(), "output") == 0) {
		bInputDirection 	= 0;
	}

	// set the active low value
	status = _SetDirection(bInputDirection);

	// generate the output json
	_GenerateJsonSuccess(status);

	return (status);
}

int Gpio::_FunctionStatus(void)
{
	int status 		= EXIT_SUCCESS;
	int value;
	int bInputDir;

	// read the pin
	status 	|= Read(gpioPin, value);

	// read the pin direction
	status	|= _GetDirection(bInputDir);

	// read the active-low setting
	status 	|= _GetActiveLow();


	// output to json
	if (status == EXIT_SUCCESS) {
		jsonOut.SetObject();

		_GenerateJsonPinId();
		_GenerateJsonValue(value, false);
		_GenerateJsonDirection(bInputDir, false);
		_GenerateJsonActiveLow(false);

		JsonPrint();
	}
	else {
		_GenerateJsonSuccess(status);
	}


	return (status);
}


//// json functions
void Gpio::_GenerateJsonPinId(void)
{
	rapidjson::Value 	element;

	//// set the pin number
	// set the element value
	element.SetInt(gpioPin);

	// add element to the json object
	jsonOut.AddMember	(	rapidjson::Value("pin", jsonOut.GetAllocator()).Move(), 
							element, 
							jsonOut.GetAllocator() 
						);
}

void Gpio::_GenerateJsonSuccess(int inputStatus, bool bPrintObject)
{
	rapidjson::Value 	element;


	// setup the json object
	if (bPrintObject) {
		jsonOut.SetObject();
	}

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
	if (bPrintObject) {
		JsonPrint();
	}
}

void Gpio::_GenerateJsonValue(int logicalValue, bool bPrintObject)
{
	rapidjson::Value 	element;

	// setup the json object
	if (bPrintObject) {
		jsonOut.SetObject();

		//// set the pin number
		_GenerateJsonPinId();
	}	

	//// set the pin value
	// set the element value
	element.SetInt(logicalValue);

	// add element to the json object
	jsonOut.AddMember	(	rapidjson::Value("value", jsonOut.GetAllocator()).Move(), 
							element, 
							jsonOut.GetAllocator() 
						);


	// output the json object
	if (bPrintObject) {
		JsonPrint();
	}
}

void Gpio::_GenerateJsonActiveLow(bool bPrintObject)
{
	rapidjson::Value 	element;

	// setup the json object
	if (bPrintObject) {
		jsonOut.SetObject();

		//// set the pin number
		_GenerateJsonPinId();
	}

	//// set the activelow value
	// set the element value
	element.SetBool((bool)bActiveLow);

	// add element to the json object
	jsonOut.AddMember	(	rapidjson::Value("activelow", jsonOut.GetAllocator()).Move(), 
							element, 
							jsonOut.GetAllocator() 
						);


	// output the json object
	if (bPrintObject) {
		JsonPrint();
	}
}

void Gpio::_GenerateJsonDirection(int bInputDir, bool bPrintObject)
{
	rapidjson::Value 	element;


	// setup the json object
	if (bPrintObject) {
		jsonOut.SetObject();

		//// set the pin number
		_GenerateJsonPinId();
	}

	//// set the direction string
	_GenerateJsonDirectionString(bInputDir);

	//// set the input direction boolean
	_GenerateJsonDirectionBool(bInputDir);


	// output the json object
	if (bPrintObject) {
		JsonPrint();
	}
}

void Gpio::_GenerateJsonDirectionString(int bInputDir)
{
	rapidjson::Value 	element;
	char*				directionText	= new char[1024];

	//// set the direction string
	if (bInputDir) {
		strcpy(directionText, "input");
	}
	else {
		strcpy(directionText, "output");
	}

	// set the element value
	element.SetString	(	directionText,
							strlen(directionText), 
							jsonOut.GetAllocator()
						);

	// add element to the json object
	jsonOut.AddMember	(	rapidjson::Value("direction", jsonOut.GetAllocator()).Move(), 
							element, 
							jsonOut.GetAllocator() 
						);

	delete[] 	directionText;
}

void Gpio::_GenerateJsonDirectionBool(int bInputDir)
{
	rapidjson::Value 	element;

	//// set the direction boolean
	// set the element value
	element.SetBool	( (bInputDir == 1 ? true : false) );

	// add element to the json object
	jsonOut.AddMember	(	rapidjson::Value("input", jsonOut.GetAllocator()).Move(), 
							element, 
							jsonOut.GetAllocator() 
						);
}


//// gpio helper functions
int Gpio::_Init(void)
{
	int status 	= EXIT_SUCCESS;

	if (!bDebugMode) 	{
		// check that gpio is free
		if ((bRequest = gpio_is_requested(gpioPin)) < 0)
		{
			if (verbosityLevel > 0) printf("gpio_is_requested\n");
			return EXIT_FAILURE;
		}

		// request the pin
		if (!bRequest) {
			if ((gpio_request(gpioPin, NULL)) < 0)
			{
				if (verbosityLevel > 0) printf("gpio_request");
				return EXIT_FAILURE;
			}
		}	
	}

	// find if pin is active-low
	status = _GetActiveLow();

	return (status);
}

int Gpio::_Exit(void)
{
	// release the pin
	if (!bRequest && !bDebugMode) {
		if (gpio_free(gpioPin) < 0)
		{
			if (verbosityLevel > 0) printf("gpio_free");
		}
	}

	return (EXIT_SUCCESS);
}

int Gpio::_SetPin(int value, bool bLogicalVaue)
{
	int status 		= EXIT_SUCCESS;

	// adjust for active-low pins
	if (bLogicalVaue) {
		if (verbosityLevel > 0) printf("Converting value to logical value, active-low is '%d'\n", bActiveLow);
		value = bActiveLow ? !value : value;
	}

	if (verbosityLevel > 0) printf("Setting GPIO ID '%d' to '%d'\n", gpioPin, value);

	// convert value to GPIO readable
	value = (value ? GPIOF_OUT_INIT_HIGH : GPIOF_OUT_INIT_LOW);

	// write the value to the pin	
	if (!bDebugMode) {
		if ((status = gpio_direction_output(gpioPin, value)) < 0) {
			if (verbosityLevel > 0) printf("gpio_direction_output");
		}
	}

	return (status);
}

int Gpio::_GetPin(int &value, bool bLogicalVaue)
{
	int status 		= EXIT_SUCCESS;

	// read the pin value
	if (!bDebugMode) {
		if ((value = gpio_get_value(gpioPin)) < 0) {
			if (verbosityLevel > 0) printf("gpio_get_value");
			status = EXIT_FAILURE;
		}
	}
	else {
		value = 1;
	}

	// take active-low into account
	//		al 	pin 	value
	//		0	0		0
	//		0	1		1
	//		1	0		1
	//		1	1		0
	// => logical value = al ^ pin
	if (bLogicalVaue) {
		if (verbosityLevel > 0) printf("Converting value to logical value, active-low is '%d'\n", bActiveLow);
		value 	= (bActiveLow ^ value);
	}

	//if (verbosityLevel > 0) printf("Read GPIO ID '%d': '%d'\n", gpioPin, rdVal);


	return (status);
}


int Gpio::_GetActiveLow(void)
{
	// find if pin is active-low
	if (!bDebugMode) {
		if ((bActiveLow = gpio_get_activelow(gpioPin)) < 0)
		{
			if (verbosityLevel > 0) printf("gpio_get_activelow");
			return EXIT_FAILURE;
		}
	}
	else {		
		bActiveLow = false;
		if (verbosityLevel > 0) printf("Active-low is '%s'\n", (bActiveLow ? "true" : "false") );
	}

	return EXIT_SUCCESS;
}

int Gpio::_SetActiveLow(bool activeLow)
{
	int activeLowInt	= (activeLow ? 1 : 0);

	// program pin's active-low setting
	if (verbosityLevel > 0) printf("Setting GPIO ID '%d' to active-low '%d'\n", gpioPin, activeLowInt);
	if (!bDebugMode) {
		if ((gpio_set_activelow(gpioPin, activeLowInt)) < 0)
		{
			if (verbosityLevel > 0) printf("gpio_set_activelow");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}


int Gpio::_GetDirection(int &bInputDir)
{
	int returnVal;

	// find pin direction
	if (!bDebugMode) {
		if ((returnVal = gpio_get_direction(gpioPin)) < 0)
		{
			if (verbosityLevel > 0) printf("gpio_get_activelow");
			return EXIT_FAILURE;
		}

		// convert the function output
		bInputDir 	= (returnVal == GPIOF_DIR_IN ? 1 : 0);
	}
	else {		
		bInputDir = 0;
		if (verbosityLevel > 0) printf("Input direction is '%s'\n", (bInputDir ? "true" : "false") );
	}

	return EXIT_SUCCESS;
}

int Gpio::_SetDirection(int bInputDir, int initValue)
{
	int returnVal;

	// program pin's direction setting
	if (verbosityLevel > 0) printf("Setting GPIO ID '%d' to input direction '%d'\n", gpioPin, bInputDir);
	
	if (!bDebugMode) {
		if (bInputDir) {
			returnVal 	= gpio_direction_input(gpioPin);
		}
		else {
			returnVal 	= gpio_direction_output(gpioPin, (initValue == 1 ? GPIOF_OUT_INIT_HIGH : GPIOF_OUT_INIT_LOW) );
		}

		if (returnVal < 0)
		{
			if (verbosityLevel > 0) printf("gpio_direction_[input/output]");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}




