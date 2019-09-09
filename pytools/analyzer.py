import sys
import csv


side = 1


# 駒の名前とプレイヤを指定して座標を取得
def getPos(name, side, board):
    if side == 1:
        x = int(board[(ord(name) - ord('A')) * 3])
        y = int(board[(ord(name) - ord('A')) * 3 + 1])
    if side == 2:
        x = int(board[(ord(name) - ord('A')) * 3 + 24])
        y = int(board[(ord(name) - ord('A')) * 3 + 1 + 24])
    return x, y


def existNeighbor(x, y, side, board):
    if side == 1:
        for i in range(8):
            nx = int(board[i * 3])
            ny = int(board[i * 3 + 1])
            if x == nx+1 and y == ny:
                return True
            if x == nx-1 and y == ny:
                return True
            if x == nx and y == ny+1:
                return True
            if x == nx and y == ny-1:
                return True
    if side == 2:
        for i in range(8):
            nx = int(board[i * 3 + 24])
            ny = int(board[i * 3 + 1 + 24])
            if x == nx+1 and y == ny:
                return True
            if x == nx-1 and y == ny:
                return True
            if x == nx and y == ny+1:
                return True
            if x == nx and y == ny-1:
                return True
    return False


def analyzer(kifu):
    result = []
    # 駒の色と初期ベクトルをセット
    if side == 1:
        for i in range(8):
            result.append(['R' if chr(i + ord('A')) in kifu[0][2] else 'B', [0] * 19])
    elif side == 2:
        for i in range(8):
            result.append(['R' if chr(i + ord('A')) in kifu[1][2] else 'B', [0] * 19])

    # 初期配置に関する特徴
    for i in range(8):
        result[i][1][i] = 1

    # 初手に動かした駒
    if side == 1:
        result[ord(kifu[2][2]) - ord('A')][1][8] = 1
    if side == 2:
        result[ord(kifu[3][2]) - ord('A')][1][8] = 1

    # 二手目に動かした駒
    if side == 1:
        result[ord(kifu[4][2]) - ord('A')][1][9] = 1
    if side == 2:
        result[ord(kifu[5][2]) - ord('A')][1][9] = 1

    # 前進した回数
    if side == 1:
        for k in kifu:
            if k[0] == 'Move' and k[1] == '1' and k[3] == 'N':
                result[ord(k[2]) - ord('A')][1][10] += 1
    if side == 2:
        for k in kifu:
            if k[0] == 'Move' and k[1] == '2' and k[3] == 'N':
                result[ord(k[2]) - ord('A')][1][10] += 1

    # 後退した回数
    if side == 1:
        for k in kifu:
            if k[0] == 'Move' and k[1] == '1' and k[3] == 'S':
                result[ord(k[2]) - ord('A')][1][11] += 1
    if side == 2:
        for k in kifu:
            if k[0] == 'Move' and k[1] == '2' and k[3] == 'S':
                result[ord(k[2]) - ord('A')][1][11] += 1

    # ゴールサイドに横移動した回数
    if side == 1:
        for k in kifu:
            if k[0] == 'Move' and k[1] == '1':
                x, y = getPos(k[2], 1, k[4])
                if (k[3] == 'E' and x > 2) or (k[3] == 'W' and x < 3):
                    result[ord(k[2]) - ord('A')][1][12] += 1
    if side == 2:
        for k in kifu:
            if k[0] == 'Move' and k[1] == '2':
                x, y = getPos(k[2], 2, k[4])
                if (k[3] == 'E' and x < 3) or (k[3] == 'W' and x > 2):
                    result[ord(k[2]) - ord('A')][1][12] += 1

    # ゴールと逆サイドに移動した回数
    if side == 1:
        for k in kifu:
            if k[0] == 'Move' and k[1] == '1':
                x, y = getPos(k[2], 1, k[4])
                if (k[3] == 'E' and x < 3) or (k[3] == 'W' and x > 2):
                    result[ord(k[2]) - ord('A')][1][13] += 1
    if side == 2:
        for k in kifu:
            if k[0] == 'Move' and k[1] == '2':
                x, y = getPos(k[2], 2, k[4])
                if (k[3] == 'E' and x > 2) or (k[3] == 'W' and x < 3):
                    result[ord(k[2]) - ord('A')][1][13] += 1

    # 駒を取った回数
    if side == 1:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '1':
                nx, ny = getPos(currentK[2], 1, nextK[-1])
                for j in range(8):
                    ox, oy = getPos(chr(j + ord('A')), 2, currentK[4])
                    if nx == ox and ny == oy:
                        result[ord(currentK[2]) - ord('A')][1][14] += 1
                        break
    if side == 2:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '2':
                nx, ny = getPos(currentK[2], 2, nextK[-1])
                for j in range(8):
                    ox, oy = getPos(chr(j + ord('A')), 1, currentK[4])
                    if nx == ox and ny == oy:
                        result[ord(currentK[2]) - ord('A')][1][14] += 1
                        break

    # 隣接から脱出
    if side == 1:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '1':
                cx, cy = getPos(currentK[2], 1, currentK[-1])
                nx, ny = getPos(currentK[2], 1, nextK[-1])
                if existNeighbor(cx, cy, 2, currentK[-1]) and not existNeighbor(nx, ny, 2, nextK[-1]):
                    result[ord(currentK[2]) - ord('A')][1][15] += 1
    if side == 2:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '2':
                cx, cy = getPos(currentK[2], 2, currentK[-1])
                nx, ny = getPos(currentK[2], 2, nextK[-1])
                if existNeighbor(cx, cy, 1, currentK[-1]) and not existNeighbor(nx, ny, 1, nextK[-1]):
                    result[ord(currentK[2]) - ord('A')][1][15] += 1

    # 隣接を保持
    if side == 1:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '1':
                for n in range(8):
                    name = chr(n + ord('A'))
                    x, y = getPos(name, 1, currentK[-1])
                    if existNeighbor(x, y, 2, currentK[-1]) and name != currentK[2]:
                        result[n][1][16] += 1
    if side == 2:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '2':
                for n in range(8):
                    name = chr(n + ord('A'))
                    x, y = getPos(name, 2, currentK[-1])
                    if existNeighbor(x, y, 1, currentK[-1]) and name != currentK[2]:
                        result[n][1][16] += 1

    # 隣接から別の隣接
    if side == 1:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '1':
                cx, cy = getPos(currentK[2], 1, currentK[-1])
                nx, ny = getPos(currentK[2], 1, nextK[-1])
                if existNeighbor(cx, cy, 2, currentK[-1]) and existNeighbor(nx, ny, 2, nextK[-1]):
                    result[ord(currentK[2]) - ord('A')][1][17] += 1
    if side == 2:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '2':
                cx, cy = getPos(currentK[2], 2, currentK[-1])
                nx, ny = getPos(currentK[2], 2, nextK[-1])
                if existNeighbor(cx, cy, 1, currentK[-1]) and existNeighbor(nx, ny, 1, nextK[-1]):
                    result[ord(currentK[2]) - ord('A')][1][17] += 1

    # 隣接していない状態から隣接
    if side == 1:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '1':
                cx, cy = getPos(currentK[2], 1, currentK[-1])
                nx, ny = getPos(currentK[2], 1, nextK[-1])
                if not existNeighbor(cx, cy, 2, currentK[-1]) and existNeighbor(nx, ny, 2, nextK[-1]):
                    result[ord(currentK[2]) - ord('A')][1][18] += 1
    if side == 2:
        for i in range(len(kifu) - 2):
            currentK = kifu[i]
            nextK = kifu[i+1]
            if currentK[0] == 'Move' and currentK[1] == '2':
                cx, cy = getPos(currentK[2], 2, currentK[-1])
                nx, ny = getPos(currentK[2], 2, nextK[-1])
                if not existNeighbor(cx, cy, 1, currentK[-1]) and existNeighbor(nx, ny, 1, nextK[-1]):
                    result[ord(currentK[2]) - ord('A')][1][18] += 1

    return result


if __name__ == "__main__":
    args = sys.argv
    filePath = args[1]
    if len(args) > 2:
        side = int(args[2])
    with open(filePath, 'r') as f:
        reader = csv.reader(f)
        data = [row for row in reader]
    # print(data)
    result = analyzer(data)
    for r in result:
        print(r[0], end="")
        for a in r[1]:
            print(",", end="")
            print(a, end="")
        print("")
