import sys, time
from copy import deepcopy


class sudoku:
    sud = [[0 for _ in range(9)] for _ in range(9)]
    is_valid_entry = lambda _, a : a > 0 and a < 10

    def get_row(self, n):
        return self.sud[n]
    
    def get_col(self, n):
        return [i[n] for i in self.sud]
    
    def get_square(self, n):
        sq = [row[3 * (n%3):3 * (n%3+1)] for row in self.sud[3 * (n//3):3 * (n//3+1)]]
        sq = [i for r in sq for i in r]
        return sq

    def row_contain(self, elem, n):
        return elem in self.get_row(n)
    
    def col_contains(self, elem, n):
        return elem in self.get_col(n)

    def square_contains(self, elem, n):
        return elem in self.get_square(n)
    
    def check_row(self, n):
        return all(self.is_valid_entry(i) for i in self.get_row(n))
    
    def check_col(self, n):
        return all(self.is_valid_entry(i) for i in self.get_col(n))
    
    def check_square(self, n):
        return all(self.is_valid_entry(i) for i in self.get_square(n))

    def solve(self):
        self._solve(0)
        for i in range(9):
            if not self.check_col(i) or not self.check_row(i) or not self.check_square(i):
                return False
        
        return True

    def _solve(self, p):
        i = p // 9
        j = p % 9
        while p < 81 and self.sud[i][j] != 0:
            p += 1
            i = p // 9
            j = p % 9

        if p < 81:
            
            sq = (i // 3) * 3 + (j // 3)

            for k in range(1, 10):
                if self.row_contain(k, i) or \
                    self.col_contains(k, j) or \
                    self.square_contains(k, sq):
                    continue
                self.sud[i][j] = k
                
                if self._solve(p+1):
                    return True
                
            self.sud[i][j] = 0
            return False
                    
        # for i in range(9):
        #     if not self.check_col(i) or not self.check_row(i) or not self.check_square(i):
        #         return False
            
        return True
    
    def __init__(self, input):
        self.sud = [list(input[i:i+9]) for i in range(0, len(input), 9)]
        self.sud = [[int(i) for i in r] for r in self.sud]

if __name__ == "__main__":
    if(len(sys.argv) != 3):
        print("Usage: python3 main.py <sudoku> <repetitions>")
        exit(1)

    input = sys.argv[1]
    repetitions = int(sys.argv[2])

    s = sudoku(input)
    orig = deepcopy(s)

    total = 0
    assert(s.solve())

    for _ in range(repetitions):
        s = deepcopy(orig)
        start = time.time_ns()
        s.solve()
        total += time.time_ns() - start
    total //= repetitions
    print(total)