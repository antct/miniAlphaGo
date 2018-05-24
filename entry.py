import pygame
import sys
from pygame.locals import *
import base
import time
import mcts


class game(object):
    def __init__(self):
        #print(sys.argv[1])
        self.surface = pygame.display.set_mode((base.board_rect.width, base.board_rect.height))
        self.over = False
        self.board = base.board()
        if sys.argv[1]=='man':
            print("man")
            self.player1 = base.player(base.black, base.man)
            self.player2 = base.player(base.white, base.ai)
        else:
            print("ai")
            self.player1 = base.player(base.black, base.ai)
            self.player2 = base.player(base.white, base.man)
        self.player = self.player1

    def __turn(self):
        self.player = self.player2 if self.player == self.player1 else self.player1

    def __flip(self, row, col):
        valid, count, reverse = base.board.check(self.player.color, self.board)
        index = valid.index((row, col))
        flip = reverse[index]
        for i, j in flip:
            self.board.matrix[i][j] = base.black if self.board.matrix[i][j] == base.white else base.white
        self.board.matrix[row][col] = self.player.color

    def __count(self):
        black, white = 0, 0
        for i in range(8):
            for j in range(8):
                if self.board.matrix[i][j] == base.black:
                    black += 1
                if self.board.matrix[i][j] == base.white:
                    white += 1
        return black, white

    def quit(self):
        pygame.quit()
        sys.exit()

    def __update(self):
        black, white = self.__count()
        x, y = pygame.mouse.get_pos()
        valid, count, reverse = base.board.check(self.player.color, self.board)
        row = int((x - base.board_x) / base.cell_width)
        col = int((y - base.board_y) / base.cell_height)

        # 画背景和棋子
        self.surface.fill((255, 255, 255))
        self.surface.blit(base.board_img, base.board_rect, base.board_rect)
        self.surface.blit(base.black_img, (0, 430, 40, 40), base.board_rect)
        self.surface.blit(base.white_img, (430, 430, 40, 40), base.board_rect)
        self.surface.blit(base.black_img, (0, 0, 40, 40), base.board_rect)
        self.surface.blit(base.white_img, (430, 0, 40, 40), base.board_rect)

        # 总的运行时间
        base.total_end = time.time()
        total_time = base.promptFont.render(str(int(base.total_end - base.total_start)), True, base.fontBlackColor)
        total_time_rect = total_time.get_rect()
        total_time_rect.center = (236, 15)
        self.surface.blit(total_time, total_time_rect)

        # 当前运行时间
        cur_time_color = base.fontWhiteColor if self.player1 == self.player else base.fontBlackColor
        cur_time_center = (20, 20) if self.player1 == self.player else (450, 20)
        cur_time = base.promptFont.render(str(int(base.total_end - self.player.time)), True, cur_time_color)
        cur_time_rect = cur_time.get_rect()
        cur_time_rect.center = cur_time_center
        self.surface.blit(cur_time, cur_time_rect)

        black_text = base.promptFont.render(str(black), True, base.fontWhiteColor)
        black_text_rect = black_text.get_rect()
        black_text_rect.center = (20, 450)
        self.surface.blit(black_text, black_text_rect)

        white_text = base.promptFont.render(str(white), True, base.fontBlackColor)
        white_text_rect = white_text.get_rect()
        white_text_rect.center = (450, 450)
        self.surface.blit(white_text, white_text_rect)

        for i in range(8):
            for j in range(8):
                cur_x = base.board_x + i * base.cell_width + 2
                cur_y = base.board_y + j * base.cell_height + 2
                rect = pygame.Rect(cur_x, cur_y, base.piece_width, base.piece_height)
                cur_x = base.board_x - 2 + i * base.cell_width
                cur_y = base.board_y - 2 + j * base.cell_height
                prompt_rect = pygame.Rect(cur_x, cur_y, base.piece_width + 2, base.piece_height + 1)
                if self.board.matrix[i][j] == base.black:
                    self.surface.blit(base.black_img, rect, base.black_rect)
                if self.board.matrix[i][j] == base.white:
                    self.surface.blit(base.white_img, rect, base.white_rect)

                if (i, j) in valid:
                    if (i, j) == (row, col):
                        pygame.draw.rect(self.surface, [255, 0, 0], prompt_rect, 0)
                    else:
                        pygame.draw.rect(self.surface, [255, 0, 0], prompt_rect, 2)
                    text = base.basicFont.render(str(count[valid.index((i, j))]), True, base.fontBlackColor)
                    text_rect = text.get_rect()
                    center_x = base.board_x + (i + 0.5) * base.cell_width - 2
                    center_y = base.board_y + (j + 0.5) * base.cell_height - 4
                    text_rect.center = (center_x, center_y)
                    self.surface.blit(text, text_rect)

        pygame.display.update()
        base.clock.tick(40)

    def run(self):
        base.total_start = self.player.time = time.time()
        next_row, next_col = 0, 0
        while not self.over:
            self.player.time = time.time()
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
                            if row < 0 or row > 7 or col < 0 or col > 7:
                                break
                            if self.board.matrix[row][col] != base.none:
                                break
                            if (row, col) not in valid:
                                break
                            flag = True
                            next_row, next_col = row, col
                    self.__update()
                    if flag:
                        break
            else:
                
                mt = mcts.mcts_thread(self)
                mt.setDaemon(True)
                mt.start()
                while mt.is_alive():
                    for event in pygame.event.get():
                        if event.type == QUIT:
                            self.quit()
                    self.__update()
                next_row, next_col = mt.get_result()
                print("final calculated step: 行： 列：" ,next_col,next_row)
                # mt = mcts.monte_carlo(self)
                # next_row, next_col = mt.uct_search()

            self.__flip(next_row, next_col)
            self.__turn()
            self.player.time = time.time()
            self.__update()
