"""メモリ上の配置
各マスの状態は4bitで表現
4x12=48bitで盤面が表現される
各持ち駒は、Aがひよこを何個(0~2)持っているか(2bit)
をぞう、きりんでも表現することで、6bit、Bも同様で12bitで表現可能
結果、60bitで局面が表現される。
        x
A0 B0 C0
A1 B1 C1
A2 B2 C2
A3 B3 C3
y
"""

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


def get_board(board: int, x: int, y: int) -> int:
    return (board >> (4 * (x + 3 * y))) & 0b1111


def set_board(board: int, x: int, y: int, piece: int) -> int:
    return board | (piece << (4 * (x + 3 * y)))


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
