#!/usr/bin/env python3

import argparse
import asyncio
import math
import os
import sys
import time

import zmq
import zmq.asyncio

import motorCmd


cmd_data = [motorCmd.motor_cmd(b"JS ", list("m")[0].encode("ascii"), 1, 50)]


async def sendCmd(subIP, data):
    context = zmq.asyncio.Context()
    socket = context.socket(zmq.PUB)
    socket.bind(subIP)
    while True:
        try:
            cmd = motorCmd.toStruct(data)
            #        print(cmd)
            result = await socket.send(cmd)
            print("send result: {}".format(result))
            await asyncio.sleep(0.1)
        except Exception as e:
            print(e)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="controller off pins for the waahwee project"
    )
    parser.add_argument("--ip", help="ip address of controller")
    parser.add_argument("--port", help="port of controller")

    parsed = parser.parse_args()
    print(parsed)
    ip = "tcp://" + parsed.ip + ":" + parsed.port

    asyncio.run(asyncio.wait([sendCmd(ip, cmd_data[0])]))
