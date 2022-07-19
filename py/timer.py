#!/usr/bin/env python3


import asyncio
import functools


def timer(secs, is_oneshot=True):
    """
    timer decorator takes a function and runs it as oneshot or repeat timer
    """

    def inner(fn):
        @functools.wraps(fn)
        async def wrapper(*args, **kwargs):
            while not is_oneshot:
                await asyncio.sleep(secs)
                fn(*args, **kwargs)

        return wrapper

    return inner
