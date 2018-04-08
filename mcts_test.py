from base import *
import datetime
import copy
import random
import base
from math import sqrt, log

'''
montecarlo class 传的参数中的game的player 为要下棋的选手
UCTSearch 返回 下一步的最好的棋布
'''


class MonteCarlo(object):
    def __init__(self, game):
        self.game = game  # 传的game是当前的状态，其中player是要下的选手的颜色
        self.board = game.board
        self.player = game.player  # 当前的player
        self.max_times = datetime.timedelta(seconds=10)
        self.max_move = 60
        self.state = 0
        self.simulate_times = 20
        self.total_time = 0  # N(v)
        self.c = 0.5

    def UctSearch(self):
        valid, count, reverse = board.check(self.player.color, self.board)
        # key 值为 合法棋步，value为list，其中list[0]为N，即访问的次数 list[1]为 Q，即reward
        children = {p: [] for p in valid}
        now = datetime.datetime.utcnow()
        while (datetime.datetime.utcnow() - now) < self.max_times:
            self.Treepolicy(children, valid, count, reverse)
        # print(children)
        # print(self.total_time)
        for p in children:
            children[p][1] = children[p][1] / base.weight[p[0]][p[1]]
        # print(children)
        value, child = max((children[p][1] / children[p][0], p) for p in children)
        return child

    # 将下一步所有合法步数传给Treepolicy
    def Treepolicy(self, children, valid, count, reverse):
        flag = False

        if not children:  # 当前棋手无合法步走
            self.state += 1
            return
            # 如果没有fully expanded
        for child in children:
            if not children[child]:
                flag = True
                children[child].append(self.simulate_times)
                self.total_time += self.simulate_times
                Qv = 0
                # 选定下一步
                cur_board = board()
                cur_board.matrix = copy.deepcopy(self.board.matrix.copy())
                Flip(cur_board, self.player.color, child, valid, count, reverse)

                for i in range(self.simulate_times):
                    if self.player.color == self.Simulate(self.player.color, cur_board):
                        Qv += 1
                children[child].append(Qv)
            else:
                break
        if (flag):
            return
        else:
            # 由flag可以确定已经将子节点遍历了一遍，即fully expanded
            # UCT 策略 BestChild
            value, child = max(
                (children[p][1] / children[p][0] + self.c * sqrt(2 * log(self.total_time) / children[p][0]), p) for p in
                children)
            print(children)
            print(child)
            cur_board = board()
            cur_board.matrix = copy.deepcopy(self.board.matrix.copy())
            Flip(cur_board, self.player.color, child, valid, count, reverse)
            Qv = 0
            for i in range(self.simulate_times):
                if self.player.color == self.Simulate(self.player.color, cur_board):
                    Qv += 1
            self.total_time += self.simulate_times
            children[child][0] += self.simulate_times
            children[child][1] += Qv

            return

    def Simulate(self, color, cur_board):
        # color是下过的即上一步的颜色
        noMove = 0
        board()
        board.matrix = copy.deepcopy(cur_board.matrix)
        while True:
            # 下一步要下的颜色
            color = base.black if color == base.white else base.white
            valid, count, reverse = board.check(color, board)
            if not valid:
                noMove += 1
                if noMove == 2:
                    return board.winner(board)

            else:
                step = random.choice(valid)
                Flip(board, color, step, valid, count, reverse)
                noMove = 0


def Flip(board, color, step, valid, count, reverse):
    index = valid.index(step)
    flip = reverse[index]
    for i, j in flip:
        board.matrix[i][j] = base.black if board.matrix[i][j] == base.white else base.white
    board.matrix[step[0]][step[1]] = color