#!/usr/bin/env python
# -*- coding: utf-8 -*-
import socket
import random
import argparse
import tcpClient
import execute


def run(th, port):
    cl = tcpClient.Client("localhost", port)

    red = "SET:" + th.send("red") + "\r\n"
    print(red)
    cl.send(red.encode('utf-8'))

    res = b""

    while res[0:3] != b"MOV":
        try:
            res = cl.recv()
        except Exception:
            cl.close()
            cl.reconnect()
            cl.send(b"SET:ABCD\r\n")

    while res[0:3] != b"WON" and res[0:3] != b"LST" and res[0:3] != b"DRW":
        # MOV?99r99r50R54R99b01B99b99b99r55u99r04u99b99b99b00u
        hand = th.send("hand " + res[4:].decode())
        print(hand)
        cl.move(hand[0], hand[4])
        res = cl.recv()
        while res[0:3] != b"MOV":
            res = cl.recv()
            if res[0:3] == b"WON" or res[0:3] == b"LST" or res[0:3] == b"DRW":
                break
    score = {"WON": 1, "LST": 0, "DRW": 0.1}
    return score[res[0:3].decode()]


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='このプログラムの説明（なくてもよい）')    # 2. パーサを作る

    # 3. parser.add_argumentで受け取る引数を追加していく
    parser.add_argument('player', help='この引数の説明（なくてもよい）')    # 必須の引数を追加
    parser.add_argument('-p', '--port', type=int, default=10000)   # よく使う引数なら省略形があると使う時に便利

    args = parser.parse_args()    # 4. 引数を解析

    e = execute.Execute(["../bin/windows/PlayerTest.exe"])
    run(e, args.port)
    e.send("exit")
