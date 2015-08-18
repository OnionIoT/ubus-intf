#ifndef _EXP_LED_H_
#define _EXP_LED_H_

#include <device.h>

#define 	EXP_LED_CTRL_CMD			"gpioctl"

#define 	EXP_LED_ENABLE_LED			"dirout-low"
#define 	EXP_LED_DISABLE_LED			"dirout-high"

#define 	EXP_LED_COLOR_R_STRING		"red"
#define 	EXP_LED_COLOR_G_STRING		"green"
#define 	EXP_LED_COLOR_B_STRING		"blue"

#define 	EXP_LED_COLOR_R_PIN_ID		17
#define 	EXP_LED_COLOR_G_PIN_ID		16
#define 	EXP_LED_COLOR_B_PIN_ID		15


typedef enum	{
	EXP_LED_COLOR_ID_R = 0,
	EXP_LED_COLOR_ID_G,
	EXP_LED_COLOR_ID_B,
	EXP_LED_COLOR_ID_NUM
} EXP_LED_COLOR_ID;

class ExpLed : public Device {
public:
	ExpLed(void);
	~ExpLed(void);

private:
	// private functions
	int 	_Process			(char* function);

	int 	_FunctionSetColor	(void);
	int 	_FunctionSet		(void);
	int		_FunctionStatus		(void);

	void 	_FunctionSetColorPrint	(int inputStatus);

	// private members
	int		pins[EXP_LED_COLOR_ID_NUM];
};

#endif	// _EXP_LED_H_