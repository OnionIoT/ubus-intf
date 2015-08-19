#!/bin/bash

device=$1
function=$2

if [ $device == "expled" ];
then
	if [ $function == "set" ]
	then
		./bin/ubus_intf -device expled -function set -json '{"value": "123"}'
	elif [ $function == "set_color" ]
	then
		./bin/ubus_intf -device expled -function set_color -json '{"color": "red", "value": "true"}'
	elif [ $function == "status" ]
	then
		./bin/ubus_intf -device expled -function status -json ''
	fi
elif [ $device == "gpio" ];
then
	if [ $function == "set" ]
	then
		./bin/ubus_intf -device gpio -function set -json '{"pin":0}'
	elif [ $function == "clear" ]
	then
		./bin/ubus_intf -device gpio -function clear -json '{"pin":1}'
	elif [ $function == "set_pin" ]
	then
		./bin/ubus_intf -device gpio -function set_pin -json '{"pin":2, "value":1}'
	elif [ $function == "get" ]
	then
		./bin/ubus_intf -device gpio -function get -json '{"pin":3}'
	elif [ $function == "status" ]
	then
		./bin/ubus_intf -device gpio -function status -json ''
	fi
fi

