#!/bin/sh

case "$1" in
	list)
		echo '{ "set": { "arg1": "string" }, "status": { } }'
	;;
	call)
		case "$2" in
			set)
				# read the arguments
				read input;

				# check the input
				val=`echo $input | grep -ci true`

				if [ "$val" == 1 ] ; then
					pinVal="dirout-low"
				else
					pinVal="dirout-high"
				fi

				cmd="gpioctl $pinVal 17"
				$cmd > /dev/null

				# return json object or an array
				echo -n '{"val": "' 
				echo -n $cmd
				echo '"}'

				#echo $input
			;;
			status)
				VAL=`gpioctl get 17 | grep -c LOW`

				# return json object or an array
				echo  -n '{"value": ' 
				echo -n $VAL
				echo '}'
			;;
		esac
	;;
esac
