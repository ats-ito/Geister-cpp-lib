#!/usr/bin/env python
# -*- coding: utf-8 -*-

import competition
import client
import sys
import gui
import execute
from cmd import Cmd
import os
import argparse


class GeisterTools(Cmd):
    prompt = "geister> "
    intro = """------------------------------------------------------------
    This is CLI for Geister
    ガイスター用の各種ツールを操作するインタフェースです
------------------------------------------------------------"""

    def __init__(self):
        Cmd.__init__(self)

    def do_hoge(self, arg):
        print("hoge")

    def help_hoge(self):
        print("echo hoge")

    def do_match(self, arg):
        parser = competition.parser
        args = parser.parse_args(arg.rsplit())
        first = execute.Execute([args.player1])
        second = execute.Execute([args.player2])
        print("1st {}".format(first.recieve()))
        print("2nd {}".format(second.recieve()))
        results = competition.match(first, second, logEnable=args.log, outputLevel=args.output, matchCount=args.count)
        # print(results)
        resultList = {1:0,2:0,3:0,-1:0,-2:0,-3:0,0:0}
        for r in results:
            resultList[r] += 1
        print(resultList)
        first.send("exit")
        second.send("exit")

    def help_match(self):
        print("start match")

    def do_connect(self, arg):
        arg = arg.rsplit()
        first = execute.Execute([arg[0]])
        client.run(first, int(arg[1]))
        first.send("exit")
    
    def help_connect(self):
        print("connect to server")

    def complete_match(self, text, line, begidx, endidx):
        line = line.split()
        if len(line) > 1:
            text = line[-1]
            if text[0] == '-':
                if len(text) > 1:
                    if text[1] == 'c':
                        return []
                else:
                    return ['c', 'l']
            if '/' in text:
                i = text.rfind('/')
                filename = text[i+1:]
                path = text[:i]
            else:
                filename = text
                path = './'
        else:
            filename = ''
            path = './'

        ls = os.listdir(path)
        if filename == '':
            return ls
        else:
            return [path + '/' + f for f in ls if f.startswith(filename)]

    def do_gui(self, args):
        gui.run()

    def help_gui(self):
        pass

    def help_server(self, arg):
        pass

    def help_server(self):
        pass

    def do_exit(self, arg):
        return True

    # 入力が空に対するオーバーライド
    def emptyline(self):
        pass

if __name__ == "__main__":
    GeisterTools().cmdloop()
    exit()
    while(True):
        command = sys.stdin.readline()
        commandList = command.rsplit()
        if(commandList):
            if(commandList[0] == "hoge"):
                print("hoge")
            elif(commandList[0] == "match"):
                first = execute.Execute([commandList[1]])
                second = execute.Execute([commandList[2]])
                competition.match(first, second)
                first.send("exit")
                second.send("exit")
            elif(commandList[0] == "connect"):
                first = execute.Execute([commandList[1]])
                client.run(first, int(commandList[2]))
                first.send("exit")
            elif(commandList[0] == "gui"):
                gui.run()
            elif(commandList[0] == "exit"):
                break
