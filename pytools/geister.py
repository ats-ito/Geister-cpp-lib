#!/usr/bin/env python
# -*- coding: utf-8 -*-
import unit


class Geister:
    colorIndex = {'B': 0, 'b': 1, 'R': 2, 'r':3, 'U': 4, 'u': 5}
    num2color = ['B', 'b', 'R', 'r', 'U', 'u']
    def __init__(self, info=b"14U24U34U44U15U25U35U45U41u31u21u11u40u30u20u10u"):
        self.state = info.decode()
        self.units = [
            unit.Unit(1, 4, Geister.colorIndex[self.state[2]], "A"), unit.Unit(2, 4, Geister.colorIndex[self.state[5]], "B"), unit.Unit(3, 4, Geister.colorIndex[self.state[8]], "C"), unit.Unit(4, 4, Geister.colorIndex[self.state[11]], "D"),
            unit.Unit(1, 5, Geister.colorIndex[self.state[14]], "E"), unit.Unit(2, 5, Geister.colorIndex[self.state[17]], "F"), unit.Unit(3, 5, Geister.colorIndex[self.state[20]], "G"), unit.Unit(4, 5, Geister.colorIndex[self.state[23]], "H"),
            unit.OpUnit(4, 1, Geister.colorIndex[self.state[26]], "a"), unit.OpUnit(3, 1, Geister.colorIndex[self.state[29]], "b"), unit.OpUnit(2, 1, Geister.colorIndex[self.state[32]], "c"), unit.OpUnit(1, 1, Geister.colorIndex[self.state[35]], "d"),
            unit.OpUnit(4, 0, Geister.colorIndex[self.state[38]], "e"), unit.OpUnit(3, 0, Geister.colorIndex[self.state[41]], "f"), unit.OpUnit(2, 0, Geister.colorIndex[self.state[44]], "g"), unit.OpUnit(1, 0, Geister.colorIndex[self.state[47]], "h")
        ]
        self.field = [
            [None, self.units[15], self.units[14], self.units[13], self.units[12], None],
            [None, self.units[11], self.units[10], self.units[9], self.units[8], None],
            [None, None, None, None, None, None],
            [None, None, None, None, None, None],
            [None, self.units[0], self.units[1], self.units[2], self.units[3], None],
            [None, self.units[4], self.units[5], self.units[6], self.units[7], None]
        ]
    
    def setState(self, state):
        self.state = state.decode()
        for i in range(6):
            for j in range(6):
                self.field[i][j] = None
        for i in range(16):
            self.units[i].x = int(self.state[4 + i * 3])
            self.units[i].y = int(self.state[4 + i * 3 + 1])
            if int(self.state[4 + i * 3 + 1]) > 5:
                self.units[i].taken = True
                self.units[i].color = Geister.colorIndex[self.state[4 + i * 3 + 2]]
                continue
            self.field[self.units[i].y][self.units[i].x] = self.units[i]
        
    def setRed(self, red):
        for i in range(8):
            if self.units[i].name in red:
                self.units[i].color = 2 #if i < 8 else 3
            else:
                self.units[i].color = 0 #if i < 8 else 1

    def printBoard(self):
        color = list(Geister.colorIndex.keys())
        print("  0 1 2 3 4 5")
        for i in range(6):
            print(i, end=" ")
            for j in range(6):
                print(color[self.field[i][j].color] if self.field[i][j] else " ", end=" ")
            print("")
        for i in range(16):
            print(self.units[i].name, "(", color[self.units[i].color], "): ", self.units[i].x, ", ", self.units[i].y, sep="")

    def canMove(self, unit, direct, x, y, color, field):
        if direct == 0:
            y -= 1
        elif direct == 1:
            if x == 5:
                return y == 0 and color == 0
            x += 1
        elif direct == 2:
            if x == 0:
                return y == 0 and color == 0
            x -= 1
        elif direct == 3:
            y += 1
        return field[y][x] == None or field[y][x] % 2

    def getLegalMove(self):
        colorField = [[None] * 6 for _ in range(6)]
        for u in [u for u in self.units if u.x < 6]:
            colorField[u.y][u.x] = u.color
        legalMoves = [(i, d) for i, u in [(i, u) for i, u in enumerate(self.units[0:8]) if u.x < 6] for d in range(4) if (d != 0 or u.y != 0) and (d != 3 or u.y != 5) and (d != 1 or u.x != 5 or (u.y == 0 and u.color == 0)) and (d != 2 or u.x != 0 or u.y == 0 and u.color == 0) and (
            d == 1 and ((u.x == 5 and u.y == 0 and u.color == 0) or u.x != 5 and (colorField[u.y][u.x + 1] == None or colorField[u.y][u.x + 1] % 2))
            or d == 2 and (u.x == 0 and u.y == 0 and u.color == 0 or u.x != 0 and (colorField[u.y][u.x - 1] == None or colorField[u.y][u.x - 1] % 2))
            or d == 0 and (colorField[u.y - 1][u.x] == None or colorField[u.y - 1][u.x] % 2)
            or d == 3 and (colorField[u.y + 1][u.x] == None or colorField[u.y + 1][u.x] % 2)
        )]
        return legalMoves

    def changeSide(self):
        for i in range(6):
            for j in range(6):
                self.field[i][j] = None

        for i in range(8):
            tmp = unit.Unit(self.units[i].x, self.units[i].y, self.units[i].color, self.units[i].name)

            if self.units[i+8].x > 5:
                self.units[i].x = self.units[i+8].x
                self.units[i].y = self.units[i+8].y
            else:
                self.units[i].x = 5 - self.units[i+8].x
                self.units[i].y = 5 - self.units[i+8].y
            self.units[i].name = self.units[i+8].name.upper()
            self.units[i].color = self.units[i+8].color - 1

            if tmp.x > 5:
                self.units[i+8].x = tmp.x
                self.units[i+8].y = tmp.y
            else:
                self.units[i+8].x = 5 - tmp.x
                self.units[i+8].y = 5 - tmp.y
            self.units[i+8].name = tmp.name.lower()
            self.units[i+8].color = tmp.color + 1
            if self.units[i].y < 6:
                self.field[self.units[i].y][self.units[i].x] = self.units[i]
            if self.units[i+8].y < 6:
                self.field[self.units[i+8].y][self.units[i+8].x] = self.units[i+8]

    def move(self, unit, direct):
        targetIndex = 0
        for i in range(16):
            if unit == self.units[i].name:
                targetIndex = i
                break
        self.field[self.units[targetIndex].y][self.units[targetIndex].x] = None
        x, y = self.units[targetIndex].x, self.units[targetIndex].y
        if(direct == 'N'):
            y -= 1
        elif(direct == 'E'):
            x += 1
        elif(direct == 'W'):
            x -= 1
        elif(direct == 'S'):
            y += 1
        self.units[targetIndex].x, self.units[targetIndex].y = x, y
        if x == -1 or x == 6:
            self.units[targetIndex].x, self.units[targetIndex].y = 8, 8
            return
        if self.field[y][x]:
            self.field[y][x].x, self.field[y][x].y = 9, 9
        self.field[y][x] = self.units[targetIndex]

    def toString(self):
        ret = ""
        for i in range(16):
            ret += "{}{}{}".format(self.units[i].x, self.units[i].y, self.num2color[self.units[i].color])
        return ret

    def checkResult(self):
        takeB = 0
        takeb = 0
        takeR = 0
        taker = 0
        for i in range(16):
            if self.units[i].x == 8:
                return 1 if i < 8 else -1
            if self.units[i].x == 9:
                if self.units[i].color == 0:
                    takeB += 1
                    if takeB == 4:
                        return -2
                if self.units[i].color == 1:
                    takeb += 1
                    if takeb == 4:
                        return 2
                if self.units[i].color == 2:
                    takeR += 1
                    if takeR == 4:
                        return 3
                if self.units[i].color == 3:
                    taker += 1
                    if taker == 4:
                        return -3
        return 0
            


if __name__ == "__main__":
    game = Geister(b"14B24B34B44B15R25R35R45R41b31b21b11b40r30r20r10r")
    game.printBoard()
    game.move("B", "N")
    game.move("B", "N")
    game.move("B", "N")
    # game.move("B", "N")
    game.move("B", "E")
    game.move("B", "E")
    game.move("B", "W")
    game.move("B", "W")
    game.move("B", "W")
    game.printBoard()
    game.changeSide()
    game.printBoard()
    print(game.toString())
    game.changeSide()
    print(game.checkResult())