#ifndef _MODULE_H_
#define _MODULE_H_

#include <stdlib.h>


class Module {
public:
	Module(void);
	~Module(void);

	void 	SetVerbosity (int input);
	void 	SetVerbosity (bool input);

protected:
	int				verbosityLevel;
};

#endif 	// _MODULE_H_