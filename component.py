import pygame, sys, random
from pygame.locals import *
import base
import time

def Winner(matrix):
    black = 0
    white = 0
    for i in range(8):
        for j in range(8):
            if not matrix[i][j] == base.none :
                if matrix[i][j] == base.black:
                    black += 1
                else:
                    white += 1

    if black > white:
        return base.black
    elif black < white:
        return base.white
    else:
        return base.tie
  

#judge the next legal steps
def check(color,board):
        aim_color = base.black if color == base.white else base.white
        valid = []
        count = []
        reverse = []
        for i in range(8):
            for j in range(8):
                if board.matrix[i][j] != base.none:
                    continue
                find_flag = False
                total_count = 0
                flip_list = []
                for k in range(8):
                    flip_save = flip_list.copy()
                    x = i + base.direction[k][0]
                    y = j + base.direction[k][1]
                    flip_count = 0
                    while 0 <= x <= 7 and 0 <= y <= 7 and board.matrix[x][y] == aim_color:
                        flip_list.append((x, y))
                        x += base.direction[k][0]
                        y += base.direction[k][1]
                        flip_count += 1
                    if x == i + base.direction[k][0] and y == j + base.direction[k][1]:
                        continue
                    if x < 0 or x > 7 or y < 0 or y > 7 or board.matrix[x][y] == base.none:
                        flip_list = flip_save
                        continue
                    find_flag = True
                    total_count += flip_count
                if find_flag is True:
                    valid.append((i, j))
                    count.append(total_count)
                    reverse.append(flip_list)
        return valid, count, reverse 
    
class player(object):
    def __init__(self, color, mode):
        self.color = color
        self.mode = mode
        self.count = 0
        self.time = 0


class board(object):
    def __init__(self):
        self.matrix = []
        for i in range(8):
            self.matrix.append([base.none] * 8)
        self.matrix[3][3] = base.black
        self.matrix[3][4] = base.white
        self.matrix[4][3] = base.white
        self.matrix[4][4] = base.black


class game(object):
    def __init__(self):
        self.surface = pygame.display.set_mode((base.board_rect.width, base.board_rect.height))
        self.over = False
        self.board = board()
        self.player1 = player(base.black, base.man)
        self.player2 = player(base.white, base.man)
        self.player = self.player1

    def turn(self):
        self.player = self.player2 if self.player == self.player1 else self.player1

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

        base.total_end = time.time()
        time_text = base.promptFont.render(str(int(base.total_end - base.total_start)), True, base.fontBlackColor)
        time_text_rect = time_text.get_rect()
        time_text_rect.center = (236, 15)
        self.surface.blit(time_text, time_text_rect)
        base.total_end = time.time()

        if self.player1 == self.player:
            cur_time_text = base.promptFont.render(str(int(base.total_end - self.player.time)), True,
                                                   base.fontWhiteColor)
            cur_time_text_rect = cur_time_text.get_rect()
            cur_time_text_rect.center = (20, 20)
        else:
            cur_time_text = base.promptFont.render(str(int(base.total_end - self.player.time)), True,
                                                   base.fontBlackColor)
            cur_time_text_rect = cur_time_text.get_rect()
            cur_time_text_rect.center = (450, 20)
        self.surface.blit(cur_time_text, cur_time_text_rect)

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
        valid, count, reverse = check(self.player.color,self.board)
        black, white = self.cal()
        base.total_start = self.player.time = time.time()
        while True:
            for event in pygame.event.get():
                if event.type == QUIT:
                    self.quit()
                if event.type == MOUSEBUTTONDOWN and event.button == 1:
                    x, y = pygame.mouse.get_pos()
                    valid, count, reverse = check(self.player.color,self.board)
                    row = int((x - base.board_x) / base.cell_width)
                    col = int((y - base.board_y) / base.cell_height)
                    if row < 0 or row > 7 or col < 0 or col > 7 or self.board.matrix[row][col] != base.none or (
                            row, col) not in valid:
                        break
                    index = valid.index((row, col))
                    flip = reverse[index]
                    for i, j in flip:
                        self.board.matrix[i][j] = base.black if self.board.matrix[i][j] == base.white else base.white

                    self.board.matrix[row][col] = self.player.color
                    self.turn()

                    self.player.time = time.time()
                    valid, count, reverse = check(self.player.color,self.board)
                    black, white = self.cal()

            x, y = pygame.mouse.get_pos()
            row = int((x - base.board_x) / base.cell_width)
            col = int((y - base.board_y) / base.cell_height)
            self.update(valid, count, (row, col), (black, white))

            pygame.display.update()
            base.clock.tick(40)
            
    
    
