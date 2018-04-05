# -*- coding: utf-8 -*-
"""
Created on Tue Apr  3 11:26:32 2018

@author: admin
"""

from component import *
import datetime
import copy
import random
import base

class MonteCarlo(object):
    def __init__(self,game):
        self.game=game#传的game是当前的状态
        self.board=game.board
        self.player=game.player#当前的player
        self.max_times=datetime.timedelta(seconds=60)
        self.max_move=60
        self.state=0
        self.simulate_times=2
    def UctSearch(self):
        valid, count, reverse=check(self.player.color,self.board)
        #key 值为 合法棋步，value为list，其中list[0]为N，即访问的次数 list[1]为 Q，即reward
        children={p:[] for p in valid}
        now=datetime.datetime.utcnow()
        while (datetime.datetime.utcnow()-now) <self.max_times:
            self.Treepolicy(children,valid,count,reverse)
# 将下一步所有合法步数传给Treepolicy
    def Treepolicy(self,children,valid,count, reverse):
        
        if not children:#当前棋手无合法步走
            self.state+=1
            return 
        for child in children:
            #如果没有fully expanded
            if not chidren[child]:
                i=0
                children[child].append(self.simulate_times)
                Qv=0
                #选定下一步
                #cur_color是下一步要下的颜色
                cur_color = base.black if self.player.color == base.white else base.white
                cur_board=board()
                cur_board.matrix=copy.deepcopy(self.board.matrix.copy())
                flip(cur_board,cur_color,child,valid,count,reverse)
                
                while(i<=self.simulate_times):
                    if self.player.color ==self.Simulate(cur_color,cur_board):
                        Qv+=1
                    i+=1
                children[child].append(Qv)
                
    def Simulate(color,board):
        #color是下过的即上一步的颜色
        noMove=0
       
        while True:
            #下一步要下的颜色
            color = base.black if color == base.white else base.white
            valid, count, reverse=check(color,board)
            if not valid:
                noMove+=1
                if noMove==2:
                    return Winner(board.matrix)
            
            else:
                step=random.choice(valid)
                Flip(board,color,step,valid,count,reverse)
                noMove=0
                
def Flip(board,color,step,valid,count,reverse):
    index = valid.index(step)
    flip = reverse[index]
    for i, j in flip:
        board.matrix[i][j] = base.black if board.matrix[i][j] == base.white else base.white
    board.matrix[step[0]][step[1]] = color            
                