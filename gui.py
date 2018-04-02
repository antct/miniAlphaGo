import pygame, sys, random
from pygame.locals import *
import base
#test


class board(object):
    def __init__(self):
        self.board = []
        self.valid = []
        self.surface = pygame.display.set_mode((base.board_rect.width, base.board_rect.height))
        for i in range(8):
            self.board.append([None] * 8)
        self.board[3][3] = True  #black
        self.board[3][4] = False
        self.board[4][3] = False
        self.board[4][4] = True
        self.over = False
        self.player = False

    def turn(self):
        self.player = not self.player
        return self.player

    def cal(self):
        black, white = 0, 0
        for i in range(8):
            for j in range(8):
                if self.board[i][j] == True:
                    black += 1
                elif self.board[i][j] == False:
                    white += 1
        print(black, white)

    def update(self):
        for i in range(8):
            for j in range(8):
                rect = pygame.Rect(base.board_x + i * base.cell_width + 2, base.board_y + j * base.cell_height + 2,
                                          base.piece_width, base.piece_height)
                if self.board[i][j] == True:
                    self.surface.blit(base.black_img, rect, base.black_rect)
                elif self.board[i][j] == False:
                    self.surface.blit(base.white_img, rect, base.white_rect)

    def check(self):
        for i in range(8):
            for j in range(8):
                if self.board[i][j] != None:




    def run(self):
        pygame.init()
        clock = pygame.time.Clock()
        while True:
            for event in pygame.event.get():
                if event.type == QUIT:
                    quit()
                if event.type == MOUSEBUTTONDOWN and event.button == 1:
                    x, y = pygame.mouse.get_pos()
                    row = int((x - base.board_x) / base.cell_width)
                    col = int((y - base.board_y) / base.cell_height)
                    if self.board[row][col] != None:
                        break
                    self.board[row][col] = self.turn()
                    self.cal()
                    self.valid()

            self.surface.fill((255, 255, 255))
            self.surface.blit(base.board_img, base.board_rect, base.board_rect)

            self.update()

            pygame.display.update()
            clock.tick(40)

    def quit(self):
        pygame.quit()
        sys.exit()
