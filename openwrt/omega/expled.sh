#!/bin/sh

device="expled"

case "$1" in
    list)
		echo '{ "set": { "red": "string", "green": "string", "blue": "string" }, "status": { } }'
    ;;
    call)
		case "$2" in
			set)
				# read the json arguments
				read input;

				# run the ubus interface
				ubus_intf -device $device -function $2 -json $input
			;;
			status)
				# run the ubus interface
				ubus_intf -device $device -function $2 -json \'\'
		;;
		esac
    ;;
esac