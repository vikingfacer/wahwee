#!/usr/bin/env python3

import math
import argparse
import os
import sys
import time
import zmq
import struct

import RPi.GPIO as GPIO
from collections import namedtuple

motor_cmd_format = "4schf"

motor_cmd = namedtuple("motor_cmd", "channel command module percent")

# enable pins

# rb motor right
pwm1 = 18
enRBR = 6
# yb motor right
pwm2 = 13
enYBR = 20
# rb motor left
pwm3 = 12
enRBL = 5
# yb motor left
pwm4 = 19
enYBL = 16

GPIO.setwarnings(False)  # disable warnings
GPIO.setmode(GPIO.BCM)  # set pin numbering system

GPIO.setup(enRBR, GPIO.OUT)
GPIO.setup(enRBL, GPIO.OUT)
GPIO.setup(enYBR, GPIO.OUT)
GPIO.setup(enYBL, GPIO.OUT)
GPIO.output(enRBR, GPIO.HIGH)
GPIO.output(enRBL, GPIO.HIGH)
GPIO.output(enYBR, GPIO.HIGH)
GPIO.output(enYBL, GPIO.HIGH)

GPIO.setup(pwm1, GPIO.OUT)
GPIO.setup(pwm2, GPIO.OUT)
GPIO.setup(pwm3, GPIO.OUT)
GPIO.setup(pwm4, GPIO.OUT)

modToPin = {
    "m1": {"neg": GPIO.PWM(pwm1, 1000), "pos": GPIO.PWM(pwm3, 1000)},
    "m2": {
        "neg": GPIO.PWM(pwm2, 1000),
        "pos": GPIO.PWM(pwm4, 1000),
    },
}

modToPin["m1"]["neg"].start(100.0)
modToPin["m1"]["pos"].start(100.0)
modToPin["m2"]["neg"].start(100.0)
modToPin["m2"]["pos"].start(100.0)


def motor(module, pwm_percent):

    module["neg"].ChangeDutyCycle(0)
    module["pos"].ChangeDutyCycle(0)

    if pwm_percent > 20:
        print("changing positive duty cycle")
        module["pos"].ChangeDutyCycle(pwm_percent)
    elif pwm_percent < -20:
        print("changing negative pin duty cycle")
        module["neg"].ChangeDutyCycle(-1 * pwm_percent)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="controller off pins for the waahwee project"
    )
    parser.add_argument("--ip", help="ip address of controller")
    parser.add_argument("--port", help="port of controller")

    parsed = parser.parse_args()
    print(parsed)
    context = zmq.Context()
    socket = context.socket(zmq.SUB)
    socket.connect("tcp://" + parsed.ip + ":" + parsed.port)

    subscription = b"JS "
    socket.setsockopt(zmq.SUBSCRIBE, subscription)

    while True:
        data = socket.recv()
        print("length:{} data: {}".format(len(data), data))
        try:
            unpackedmotor = motor_cmd._make(struct.unpack(motor_cmd_format, data))
            print(unpackedmotor)

            if unpackedmotor.command == b"m":
                if unpackedmotor.module == 1:
                    motor(modToPin["m1"], unpackedmotor.percent)
                elif unpackedmotor.module == 2:
                    motor(modToPin["m2"], unpackedmotor.percent)

        except Exception as e:
            print(e)
