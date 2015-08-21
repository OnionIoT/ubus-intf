#ifndef _UBUS_INTF_
#define _UBUS_INTF_

#include <rapidjson/document.h>

#include <stdlib.h>
#include <cstdio>
#include <cstring>

#include <device/device.h>
#include <device/exp_led.h>
#include <device/gpio.h>



#define	UBUS_INTF_IS_PRODUCTION		0

#ifdef __APPLE__
	#define UBUS_INTF_IS_OMEGA		0
#else
	#define UBUS_INTF_IS_OMEGA		1
#endif 


#if UBUS_INTF_IS_PRODUCTION
	#define		UBUS_INTF_VERBOSE	false
#else
	#define		UBUS_INTF_VERBOSE	true
#endif	// UBUS_INTF_IS_PRODUCTION


#if UBUS_INTF_IS_OMEGA
	#define		UBUS_INTF_DEBUG		false
#else
	#define		UBUS_INTF_DEBUG		true
#endif	// UBUS_INTF_IS_OMEGA



#endif 	// _UBUS_INTF_
