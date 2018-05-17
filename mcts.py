# -*- coding: utf-8 -*-
import datetime
import copy
import random
import base
import math
import queue

'''
    def alpha_beta(self,node,depth,isMaximizingPlayer,alpha,beta):
        valid, count, reverse = base.board.check(self.simulate_player.color, self.simulate_board)
        if not valid:
            winner,rate=base.board.winner(self.simulate_board)
            return winner,rate
        if isMaximizingPlayer:
            bestVal=-inf
            for child in valid:
                winner,rate

          bestVal = -INFINITY 
        for each child node :
            value = minimax(node, depth+1, false, alpha, beta)
            bestVal = max( bestVal, value) 
            alpha = max( alpha, bestVal)
            if beta <= alpha:
                break
        return bestVal
  '''

'''
function minimax(node, depth, isMaximizingPlayer, alpha, beta):

    if node is a leaf node :
        return value of the node

    if isMaximizingPlayer :
        bestVal = -INFINITY 
        for each child node :
            value = minimax(node, depth+1, false, alpha, beta)
            bestVal = max( bestVal, value) 
            alpha = max( alpha, bestVal)
            if beta <= alpha:
                break
        return bestVal

    else :
        bestVal = +INFINITY 
        for each child node :
            value = minimax(node, depth+1, true, alpha, beta)
            bestVal = min( bestVal, value) 
            beta = min( beta, bestVal)
            if beta <= alpha:
                break
        return bestVal
'''


class tree_node(object):
    def __init__(self, color, board):
        self.board = board
        self.color = color
        self.parent = None
        self.children = []  # children node
        self.valid, self.count, self.reverse = base.board.check(self.color, self.board)
        self.untried_action = copy.deepcopy(self.valid)
        self.Nv = 0
        self.Qv = 0
        self.move = (-1, -1)
        self.depth = 0


def Flip(board, color, step, valid, count, reverse):
    index = valid.index(step)
    flip = reverse[index]
    for i, j in flip:
        board.matrix[i][j] = base.black if board.matrix[i][j] == base.white else base.white
    board.matrix[step[0]][step[1]] = color


class monte_carlo(object):
    def __init__(self, game):
        self.game = game  # 传的game是当前的状态，其中player是要下的选手的颜色
        self.board = game.board
        self.simulate_board = base.board()
        self.player = game.player
        self.simulate_player = base.player(game.player.color, game.player.mode)
        self.max_times = datetime.timedelta(seconds=3)
        self.max_move = 60
        self.state = 0
        # self.simulate_times = 20
        self.total_time = 0  # N(v)
        self.c = 0.5
        # self.expanded = False

    def __backup(self, node, delta):
        self.total_time += 1
        while node.parent:
            node.Nv += 1
            node.Qv += delta
            node = node.parent

    def uct_search(self):
        # create root node
        root = tree_node(self.player.color, self.board)
        print("root", root.color)

        now = datetime.datetime.utcnow()
        i = 1000
        # node=self.__tree_policy(root)
        # while (datetime.datetime.utcnow() - now) < self.max_times:
        while i > 0:
            node = self.__tree_policy(root)
            # print(node.move)
            delta = self.__simulate(node)
            # print(delta)
            self.__backup(node, delta)
            i = i - 1
        print()
        print()
        # print("winnertest")
        # print(base.winnertest)
        # print("ending",child.move)
        # 打印整课数

        myqueue = queue.Queue()
        myqueue.put(root)
        '''
        while not myqueue.empty():
            a=myqueue.get()
            for p in a.children:
                myqueue.put(p)


            if a.parent:
                print(a.parent.move,a.move,a.Qv,a.Nv,a.depth)
            else:
                print(a.move,a.Qv,a.Nv,a.depth)
         '''

        print("result", [p.Qv / p.Nv for p in root.children])
        # print([(p.Qv,p.Nv) for p in root.children])
        print(self.total_time)
        child = self.__best_child(root, 0)

        return child.move

    def __simulate(self, node):

        tmpboard = base.board()
        tmpboard.matrix = copy.deepcopy(node.board.matrix)
        color = node.color
        while True:
            # 下一步要下的颜色
            valid, count, reverse = base.board.check(color, tmpboard)
            # print("valid",valid,node.valid)
            if not valid:
                # print("当前simulate的点",node.move,node.depth)
                # if node.depth>=7:
                #   print(node.board.matrix)
                # print(tmpboard.matrix)
                a = base.board.winner(tmpboard, self.player.color)
                if base.flag:
                    # print("what happened")
                    # print(node.move,node.depth,node.board.matrix)
                    base.flag = False
                return a
            else:
                step = random.choice(valid)
                # print(step)
                Flip(tmpboard, color, step, valid, count, reverse)
                # print("very important",node.board.matrix)
                color = base.black if color == base.white else base.white

    def __tree_policy(self, node):

        while True:
            # if the current node is the terminal
            if not node.valid:  # 当前棋
                self.state += 1  # 0: 1: 2:
                return node
            # if v not fully expanded
            if node.untried_action:
                # print("当前没有fully expand 的节点",node.move)
                # print("当前没有fully expand 的节点的可走步数",[node.valid])
                node = self.__expand(node)
                # print("当前节点发展的子节点",node.move)
                # print([p.move for p in node.parent.children])
                return node

            else:
                # print("best parent",node.move)
                # print([p.move for p in node.children])
                node = self.__best_child(node, self.c)
                # print("best child",node.move)
        return node

    def __board_copy(self):
        self.simulate_board.matrix = copy.deepcopy(self.board.matrix)

    def __flip(self, step, valid, reverse):
        flip = reverse[valid.index(step)]
        for i, j in flip:
            self.simulate_board.matrix[i][j] = base.black if self.simulate_board.matrix[i][
                                                                 j] == base.white else base.white
            self.simulate_board.matrix[step[0]][step[1]] = self.simulate_player.color

    def __expand(self, node):
        # choose an untried action
        next_action = node.untried_action.pop()
        # add the new child to node
        clr = base.black if node.color == base.white else base.white

        tmpboard = base.board()
        tmpboard.matrix = copy.deepcopy(node.board.matrix)
        Flip(tmpboard, node.color, next_action, node.valid, node.count, node.reverse)

        childnode = tree_node(clr, tmpboard)
        childnode.parent = node
        childnode.move = next_action
        childnode.depth = node.depth + 1
        node.children.append(childnode)
        return childnode

    def __best_child(self, node, c):
        children = node.children
        if node.color == self.player.color:
            # 选取最大值
            # print("enter1")
            value = -10000
            for p in children:
                tmp = p.Qv / p.Nv + 2 * math.sqrt(c * math.log(self.total_time) / p.Nv)
                if tmp > value:
                    value = tmp
                    child = p
        else:
            # print("enter2")
            value = 10000
            for p in children:
                tmp = p.Qv / p.Nv + 2 * math.sqrt(c * math.log(self.total_time) / p.Nv)
                if tmp < value:
                    value = tmp
                    child = p
        return child


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
