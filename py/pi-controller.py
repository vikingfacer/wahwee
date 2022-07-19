#!/usr/bin/env python3

import attrs
import argparse
import asyncio
import functools
import math
import os
import sys
import time
import timer

import zmq
import zmq.asyncio

import motorCmd

try:
    # checks if you have access to RPi.GPIO, which is available inside RPi
    import RPi.GPIO as GPIO
except:
    # In case of exception, you are executing your script outside of RPi, so import Mock.GPIO
    import Mock.GPIO as GPIO


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

GPIO.setwarnings(True)  # disable warnings
GPIO.setmode(GPIO.BCM)  # set pin numbering system


class motormodule:
    def __init__(self, neg: int, negEn: int, pos: int, posEn: int, shutoff: int):
        for arg in locals():
            GPIO.setup(arg, GPIO.OUT)
            GPIO.output(arg, GPIO.LOW)

        self._neg = GPIO.PWM(neg, 1000)
        self._negEn = negEn
        self._pos = GPIO.PWM(pos, 1000)
        self._posEn = posEn
        self._last_cmd = time.time_ns()
        self._pos.start(0)
        self._neg.start(0)
        self.shutoff = shutoff

    def motor(self, pwm_percent) -> None:

        self._last_cmd = time.time_ns()
        self._neg.ChangeDutyCycle(0)
        self._pos.ChangeDutyCycle(0)

        if pwm_percent > 20:
            GPIO.output(self._posEn, GPIO.HIGH)
            GPIO.output(self._negEn, GPIO.LOW)
            self._pos.ChangeDutyCycle(pwm_percent)
        elif pwm_percent < -20:
            print("changing negative pin duty cycle")
            GPIO.output(self._posEn, GPIO.LOW)
            GPIO.output(self._negEn, GPIO.HIGH)
            self._neg.ChangeDutyCycle(-1 * pwm_percent)

    def checkpins(self):
        if time.time_ns() - self._last_cmd > self.shutoff:
            GPIO.output(self._posEn, GPIO.LOW)
            GPIO.output(self._negEn, GPIO.LOW)


@timer.timer(1, is_oneshot=False)
def checkPins(modules):
    print("checkin pins", flush=True)
    modules["m1"].checkpins()
    modules["m2"].checkpins()


async def receiveSubScription(modToPin, subIP, subscription) -> None:
    context = zmq.asyncio.Context()
    socket = context.socket(zmq.SUB)
    socket.connect(subIP)
    socket.setsockopt(zmq.SUBSCRIBE, subscription)
    while True:
        data = await socket.recv()
        print("length:{} data: {}".format(len(data), data), flush=True)
        try:
            unpackedmotor = motorCmd.toTuple(data)
            print(unpackedmotor)
            processCmd(modToPin, unpackedmotor)

        except Exception as e:
            print(e)


def processCmd(modToPin, cmd) -> None:
    if cmd.command == b"m":
        if cmd.module == 1:
            modToPin["m1"].motor(cmd.percent)
        elif cmd.module == 2:
            modToPin["m2"].motor(cmd.percent)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="controller off pins for the waahwee project"
    )
    parser.add_argument("--ip", help="ip address of controller")
    parser.add_argument("--port", help="port of controller")

    parsed = parser.parse_args()
    print(parsed)
    ip = "tcp://" + parsed.ip + ":" + parsed.port
    subscription = b"JS "

    modToPin = {
        "m1": motormodule(pwm1, enRBR, pwm3, enYBR, 200),
        "m2": motormodule(pwm2, enRBL, pwm4, enYBL, 200),
    }
    asyncio.run(
        asyncio.wait(
            [checkPins(modToPin), receiveSubScription(modToPin, ip, subscription)]
        )
    )
