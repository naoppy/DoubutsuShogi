import pickle
from collections import deque
from typing import Dict

from doubutsusyogi.board import board_flip, get_first_board, get_next_boards


def search():
    visited_dict: Dict[int, bool] = {}
    first_board = get_first_board()
    visited_dict[first_board] = True
    queue = deque([first_board])
    dist: int = 0
    while len(queue) != 0:
        print(dist)
        print(len(queue))
        board = queue.popleft()
        for next in get_next_boards(board):
            next = board_flip(next)
            if next not in visited_dict:
                visited_dict[next] = True
                queue.append(next)
        dist += 1
    with open("visited_dict.pickle", "wb") as f:
        pickle.dump(visited_dict, f)


if __name__ == "__main__":
    search()
