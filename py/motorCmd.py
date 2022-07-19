#!/usr/bin/env python3


"""
    All python cmds are defined here for the wahwee communications

"""

import struct
from collections import namedtuple


"""
motor_cmd_format doc string
4s - subscription
c - cmd
h - motor module
f - percentage PWM
"""
motor_cmd_format = "4schf"
motor_cmd = namedtuple("motor_cmd", "channel command module percent")


def toTuple(data):
    return motor_cmd._make(struct.unpack(motor_cmd_format, data))


def toStruct(cmd):
    return struct.pack(
        motor_cmd_format, cmd.channel, cmd.command, cmd.module, cmd.percent
    )
