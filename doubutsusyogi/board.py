"""メモリ上の配置
各マスの状態は4bitで表現
4x12=48bitで盤面が表現される
各持ち駒は、Aがひよこを何個(0~2)持っているか(2bit)
をぞう、きりんでも表現することで、6bit、Bも同様で12bitで表現可能
A0 B0 C0 ... C3, Aのひよこ個数, Aのぞう個数, Aのきりん個数, Bのひよこ個数, Bのぞう個数, Bのきりん個数
結果、60bitで局面が表現される。

  Player B
    0  1  2 x
0  A0 B0 C0
1  A1 B1 C1
2  A2 B2 C2
3  A3 B3 C3
y
  Player A
"""

from typing import Set

EMPTY = 0
A_HIYOKO = 1
A_ZOU = 2
A_KIRIN = 3
A_LION = 4
A_NIWATORI = 5
B_HIYOKO = 6
B_ZOU = 7
B_KIRIN = 8
B_LION = 9
B_NIWATORI = 10


def A_hiyoko(board: int) -> int:
    return (board >> 48) & 0b11


def A_zou(board: int) -> int:
    return (board >> 48) & 0b1100


def A_kirin(board: int) -> int:
    return (board >> 48) & 0b110000


def B_hiyoko(board: int) -> int:
    return (board >> 54) & 0b11


def B_zou(board: int) -> int:
    return (board >> 54) & 0b1100


def B_kirin(board: int) -> int:
    return (board >> 54) & 0b110000


def A_hiyoko_inc(board: int) -> int:
    return board + (1 << 48)


def A_hiyoko_dec(board: int) -> int:
    return board - (1 << 48)


def A_zou_inc(board: int) -> int:
    return board + (1 << 50)


def A_zou_dec(board: int) -> int:
    return board - (1 << 50)


def A_kirin_inc(board: int) -> int:
    return board + (1 << 52)


def A_kirin_dec(board: int) -> int:
    return board - (1 << 52)


def get_board_cell(board: int, x: int, y: int) -> int:
    return (board >> (4 * (x + 3 * y))) & 0b1111


def get_board_celli(board: int, i: int) -> int:
    return (board >> (4 * i)) & 0b1111


def set_board_cell(board: int, x: int, y: int, piece: int) -> int:
    return board | (piece << (4 * (x + 3 * y)))


def set_board_celli(board: int, i: int, piece: int) -> int:
    return board | (piece << (4 * i))


def get_next_boards(board: int) -> Set[int]:
    """プレイヤーAの手番で、boardから次の局面を生成する

    Returns:
        list[int]: 次の局面(プレイヤーBの手番) のリスト
    """
    ret: Set[int] = []
    for i in range(12):
        piece = get_board_celli(board, i)
        if piece == EMPTY:
            ret += get_next_boards_empty(board, i)
        if piece == A_HIYOKO:
            ret += get_next_boards_hiyoko(board, i)
        elif piece == A_ZOU:
            ret += get_next_boards_zou(board, i)
        elif piece == A_KIRIN:
            ret += get_next_boards_kirin(board, i)
        elif piece == A_LION:
            ret += get_next_boards_lion(board, i)
        elif piece == A_NIWATORI:
            ret += get_next_boards_niwatori(board, i)
    return ret


def get_next_boards_empty(board: int, i: int) -> Set[int]:
    """Aがiの場所に駒を置くときの局面を考える"""
    ret = set()
    if A_hiyoko(board) > 0:
        new_board = set_board_celli(board, i, A_HIYOKO)
        new_board = A_hiyoko_dec(new_board)
        ret.add(new_board)
    if A_zou(board) > 0:
        new_board = set_board_celli(board, i, A_ZOU)
        new_board = A_zou_dec(new_board)
        ret.add(new_board)
    if A_kirin(board) > 0:
        new_board = set_board_celli(board, i, A_KIRIN)
        new_board = A_kirin_dec(new_board)
        ret.add(new_board)
    return ret


def get_next_boards_hiyoko(board: int, i: int) -> Set[int]:
    """Aがiの場所のひよこを動かすときの局面を考える"""
    ret = set()
    return ret


def board_flip(board: int) -> int:
    """プレイヤーを入れ替える(盤面を180度回転する)"""
    ret: int = 0
    for i in range(12):
        val = get_board_celli(board, i)
        if val == EMPTY:
            ret = set_board_celli(ret, i, EMPTY)
        elif val < B_HIYOKO:
            ret = set_board_celli(ret, i, val + 5)
        else:
            ret = set_board_celli(ret, i, val - 5)
    ret |= ((board >> 48) & 0b111) << (48 + 3)
    ret |= ((board >> 51) & 0b111) << (48 + 0)
    return ret


def get_first_board() -> int:
    board_array = [
        B_KIRIN,
        B_LION,
        B_ZOU,
        EMPTY,
        B_HIYOKO,
        EMPTY,
        EMPTY,
        A_HIYOKO,
        EMPTY,
        A_ZOU,
        A_LION,
        A_KIRIN,
    ]
    ret = 0
    for i in range(12):
        ret |= (board_array[i] & 0b1111) << (4 * i)
    return ret


def board_str(board: int) -> str:
    ret = "\n-----------\n"
    for y in range(4):
        for x in range(3):
            ret += "|"
            piece = get_board_cell(board, x, y)
            if piece == EMPTY:
                ret += "  "
            elif piece == A_HIYOKO:
                ret += "Ha"
            elif piece == A_ZOU:
                ret += "Za"
            elif piece == A_KIRIN:
                ret += "Ka"
            elif piece == A_LION:
                ret += "La"
            elif piece == A_NIWATORI:
                ret += "Na"
            elif piece == B_HIYOKO:
                ret += "Hb"
            elif piece == B_ZOU:
                ret += "Zb"
            elif piece == B_KIRIN:
                ret += "Kb"
            elif piece == B_LION:
                ret += "Lb"
            elif piece == B_NIWATORI:
                ret += "Nb"
            else:
                raise Exception("Invalid piece")
        ret += "|\n"
        ret += "----------\n"
    return ret
