#!/usr/bin/env python
# -*- coding: utf-8 -*-
import subprocess
import time

class Execute(object):
    """外部プログラムを実行。stdinとstdoutで対話する。"""

    def __init__(self, *args_1, **args_2):
        if 'encoding' in args_2:
            self.encoding = args_2.pop('encoding')
        else:
            self.encoding = 'utf-8'
        self.popen = subprocess.Popen(*args_1, stdin=subprocess.PIPE, stdout=subprocess.PIPE, encoding=self.encoding, **args_2)
    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback):
        self.close()

    def send(self, message, recieve=True, incr=False):
        message = message.rstrip('\n')
        if not incr and '\n' in message:
            raise ValueError("message in \\n!")
        self.popen.stdin.write(message + '\n')
        self.popen.stdin.flush()
        if recieve:
            return self.recieve().rstrip('\n')
        return None

    def recieve(self):
        self.popen.stdout.flush()
        return self.popen.stdout.readline().rstrip('\n')

    def close(self):
        self.popen.stdin.close()
