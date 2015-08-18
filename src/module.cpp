#include <module.h>

Module::Module(void)
{
	// not verbose by default
	verbosityLevel	= 0;
}

Module::~Module(void)
{
	// nothing for now
}

void Module::SetVerbosity (int input)
{
	verbosityLevel	= input;
}

void Module::SetVerbosity (bool input)
{
	verbosityLevel	= (input ? 1 : 0);
}