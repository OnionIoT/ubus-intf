#!/bin/bash

device=$1
function=$2

if [ $device == "expled" ];
then
	if [ $function == "set" ]
	then
		./bin/ubus-intf -device expled -function set -json '{"red":1, "blue":0, "green":0}'
	elif [ $function == "set_color" ]
	then
		./bin/ubus-intf -device expled -function set_color -json '{"blue": 1}'
	elif [ $function == "status" ]
	then
		./bin/ubus-intf -device expled -function status -json ''
	fi
elif [ $device == "gpio" ];
then
	if [ $function == "set" ]
	then
		./bin/ubus-intf -device gpio -function set -json '{"pin":0}'
	elif [ $function == "clear" ]
	then
		./bin/ubus-intf -device gpio -function clear -json '{"pin":1}'
	elif [ $function == "set_pin" ]
	then
		./bin/ubus-intf -device gpio -function set_pin -json '{"pin":2, "value":1}'
	elif [ $function == "get" ]
	then
		./bin/ubus-intf -device gpio -function get -json '{"pin":3}'
	elif [ $function == "get_activelow" ]
	then
		./bin/ubus-intf -device gpio -function get_activelow -json '{"pin":4}'
	elif [ $function == "set_activelow" ]
	then
		./bin/ubus-intf -device gpio -function set_activelow -json '{"pin":4, "activelow":false}'
	elif [ $function == "get_direction" ]
	then
		./bin/ubus-intf -device gpio -function get_direction -json '{"pin":5}'
	elif [ $function == "set_direction" ]
	then
		./bin/ubus-intf -device gpio -function set_direction -json '{"pin":5, "direction":"input"}'
		
	elif [ $function == "status" ]
	then
		./bin/ubus-intf -device gpio -function status -json ''
	fi
fi

