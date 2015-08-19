#ifndef _GPIO_H_
#define _GPIO_H_

#include <device/device.h>

#include <libugpio/ugpio.h>

#define		GPIO_FUNCTION_SET			"set"
#define		GPIO_FUNCTION_CLEAR			"clear"
#define		GPIO_FUNCTION_SET_PIN		"set_pin"
#define		GPIO_FUNCTION_GET 			"get"
#define		GPIO_FUNCTION_STATUS		"status"

class Gpio : public Device {
public:
	Gpio(void);
	~Gpio(void);

	int 	Init			(void);
	int 	SetPin 			(int value, bool bLogicalVaue = true);
	int 	Exit 			(void);

private:
	// private functions
	int 	_Process			(char* function);

	int 	_FunctionSet		(void);
	int		_FunctionClear		(void);
	int 	_FunctionSetValue	(void);

	int		_FunctionGet		(void);

	int 	_FunctionStatus		(void);


	void 	_GenerateGetJson 	(int logicalValue);
	void 	_GenerateJsonOut 	(int inputStatus);


	// private members
	int 	gpioPin;
	int 	bRequest;
	int 	bActiveLow;
};


#endif	// _GPIO_H_