# Python 3 implementation of a Board class that represents a "sliding
# tile" puzzle (usually with 8 tiles on 9 squares).
#
# Based on courseware and the Algorithms textbook from
# Bob Sedgewick and Kevin Wayne.
#
class Board(object):
    '''Each board object, which is immutable, represents one configuration
    of a "sliding tiles" puzzle.

    Each 'tile' is represented by the number on that tile, from 1-N, where
    N=dim**2-1, where dim is the dimension of the puzzle.

    To save a little space, I store the puzzle as a single tuple which has
    the length of the square of the dimension, so a solved 3x3 puzzle is
    stored like this:
    (1, 2, 3, 4, 5, 6, 7, 8, 0)
    This arrangement makes testing for correctness really easy, and makes
    computing the Hamming distance trivial.

    To convert from a linear index 0-9 to a pair of x and y coordinates
    on the puzzle grid, just divide the index by the dimension to get the
    row, and take the remainder of the index by the dimension to get the
    column:
        x, y = k % dim, k // dim
    To convert from a row and a column back to a linear index, just perform
    this calculation:
        k = y * dim + x
    '''
    use_hamming = False          # Controls which metric to use.
    def __init__(self, arg):
        '''Initialize a Board object, either from an iterable or a string
        representation.'''
        try:
            # Assume it's a string representation.
            data = [int(tile) for tile in arg.split()]
            dim = data[0]
            self.__board = tuple(data[1:])
        except AttributeError:
            # If the split fails we'll wind up here.
            dim = int(len(arg) ** 0.5) # dim is the square root of len
            self.__board = tuple(arg)
        self.__dim = dim
        # Compute the distance metrics. We compute them once when the
        # board is created and save the results, rather than recomputing
        # them every time the distance is requested.
        self.__manhattan_distance = self.__compute_manhattan()
        self.__hamming_distance = self.__compute_hamming()

    def __compute_manhattan(self):
        '''Compute the "Manhattan" distance from this board configuration
        to the solved board configuration. This is the total difference
        between the current x and y coordinates and the correct x and y
        coordinates for each tile.'''
        count = 0
        dim = self.__dim
        for k, tile in enumerate(self.__board):
            if tile != 0:
                # compute the current position using 'k'
                tx, ty = k % dim, k // dim
                # compute the correct position using 'tile'
                cx, cy = (tile - 1) % dim, (tile - 1) // dim
                # compute difference
                count += abs(cx - tx) + abs(cy - ty)
        return count

    def __compute_hamming(self):
        '''Compute the Hamming distance from this board configuration
        to the solved board configuration. This is just the total number
        of incorrectly placed tiles. The Hamming distance is easy to
        compute, but it isn't the best measure of distance from the
        solution.'''
        count = 0
        for k, tile in enumerate(self.__board):
            if tile != 0 and tile != k + 1:
                count += 1
        return count

    def __getstate(self):
        '''Make a mutable copy of the state of a board, suitable for
        constructing a new board.'''
        return list(self.__board)

    def dimension(self):
        '''Return the size of the board.'''
        return self.__dim

    def distance(self):
        '''Return the distance from a solved board. Can return either the
        Hamming or Manhattan distance.'''
        if self.use_hamming:
            return self.__hamming_distance
        else:
            return self.__manhattan_distance

    def solved(self):
        '''Return True if the board is solved.'''
        return self.distance() == 0

    def neighbors(self):
        '''Construct a list of boards that can be reached from the current
        board in one step.'''
        result = []
        dim = self.__dim
        dxs = [ 1, -1, 0,  0 ]
        dys = [ 0,  0, 1, -1 ]

        for k, tile in enumerate(self.__board):
            if tile == 0:
                x, y = k % dim, k // dim
                for dx, dy in zip(dxs, dys):
                    new_x, new_y = x + dx, y + dy
                    if 0 <= new_x < dim and 0 <= new_y < dim:
                        temp = self.__getstate()
                        j = new_y * dim + new_x
                        temp[k] = temp[j]
                        temp[j] = 0
                        result.append(Board(temp))
        return tuple(result)

    def __eq__(self, other):
        '''Special method to test equality of two Board objects.'''
        if self is other:       # Identical objects?
            return True
        if other == None:       # Never equal to None
            return False
        if type(self) != type(other): # Check types.
            return False
        if self.__dim != other.__dim: # Compare dimensions.
            return False
        return self.__board == other.__board

    def __repr__(self): #just draw out the matrix from the tuple
        '''Convert a Board object to a string.'''
        print(self.__dim)
        board_str = ""
        counter = 0
        for i in self.__board:
            board_str += str(i) + " "
            counter += 1
            if counter == self.__dim:
                board_str += "\n"
                counter = 0
        return board_str # YOU MUST IMPLEMENT THIS!
