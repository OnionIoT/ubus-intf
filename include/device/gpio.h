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
#define		GPIO_FUNCTION_GET_DIR		"get_direction"
#define		GPIO_FUNCTION_SET_DIR		"set_direction"
#define		GPIO_FUNCTION_STATUS		"status"

class Gpio : public Device {
public:
	Gpio(void);
	~Gpio(void);

	void 	Reset 			(void);

	int 	SetPinNumber 	(int pinNum);

	int 	Write 			(int pinNum, int value);
	int 	Read 			(int pinNum, int &value);
	

	bool 	GetActiveLow	(void);
	int 	SetActiveLow	(bool activeLow);

	bool 	GetDirectionInput	(int pinNum);
	int 	SetDirection 		(int pinNum, bool bInputDir);

private:
	//// private functions
	int 	_Process				(char* function);

	int 	_FunctionSet			(void);
	int		_FunctionClear			(void);
	int 	_FunctionSetValue		(void);

	int		_FunctionGet			(void);

	int		_FunctionGetActiveLow	(void);
	int		_FunctionSetActiveLow	(void);

	int		_FunctionGetDirection	(void);
	int		_FunctionSetDirection	(void);	

	int 	_FunctionStatus			(void);

	// json functions
	void 	_GenerateJsonPinId				(void);
	void 	_GenerateJsonValue 				(int logicalValue, bool bPrintObject = true);
	void 	_GenerateJsonActiveLow	 		(bool bPrintObject = true);
	void 	_GenerateJsonDirection 			(int bInputDir, bool bPrintObject = true);
	void 	_GenerateJsonDirectionString	(int bInputDir);
	void 	_GenerateJsonDirectionBool 		(int bInputDir);
	void 	_GenerateJsonSuccess 			(int inputStatus, bool bPrintObject = true);

	// gpio helper functions
	int 	_Init			(void);
	int 	_Exit 			(void);

	int 	_SetPin 		(int value, bool bLogicalVaue = true);
	int 	_GetPin 		(int &value, bool bLogicalVaue = true);

	int 	_GetActiveLow	(void);
	int 	_SetActiveLow	(bool activeLow);

	int 	_GetDirection	(int &bInputDir);
	int 	_SetDirection	(int bInputDir, int initValue = 0);


	// private members
	int 	gpioPin;
	int 	bRequest;
	int 	bActiveLow;
};


#endif	// _GPIO_H_