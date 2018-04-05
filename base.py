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
tie="tie"
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
weight=[[1,5,3,3,3,3,5,1],[5,5,4,4,4,4,5,5],[3,4,2,2,2,2,4,3],[3,4,2,1,1,2,4,3],[3,4,2,1,1,2,4,3],[3,4,2,2,2,2,4,3],[5,5,4,4,4,4,5,5],[1,5,3,3,3,3,5,1]]