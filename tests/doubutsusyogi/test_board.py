from doubutsusyogi.board import (
    A_HIYOKO,
    A_KIRIN,
    A_LION,
    B_HIYOKO,
    B_KIRIN,
    B_LION,
    B_ZOU,
    EMPTY,
    A_hiyoko,
    A_kirin,
    A_zou,
    B_hiyoko,
    B_kirin,
    B_zou,
    board_flip,
    board_str,
    get_board_cell,
    get_first_board,
    move,
)


def test_first_board_valid():
    board = get_first_board()
    assert A_hiyoko(board) == 0
    assert A_zou(board) == 0
    assert A_kirin(board) == 0
    assert B_hiyoko(board) == 0
    assert B_zou(board) == 0
    assert B_kirin(board) == 0
    assert get_board_cell(board, 0, 0) == B_KIRIN
    assert get_board_cell(board, 1, 0) == B_LION
    assert get_board_cell(board, 2, 0) == B_ZOU
    assert get_board_cell(board, 1, 1) == B_HIYOKO
    assert get_board_cell(board, 1, 2) == A_HIYOKO
    assert get_board_cell(board, 0, 1) == EMPTY
    assert get_board_cell(board, 1, 3) == A_LION
    assert get_board_cell(board, 2, 3) == A_KIRIN


def test_board_flip():
    board = get_first_board()
    flip_board = board_flip(board)
    assert get_board_cell(flip_board, 1, 1) == B_HIYOKO
    assert get_board_cell(flip_board, 1, 2) == A_HIYOKO
    flip_flip_board = board_flip(flip_board)
    assert board == flip_flip_board


def test_str():
    board = get_first_board()
    str_board = board_str(board)
    print(str_board)
    assert len(str_board) > 12 * 3


def test_get_new_piece():
    board = get_first_board()
    print(board_str(board))
    assert get_board_cell(board, 1, 1) == B_HIYOKO
    board = move(board, 1 + 3 * 2, 1 + 3 * 1)
    print(board_str(board))
    assert get_board_cell(board, 1, 1) == A_HIYOKO
    assert get_board_cell(board, 1, 2) == EMPTY
    assert A_hiyoko(board) == 1
    assert A_zou(board) == 0
    assert A_kirin(board) == 0
