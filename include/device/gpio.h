#ifndef _GPIO_H_
#define _GPIO_H_

#include <device/device.h>

#include <libugpio/ugpio.h>

#define		GPIO_FUNCTION_SET			"set"
#define		GPIO_FUNCTION_CLEAR			"clear"
#define		GPIO_FUNCTION_SET_PIN		"set_pin"
#define		GPIO_FUNCTION_GET 			"get"
#define		GPIO_FUNCTION_GET_AL		"get_activelow"
#define		GPIO_FUNCTION_SET_AL		"set_activelow"
#define		GPIO_FUNCTION_STATUS		"status"

class Gpio : public Device {
public:
	Gpio(void);
	~Gpio(void);

	void 	Reset 			(void);

	int 	Init			(void);
	int 	Exit 			(void);

	int 	SetPinNumber 	(int pinNum);
	int 	SetPin 			(int value, bool bLogicalVaue = true);
	int 	GetPin 			(int *value, bool bLogicalVaue = true);

	bool 	GetActiveLow	(void);
	int 	SetActiveLow	(bool activeLow);

private:
	// private functions
	int 	_Process				(char* function);

	int 	_FunctionSet			(void);
	int		_FunctionClear			(void);
	int 	_FunctionSetValue		(void);

	int		_FunctionGet			(void);

	int		_FunctionGetActiveLow	(void);
	int		_FunctionSetActiveLow	(void);

	int 	_FunctionStatus			(void);


	void 	_GenerateJsonPinId			(void);
	void 	_GenerateGetJson 			(int logicalValue);
	void 	_GenerateGetActiveLowJson 	(void);
	void 	_GenerateJsonOut 			(int inputStatus);

	// helper functions
	int 	_GetActiveLow			(void);
	int 	_SetActiveLow			(bool activeLow);


	// private members
	int 	gpioPin;
	int 	bRequest;
	int 	bActiveLow;
};


#endif	// _GPIO_H_