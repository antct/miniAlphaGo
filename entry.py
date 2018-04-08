import pygame, sys, random
from pygame.locals import *
import base
import time
import mcts
import mcts_test


class game(object):
    def __init__(self):
        self.surface = pygame.display.set_mode((base.board_rect.width, base.board_rect.height))
        self.over = False
        self.board = base.board()
        self.player1 = base.player(base.black, base.man)
        self.player2 = base.player(base.white, base.ai)
        self.player = self.player1

    def turn(self):
        self.player = self.player2 if self.player == self.player1 else self.player1

    def wait(self):
        valid, count, reverse = base.board.check(self.player.color, self.board)
        black, white = self.cal()
        if self.player.mode == base.man:
            flag = False
            while True:
                for event in pygame.event.get():
                    if event.type == QUIT:
                        self.quit()
                    if event.type == MOUSEBUTTONDOWN and event.button == 1:
                        x, y = pygame.mouse.get_pos()
                        valid, count, reverse = base.board.check(self.player.color, self.board)
                        row = int((x - base.board_x) / base.cell_width)
                        col = int((y - base.board_y) / base.cell_height)
                        if row < 0 or row > 7 or col < 0 or col > 7 or self.board.matrix[row][col] != base.none or (
                                row, col) not in valid:
                            break
                        flag = True

                x, y = pygame.mouse.get_pos()
                row = int((x - base.board_x) / base.cell_width)
                col = int((y - base.board_y) / base.cell_height)
                self.update(valid, count, (row, col), (black, white))
                pygame.display.update()
                base.clock.tick(40)
                if flag:
                    break
        else:
            mt = mcts.monte_carlo(self)
            row, col = mt.uct_search()

        index = valid.index((row, col))
        flip = reverse[index]
        for i, j in flip:
            self.board.matrix[i][j] = base.black if self.board.matrix[i][
                                                        j] == base.white else base.white

        self.board.matrix[row][col] = self.player.color
        self.turn()

        # self.player.time = time.time()
        valid, count, reverse = base.board.check(self.player.color, self.board)
        black, white = self.cal()

        x, y = pygame.mouse.get_pos()
        row = int((x - base.board_x) / base.cell_width)
        col = int((y - base.board_y) / base.cell_height)
        self.update(valid, count, (row, col), (black, white))

        pygame.display.update()
        base.clock.tick(40)

    def cal(self):
        black, white = 0, 0
        for i in range(8):
            for j in range(8):
                if self.board.matrix[i][j] == base.black:
                    black += 1
                if self.board.matrix[i][j] == base.white:
                    white += 1
        return black, white

    def update(self, valid, count, position, score):
        self.surface.fill((255, 255, 255))
        self.surface.blit(base.board_img, base.board_rect, base.board_rect)
        self.surface.blit(base.black_img, (0, 430, 40, 40), base.board_rect)
        self.surface.blit(base.white_img, (430, 430, 40, 40), base.board_rect)
        self.surface.blit(base.black_img, (0, 0, 40, 40), base.board_rect)
        self.surface.blit(base.white_img, (430, 0, 40, 40), base.board_rect)

        # base.total_end = time.time()
        # time_text = base.promptFont.render(str(int(base.total_end - base.total_start)), True, base.fontBlackColor)
        # time_text_rect = time_text.get_rect()
        # time_text_rect.center = (236, 15)
        # self.surface.blit(time_text, time_text_rect)
        # base.total_end = time.time()

        # if self.player1 == self.player:
        #     cur_time_text = base.promptFont.render(str(int(base.total_end - self.player.time)), True,
        #                                            base.fontWhiteColor)
        #     cur_time_text_rect = cur_time_text.get_rect()
        #     cur_time_text_rect.center = (20, 20)
        # else:
        #     cur_time_text = base.promptFont.render(str(int(base.total_end - self.player.time)), True,
        #                                            base.fontBlackColor)
        #     cur_time_text_rect = cur_time_text.get_rect()
        #     cur_time_text_rect.center = (450, 20)
        # self.surface.blit(cur_time_text, cur_time_text_rect)

        black_text = base.promptFont.render(str(score[0]), True, base.fontWhiteColor)
        black_text_rect = black_text.get_rect()
        black_text_rect.center = (20, 450)
        self.surface.blit(black_text, black_text_rect)

        white_text = base.promptFont.render(str(score[1]), True, base.fontBlackColor)
        white_text_rect = white_text.get_rect()
        white_text_rect.center = (450, 450)
        self.surface.blit(white_text, white_text_rect)
        for i in range(8):
            for j in range(8):
                rect = pygame.Rect(base.board_x + i * base.cell_width + 2, base.board_y + j * base.cell_height + 2,
                                   base.piece_width, base.piece_height)
                prompt_rect = pygame.Rect(base.board_x - 2 + i * base.cell_width,
                                          base.board_y - 2 + j * base.cell_height,
                                          base.piece_width + 2, base.piece_height + 1)
                if self.board.matrix[i][j] == base.black:
                    self.surface.blit(base.black_img, rect, base.black_rect)
                if self.board.matrix[i][j] == base.white:
                    self.surface.blit(base.white_img, rect, base.white_rect)

                if (i, j) in valid:
                    if (i, j) == position:
                        pygame.draw.rect(self.surface, [255, 0, 0], prompt_rect, 0)
                    else:
                        pygame.draw.rect(self.surface, [255, 0, 0], prompt_rect, 2)
                    text = base.basicFont.render(str(count[valid.index((i, j))]), True, base.fontBlackColor)
                    text_rect = text.get_rect()
                    text_rect.center = (
                        base.board_x + (i + 0.5) * base.cell_width - 2, base.board_y + (j + 0.5) * base.cell_height - 4)
                    self.surface.blit(text, text_rect)

    def quit(self):
        pygame.quit()
        sys.exit()

    def run(self):
        base.total_start = self.player.time = time.time()
        while True:
            self.wait()
