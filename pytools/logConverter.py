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
    print('Set,1,{},{}'.format(kifu[0][-4:], game.toString()))
    game.setRed(kifu[0][-4:])
    print('Set,2,{},{}'.format(kifu[1][-4:], game.toString()))
    game.changeSide()
    game.setRed(kifu[1][-4:])
    game.changeSide()
    player = 1
    for k in kifu[2:]:
        k_split = k.split(',')
        unit = k_split[-2][-1]
        direct = k_split[-1][0]
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
