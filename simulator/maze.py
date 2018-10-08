# !!!!!!!!!!!!!
# Copied from https://github.com/AlexandruValeanu/Mazify
# Adapted to suit my needs
# !!!!!!!!!!!!!


from numpy.random import random_integers as rand
from draw_maze import ascii_representation
from constants import *
from worker import Worker
import pygame


# Define some colors
WHITE = 0
BLACK = 1
WORKER_1 = 2
WORKER_2 = 3
PATH = 4

class Maze:

    def __init__(self, rows, columns):
        assert rows >= 1 and columns >= 1
        self.nrows = rows
        self.ncolumns = columns
        self.board = np.zeros((rows, columns), dtype=WALL_TYPE)
        self.board.fill(EMPTY)
        self.workers_positions = {}
        self.workers_orientation = {}


    # # # # # # # # # # # BEGIN ADDINGS  # # # # # # # # # # # # # #

    def generate_workers(self, n_workers):
        worker_id_1 = 0
        self.workers_positions[worker_id_1] = (5,4)
        self.workers_orientation[worker_id_1] = 'N'

        worker_id_2 = 1
        self.workers_positions[worker_id_2] = (2,9)
        self.workers_orientation[worker_id_2] = 'W'

        return (worker_id_1, worker_id_2)


    def get_sensors_vector(self, worker_id):
        (i, j) = self.workers_positions[worker_id]
        orientation = self.workers_orientation[worker_id]
        
        if orientation == 'N':
            left_path = not self.is_wall(i,j-1) and self.in_maze(i,j-1)
            right_path = not self.is_wall(i,j+1) and self.in_maze(i,j+1)
            front_path = not self.is_wall(i-1,j) and self.in_maze(i-1,j)
        elif orientation == 'E':
            left_path = not self.is_wall(i-1,j) and self.in_maze(i-1,j)
            right_path = not self.is_wall(i+1,j) and self.in_maze(i,j)
            front_path = not self.is_wall(i,j+1) and self.in_maze(i,j+1)
        elif orientation == 'S':
            left_path = not self.is_wall(i,j+1) and self.in_maze(i,j+1)
            right_path = not self.is_wall(i,j-1) and self.in_maze(i,j-1)
            front_path = not self.is_wall(i+1,j) and self.in_maze(i+1,j)
        elif orientation == 'W':
            left_path = not self.is_wall(i+1,j) and self.in_maze(i+1,j)
            right_path = not self.is_wall(i-1,j) and self.in_maze(i-1,j)
            front_path = not self.is_wall(i,j-1) and self.in_maze(i,j-1)

        current_path = EMPTY
        return (current_path, left_path, right_path, front_path)


    def turn_left(self, worker_id):
        orientation = self.workers_orientation[worker_id]
        new_orientation = orientation
        if orientation == 'N':
            new_orientation = 'W'
        elif orientation == 'S':
            new_orientation = 'E'
        elif orientation == 'W':
            new_orientation = 'S'
        elif orientation == 'E':
            new_orientation = 'N'

        self.workers_orientation[worker_id] = new_orientation



    def turn_right(self, worker_id):
        orientation = self.workers_orientation[worker_id]
        new_orientation = orientation
        if orientation == 'N':
            new_orientation = 'E'
        elif orientation == 'S':
            new_orientation = 'W'
        elif orientation == 'W':
            new_orientation = 'N'
        elif orientation == 'E':
            new_orientation = 'S'

        self.workers_orientation[worker_id] = new_orientation


    def turn_back(self, worker_id):
        orientation = self.workers_orientation[worker_id]
        new_orientation = orientation
        if orientation == 'N':
            new_orientation = 'S'
        elif orientation == 'S':
            new_orientation = 'N'
        elif orientation == 'W':
            new_orientation = 'E'
        elif orientation == 'E':
            new_orientation = 'W'

        self.workers_orientation[worker_id] = new_orientation



    def go_straight(self, worker_id):
        (i, j) = self.workers_positions[worker_id]
        orientation = self.workers_orientation[worker_id]

        (new_i, new_j) = (i, j)
        if orientation == 'N':
            new_i -= 1
        elif orientation == 'S':
            new_i += 1
        elif orientation == 'W':
            new_j -= 1
        elif orientation == 'E':
            new_j += 1

        self.workers_positions[worker_id] = (new_i, new_j)


    def get_grid(self):
        grid = [[WHITE for column in 
            range(self.ncolumns)] for row in range(self.nrows)]
        for row in range(self.nrows):
            for column in range(self.ncolumns):
                color = BLACK

                if self.is_wall(row, column):
                    color = WHITE

                for i in range(len(self.workers_positions)):
                    position = self.workers_positions[i]
                    if (row, column) == position:
                        if i == 0:
                            color = WORKER_1
                        else: 
                            color = WORKER_2
                        break 

                grid[row][column] = color
        grid = np.array(grid)
        return (grid, grid.shape)


    # # # # # # # # # # # END ADDINGS  # # # # # # # # # # # # # #


    def set_borders(self):
        self.board[0, :] = self.board[-1, :] = WALL
        self.board[:, 0] = self.board[:, -1] = WALL


    def is_wall(self, x, y):
        assert self.in_maze(x, y)
        return self.board[x][y] == WALL


    def set_wall(self, x, y):
        assert self.in_maze(x, y)
        self.board[x][y] = WALL


    def remove_wall(self, x, y):
        assert self.in_maze(x, y)
        self.board[x][y] = EMPTY


    def in_maze(self, x, y):
        return 0 <= x < self.nrows and 0 <= y < self.ncolumns


    def write_to_file(self, filename):
        f = open(filename, 'w')
        f.write(ascii_representation(self))
        f.close()


    @staticmethod
    def load_from_file(filename):
        with open(filename, 'r') as f:
            content = f.readlines()

        # remove whitespace characters like `\n` at the end of each line
        content = [x.strip() for x in content]

        xss = []
        for line in content:
            xs = []

            for c in line:
                if c == ' ':
                    xs.append(EMPTY)
                elif c == 'X':
                    xs.append(WALL)
                else:
                    raise ValueError('unexpected character found: ' + c)

            xss.append(xs)

        maze = Maze(len(xss), len(xss[0]))

        for xs in xss:
            assert len(xs) == maze.ncolumns

        for i in range(maze.nrows):
            for j in range(maze.ncolumns):
                if xss[i][j] == EMPTY:
                    maze.remove_wall(i, j)
                else:
                    maze.set_wall(i, j)

        return maze


    @staticmethod
    def complete_maze(rows, columns):
        maze = Maze(rows, columns)

        for i in range(rows):
            for j in range(columns):
                maze.board[i][j] = WALL

        return maze


    @staticmethod
    def create_maze(rows, columns, seed=None, complexity=.5, density=.2):
        rows = (rows // 2) * 2 + 1
        columns = (columns // 2) * 2 + 1

        if seed is not None:
            np.random.seed(seed)

        # Adjust complexity and density relative to maze size
        complexity = int(complexity * (5 * (rows + columns)))
        density = int(density * ((rows // 2) * (columns // 2)))

        maze = Maze(rows, columns)
        maze.set_borders()

        # Make aisles
        for i in range(density):
            x, y = rand(0, rows // 2) * 2, rand(0, columns // 2) * 2
            maze.set_wall(x, y)

            for j in range(complexity):
                neighbours = []

                if maze.in_maze(x - 2, y):
                    neighbours.append((x - 2, y))

                if maze.in_maze(x + 2, y):
                    neighbours.append((x + 2, y))

                if maze.in_maze(x, y - 2):
                    neighbours.append((x, y - 2))

                if maze.in_maze(x, y + 2):
                    neighbours.append((x, y + 2))

                if len(neighbours):
                    next_x, next_y = neighbours[rand(0, len(neighbours) - 1)]

                    if not maze.is_wall(next_x, next_y):
                        maze.set_wall(next_x, next_y)
                        maze.set_wall(next_x + (x - next_x) // 2, next_y + (y - next_y) // 2)
                        x, y = next_x, next_y

        return maze
