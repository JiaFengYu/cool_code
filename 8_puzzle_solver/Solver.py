# Main program for assignment 2. Your job here is to finish the
# Solver class's __init__ method to solve the puzzle as described
# in the handout.
#
from MinPQ import MinPQ
from Board import Board

import functools
@functools.total_ordering
class Node(object):
    def __init__(self, bd, moves, node):
        '''Construct a new node object. A node contains four attributes:
        1. The board associated with this node (Board).
        2. The number of moves to reach this node (int).
        3. The cost or distance metric for this node (int).
        4. The previous node (Node).
        '''
        self.board = bd         # save the board
        self.moves = moves      # number of moves to reach this board.
        self.cost = bd.distance() # save the distance metric.
        self.previous = node      # save the previous node.
    def __gt__(self, other):
        '''A node is 'greater' than another if the cost plus the
        number of moves is larger. Note that this code will fail
        if 'other' is None.'''
        return (self.cost + self.moves) > (other.cost + other.moves)
    def __eq__(self, other):
        '''Two nodes are equal if the sum of the cost and moves are
        the same. The board itself is ignored.'''
        if self is other:       # comparing to itself?
            return True
        if other is None:       # comparing to None
            return False
        return (self.cost + self.moves) == (other.cost + other.moves)

class Solver(object):
    def __init__(self, initial):
        '''Initialize the object by finding the solution for the
        puzzle.'''
        self.__solvable = False
        self.__trace = []       # List of Board objects.
        a = MinPQ()
        b = Node(initial,0, None)
        a.insert(b)
        counter = 0
        while len(a) != 0:
            c = a.delete()
            if c.board.solved():
                self.__solvable = True
                self.__trace.append(c.board)
                node = c
                while node.previous != None:
                    self.__trace.append(node.previous.board)
                    node = node.previous
                break
            else:
                for i in c.board.neighbors():
                    if c.previous == None:
                        new = Node(i,1,c)
                        a.insert(new)
                    else:
                        new = Node(i,c.moves+1, c)
                        if i != c.previous.board:
                            a.insert(new)
            counter += 1
        print(counter)
    def solvable(self):
        '''Returns True if this puzzle can be solved.'''
        return self.__solvable;

    def moves(self):
        '''Returns the number of moves in the solution, or -1 if
        not solvable.'''
        return len(self.__trace) - 1 if self.__solvable else -1

    def solution(self):
        '''Returns the list of board positions in the solution.'''
        return self.__trace.copy()

# Add your main program here. It should prompt for a file name, read
# the file, and create and run the Solver class to find the solution
# for the puzzle. Then it should print the result (see the example output
# file for details).
#
prompt = input("Puzzle to solve: ")
fp = open(prompt + ".txt")
new_board = Board(fp.read())
a = Solver(new_board)
print("Minimum number of moves = " + str(a.moves()))
counter = 0
for move in reversed(a.solution()):
    print("Move #" + str(counter))
    print(move)
    counter += 1
