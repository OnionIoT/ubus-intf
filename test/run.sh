#!/bin/bash

device=$1
function=$2

if [ $device == "expled" ] 
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
fi

