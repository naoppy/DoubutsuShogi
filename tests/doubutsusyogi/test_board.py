from doubutsusyogi.board import *

def test_first_board_valid():
    board = get_first_board()
    assert A_hiyoko(board) == 0
    assert A_zou(board) == 0
    assert A_kirin(board) == 0
    assert B_hiyoko(board) == 0
    assert B_zou(board) == 0
    assert B_kirin(board) == 0
    assert get_board(board, 0, 0) == B_KIRIN
    assert get_board(board, 1, 0) == B_LION
    assert get_board(board, 2, 0) == B_ZOU
    assert get_board(board, 0, 1) == EMPTY
    assert get_board(board, 1, 3) == A_LION
    assert get_board(board, 2, 3) == A_KIRIN
