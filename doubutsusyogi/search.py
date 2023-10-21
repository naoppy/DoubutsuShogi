import pickle
from collections import deque
from typing import Dict, Set

from doubutsusyogi.board import (
    board_flip,
    board_normalize,
    get_first_board,
    get_gameover,
    get_next_boards,
)


def search():
    visited_set: Set[int] = set()
    first_board = board_normalize(get_first_board())
    visited_set.add(first_board)
    queue = deque([first_board])
    dist: int = 0  # noqa: F841
    while len(queue) != 0:
        # print(len(queue))
        board = queue.popleft()
        nexts = get_next_boards(board)
        gameovers = filter(get_gameover, nexts)
        contains_gameover = False
        # 勝ち確定の局面がある場合は見逃さない
        for end_board in gameovers:
            end_board = board_flip(end_board)
            end_board = board_normalize(end_board)
            visited_set.add(end_board)
            contains_gameover = True
        if contains_gameover:
            continue
        # 次のの局面で勝ちが確定しない場合は、次の1手を全て探索
        for next in nexts:
            next = board_flip(next)
            next = board_normalize(next)
            if next not in visited_set:
                visited_set.add(next)
                queue.append(next)
        # dist += 1
    with open("visited_dict.pkl", "wb") as f:
        pickle.dump(visited_set, f)
    print(len(visited_set))


if __name__ == "__main__":
    search()
