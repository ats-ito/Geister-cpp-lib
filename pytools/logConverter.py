import geister
import sys

args = sys.argv

if __name__ == "__main__":
    game = geister.Geister()
    kifu = []
    with open(args[1], 'r') as fin:
        kifu = fin.readlines()
        kifu = [k for k in kifu if len(k)]
        kifu = kifu[:-1]
        kifu = [x.rstrip('\n') for x in kifu]
    # print(args[1], file=sys.stderr)
    if kifu[0][7] == '1':
        kifu[0], kifu[1] = kifu[1], kifu[0]
    kifu = [k.split(',') for k in kifu]
    firstRed = kifu[0][1][-4:]
    secondRed = kifu[1][1][-4:]
    print('Set,1,{},{}'.format(firstRed, game.toString()))
    game.setRed(firstRed)
    print('Set,2,{},{}'.format(secondRed, game.toString()))
    game.changeSide()
    game.setRed(secondRed)
    game.changeSide()
    player = 1
    for k in kifu[2:]:
        unit = k[1][4]
        direct = k[2]
        # print(k, file=sys.stderr)
        print('Move,{},{},{},{}'.format(player, unit, direct, game.toString()))
        if player == 2:
            game.changeSide()
        game.move(unit, direct)
        if player == 2:
            game.changeSide()
        player = 1 if player == 2 else 2
    print('Result,{},{}'.format(game.checkResult(), game.toString()))
    print('Turn,{}'.format(len(kifu)-3))
