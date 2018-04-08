import pygame

board_img = pygame.image.load('./assets/board.png')
black_img = pygame.image.load('./assets/black.png')
white_img = pygame.image.load('./assets/white.png')
board_rect = board_img.get_rect()
black_rect = black_img.get_rect()
white_rect = white_img.get_rect()

pygame.init()
clock = pygame.time.Clock()
pygame.display.set_caption('Reversi')

cell_width, cell_height = 50, 50
piece_width, piece_height = 49, 48
board_x, board_y = 39, 38

black = "black"
white = "white"
none = "none"
tie = "tie"
man = "man"
ai = "ai"

total_start, total_end = 0, 0

basicFont = pygame.font.Font('./assets/SourceHanSans-Regular.otf', 30)
promptFont = pygame.font.Font('./assets/SourceHanSans-Regular.otf', 20)
fontBlackColor = (0, 0, 0)
fontWhiteColor = (255, 255, 255)
fontBgColor = (255, 255, 255)

direction = [(0, -1), (0, 1), (-1, 0), (1, 0), (-1, -1), (-1, 1), (1, -1), (1, 1)]
init_valid = [(2, 4), (3, 5), (4, 2), (5, 3)]
init_count = [1, 1, 1, 1]
weight = [[1, 5, 3, 3, 3, 3, 5, 1], [5, 5, 4, 4, 4, 4, 5, 5], [3, 4, 2, 2, 2, 2, 4, 3], [3, 4, 2, 1, 1, 2, 4, 3],
          [3, 4, 2, 1, 1, 2, 4, 3], [3, 4, 2, 2, 2, 2, 4, 3], [5, 5, 4, 4, 4, 4, 5, 5], [1, 5, 3, 3, 3, 3, 5, 1]]


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
            self.matrix.append([none] * 8)
        self.matrix[3][3] = black
        self.matrix[3][4] = white
        self.matrix[4][3] = white
        self.matrix[4][4] = black

    @staticmethod
    def winner(board):
        black = 0
        white = 0
        for i in range(8):
            for j in range(8):
                if not board.matrix[i][j] == none:
                    if board.matrix[i][j] == black:
                        black += 1
                    else:
                        white += 1
        if black > white:
            return black
        elif black < white:
            return white
        else:
            return tie

    @staticmethod
    def check(color, board):
        aim_color = black if color == white else white
        valid = []
        count = []
        reverse = []
        for i in range(8):
            for j in range(8):
                if board.matrix[i][j] != none:
                    continue
                find_flag = False
                total_count = 0
                flip_list = []
                for k in range(8):
                    flip_save = flip_list.copy()
                    x = i + direction[k][0]
                    y = j + direction[k][1]
                    flip_count = 0
                    while 0 <= x <= 7 and 0 <= y <= 7 and board.matrix[x][y] == aim_color:
                        flip_list.append((x, y))
                        x += direction[k][0]
                        y += direction[k][1]
                        flip_count += 1
                    if x == i + direction[k][0] and y == j + direction[k][1]:
                        continue
                    if x < 0 or x > 7 or y < 0 or y > 7 or board.matrix[x][y] == none:
                        flip_list = flip_save
                        continue
                    find_flag = True
                    total_count += flip_count
                if find_flag is True:
                    valid.append((i, j))
                    count.append(total_count)
                    reverse.append(flip_list)
        return valid, count, reverse
