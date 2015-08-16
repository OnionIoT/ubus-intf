#!/bin/bash

device=$1
function=$2

if [ $device == "expled" ] 
then
	if [ $function == "set" ]
	then
		./bin/runner -device expled -function set -json '{"value": "123"}'
	elif [ $function == "set_color" ]
	then
		./bin/runner -device expled -function set_color -json '{"color": "red", "value": "true"}'
	fi
fi

