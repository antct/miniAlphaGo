# -*- coding: utf-8 -*-
import datetime
import copy
import random
import base
import math


class monte_carlo(object):
    def __init__(self, game):
        self.game = game  # 传的game是当前的状态，其中player是要下的选手的颜色
        self.board = game.board
        self.simulate_board = base.board()
        self.player = game.player
        self.simulate_player = base.player(game.player.color, game.player.mode)
        self.max_times = datetime.timedelta(seconds=30)
        self.max_move = 60
        self.state = 0
        self.simulate_times = 20
        self.total_time = 0  # N(v)
        self.c = 0.5
        self.expanded = False

    def uct_search(self):
        valid, count, reverse = base.board.check(self.player.color, self.board)
        # key 值为 合法棋步，value为list，其中list[0]为N，即访问的次数 list[1]为 Q，即reward
        children = {p: [] for p in valid}
        now = datetime.datetime.utcnow()
        while (datetime.datetime.utcnow() - now) < self.max_times:
            self.__tree_policy(children, valid, count, reverse)
        # print(children)
        # print(self.total_time)
        # for p in children:
        #     children[p][1] = children[p][1] / base.weight[p[0]][p[1]]
        # print(children)
        value, child = max((children[p][1] / children[p][0], p) for p in children)
        print(str(child) + ": " + str(value))
        return child

    def __board_copy(self):
        self.simulate_board.matrix = copy.deepcopy(self.board.matrix.copy())

    def __flip(self, step, valid, reverse):
        flip = reverse[valid.index(step)]
        for i, j in flip:
            self.simulate_board.matrix[i][j] = base.black if self.simulate_board.matrix[i][
                                                                 j] == base.white else base.white
            self.simulate_board.matrix[step[0]][step[1]] = self.simulate_player.color

    def __expand(self, children, valid, reverse):
        for child in children:
            if not children[child]:
                children[child].append(self.simulate_times)
                self.total_time += self.simulate_times
                Qv = 0
                for i in range(self.simulate_times):
                    self.__board_copy()
                    self.__flip(child, valid, reverse)
                    if self.player.color == self.__simulate():
                        Qv += 1
                children[child].append(Qv)
            else:
                break
        self.expanded = True

    def __best_child(self, children):
        value, child = max(
            (children[p][1] / children[p][0] + self.c * math.sqrt(2 * math.log(self.total_time) / children[p][0]), p)
            for p in children)
        return value, child

    def __tree_policy(self, children, valid, count, reverse):
        if not children:  # 当前棋
            self.state += 1  # 0: 1: 2:
            return
        if not self.expanded:
            self.__expand(children, valid, reverse)
        else:
            value, child = self.__best_child(children)
            Qv = 0
            for i in range(self.simulate_times):
                self.__board_copy()
                self.__flip(child, valid, reverse)
                if self.player.color == self.__simulate():
                    Qv += 1
            self.total_time += self.simulate_times
            children[child][0] += self.simulate_times
            children[child][1] += Qv
        print(children)

    def __simulate(self):
        noMove = 0
        self.simulate_player.color = self.player.color
        while True:
            # 下一步要下的颜色
            self.simulate_player.color = base.black if self.simulate_player.color == base.white else base.white
            valid, count, reverse = base.board.check(self.simulate_player.color, self.simulate_board)
            if not valid:
                noMove += 1
                if noMove == 2:
                    return base.board.winner(self.simulate_board)
            else:
                step = random.choice(valid)
                self.__flip(step, valid, reverse)
                noMove = 0


import threading


class mcts_thread(threading.Thread):
    def __init__(self, game):
        threading.Thread.__init__(self)
        self.mcts = monte_carlo(game)
        self.result = 0, 0

    def run(self):
        self.result = self.mcts.uct_search()

    def get_result(self):
        return self.result
