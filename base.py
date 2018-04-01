import pygame

board_img = pygame.image.load('./assets/board.png')
black_img = pygame.image.load('./assets/black.png')
white_img = pygame.image.load('./assets/white.png')
board_rect = board_img.get_rect()
black_rect = black_img.get_rect()
white_rect = white_img.get_rect()

pygame.display.set_caption('Reversi')

cell_width, cell_height = 50, 50
piece_width, piece_height = 48, 48
board_x, board_y = 40, 38
