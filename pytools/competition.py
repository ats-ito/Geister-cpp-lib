#!/usr/bin/env python
# -*- coding: utf-8 -*-
import socket
import random
import geister
import execute
import argparse
import logging
import sys


def match(first, second, output=2, log=False):
    # first.send("init")
    # second.send("init")
    game = geister.Geister()
    red1 = first.send("red")#.upper()
    game.setRed(red1)
    game.changeSide()
    red2 = second.send("red")#.lower()
    game.setRed(red2)
    game.changeSide()
    game.printBoard()
    result = 0
    while(result == 0):
        game.printBoard()
        hand = first.send("hand " + game.toString())
        print(hand)
        game.move(hand[0], hand[4])
        game.printBoard()
        result = game.checkResult()
        if(result):
            break
        game.changeSide()
        hand = second.send("hand " + game.toString())
        print(hand)
        game.move(hand[0], hand[4])
        game.changeSide()
        result = game.checkResult()
    print(result)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='このプログラムの説明（なくてもよい）')

    parser.add_argument('player1')
    parser.add_argument('player2')
    parser.add_argument('-c', '--count', type=int, default=1)

    args = parser.parse_args()
    first = execute.Execute([args.player1])
    second = execute.Execute([args.player2])
    print("1st {}".format(first.recieve()))
    print("2nd {}".format(second.recieve()))
    match(first, second)
    first.send("exit")
    second.send("exit")
