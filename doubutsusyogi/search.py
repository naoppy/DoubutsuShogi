import pickle
from collections import deque
from typing import Dict

from doubutsusyogi.board import (
    board_flip,
    board_normalize,
    get_first_board,
    get_gameover,
    get_next_boards,
)


def search():
    visited_dict: Dict[int, bool] = {}
    first_board = board_normalize(get_first_board())
    visited_dict[first_board] = True
    queue = deque([first_board])
    dist: int = 0  # noqa: F841
    while len(queue) != 0:
        # print(len(queue))
        board = queue.popleft()
        for next in get_next_boards(board):
            next = board_flip(next)
            next = board_normalize(next)
            if next not in visited_dict:
                visited_dict[next] = True
                if not get_gameover(next):
                    queue.append(next)
        # dist += 1
    with open("visited_dict.pkl", "wb") as f:
        pickle.dump(visited_dict, f)
    print(len(visited_dict))


if __name__ == "__main__":
    search()
