#!/usr/bin/env bash


script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)

joyStickService=$script_dir/build/joystick-service
piController=py/pi-controller.py

device=/dev/input/js0
config=./xboxController.yaml
rate_us=5000
port=5005
netdev=lo
#netdev=wlp0s20u1


ip=$(ip addr show $netdev | awk '/inet /{split($2,i,"/"); print i[1]}')

pyIp=192.168.200.2

echo $ip
eval "$(ssh-agent -s)"

ssh $pyIp $piController --ip $ip --port $port
piControllerPID=$!

echo $piControllerPID

#$joyStickService --device $device --config $config --rate $rate_us --port $port
wait $piControllerPID
