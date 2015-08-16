#include <device.h>

Device::Device(void)
{
	// not in debug mode by default
	bDebugMode	= false;
}

Device::~Device(void)
{
	// nothing for now
}

// public functions
void Device::SetDebugMode(bool input)
{
	bDebugMode	= input;
}

void Device::SystemCommand(char* cmd)
{
	if (bDebugMode) {
		printf("Running: %s", cmd);
	}
	else	{
		system(cmd);
	}
}