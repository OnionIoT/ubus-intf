#!/bin/sh

device="expled"

jsonSet='"set": { "red": 32, "green": 32, "blue": 32 }'
jsonStatus='"status": { }'

case "$1" in
    list)
		echo "{ $jsonSet, $jsonStatus }"
    ;;
    call)
		case "$2" in
			set)
				# read the json arguments
				read input;

				# run the ubus interface
				ubus-intf -device $device -function $2 -json $input
			;;
			status)
				# run the ubus interface
				ubus-intf -device $device -function $2 -json \'\'
		;;
		esac
    ;;
esac

