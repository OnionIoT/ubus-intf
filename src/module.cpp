#include <module.h>

Module::Module(void)
{
	// not verbose by default
	bVerboseMode	= false;
}

Module::~Module(void)
{
	// nothing for now
}

void Module::SetVerbosity (bool input)
{
	bVerboseMode	= input;
}