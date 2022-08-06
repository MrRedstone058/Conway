import pygame
import random
pygame.init()

screen_length = 150 #python can't handle 300 (max)
fps = 60
run = True
cell_size = 1
chance = 5
pause = False
grid_size = screen_length / cell_size
itr = range(int(grid_size))
g = grid_size - 1

screen = pygame.display.set_mode((screen_length, screen_length), pygame.SCALED)
clock = pygame.time.Clock()
cells = [[] for a in itr]

for i in itr:
	for j in itr:
		d = bool(random.randint(0, chance))
		cells[i].append([d, d])
		
while run:
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			run = False
		if event.type == pygame.KEYDOWN:
			if event.key == pygame.K_SPACE:
				pause = not pause
			if event.key == pygame.K_RETURN:
				for i in cells:
					for j in i:
						j[0] = j[1] = bool(random.randint(0, chance))
								
	if not pause:	
		screen.fill((0, 0, 0))
		
		for x, i in enumerate(cells):
			for y, j in enumerate(i):
				if not j[0]:
					screen.set_at((x, y), (255, 255, 255))
				
				if (x > 0 and x < g and y > 0 and y < g):
					alive = (8 - cells[x - 1][y - 1][0] -
	 							cells[x][y - 1][0] -
	 							cells[x + 1][y - 1][0] -
	 							cells[x - 1][y][0] -
	 							cells[x + 1][y][0] -
	 							cells[x - 1][y + 1][0] -
	 							cells[x][y + 1][0] -
	 							cells[x + 1][y + 1][0])
					if not j[0]:
						if not (4 > alive > 1):
							j[1] = True
					else:
						if alive == 3:
							j[1] = False
				else:
					if not j[0]:
						j[1] = j[0] = True
				
		for i in cells:
			for j in i:
				j[0] = j[1]
				
	clock.tick(fps)
	print(clock.get_fps())
	pygame.display.update()
pygame.quit()
						
