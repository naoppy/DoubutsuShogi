#pragma once
/*
"""メモリ上の配置
各マスの状態は4bitで表現
4x12 = 48bitで盤面が表現される
各持ち駒は、Aがひよこを何個(0~2)持っているか(2bit)
をぞう、きりんでも表現することで、6bit、Bも同様で12bitで表現可能
A0 B0 C0 ... C3, Aのひよこ個数, Aのぞう個数, Aのきりん個数, Bのひよこ個数, Bのぞう個数, Bのきりん個数
結果、60bitで局面が表現される。
1bit終局フラグをつける。1なら終局、0なら終局していない。

Player B
0  1  2 x
0  A0 B0 C0
1  A1 B1 C1
2  A2 B2 C2
3  A3 B3 C3
y
Player A
"""*/

#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <unordered_set>

const int64_t EMPTY = 0;
const int64_t A_HIYOKO = 1;
const int64_t A_ZOU = 2;
const int64_t A_KIRIN = 3;
const int64_t A_LION = 4;
const int64_t A_NIWATORI = 5;
const int64_t B_HIYOKO = 6;
const int64_t B_ZOU = 7;
const int64_t B_KIRIN = 8;
const int64_t B_LION = 9;
const int64_t B_NIWATORI = 10;

const int64_t WIN = 0b11;
const int64_t LOSE = 0b10;
const int64_t UNKNOWN = 0b00;

inline int64_t get_A_hiyoko(const int64_t board) {
	return (board >> 48) & 0b11;
}

inline int64_t get_A_zou(const int64_t board) {
	return (board >> 50) & 0b11;
}

inline int64_t get_A_kirin(const int64_t board) {
	return (board >> 52) & 0b11;
}

inline int64_t get_B_hiyoko(const int64_t board) {
	return (board >> 54) & 0b11;
}

inline int64_t get_B_zou(const int64_t board) {
	return (board >> 56) & 0b11;
}

inline int64_t get_B_kirin(const int64_t board) {
	return (board >> 58) & 0b11;
}

inline int64_t A_hiyoko_inc(const int64_t board) {
	return board + (int64_t(1) << 48);
}

inline int64_t A_zou_inc(const int64_t board) {
	return board + (int64_t(1) << 50);
}

inline int64_t A_kirin_inc(const int64_t board) {
	return board + (int64_t(1) << 52);
}

inline int64_t A_hiyoko_dec(const int64_t board) {
	return board - (int64_t(1) << 48);
}

inline int64_t A_zou_dec(const int64_t board) {
	return board - (int64_t(1) << 50);
}

inline int64_t A_kirin_dec(const int64_t board) {
	return board - (int64_t(1) << 52);
}

inline int64_t get_board_cell(const int64_t board, const int x, const int y) {
	return (board >> (4 * (x + 3 * y))) & 0b1111;
}

inline int64_t get_board_celli(const int64_t board, const int i) {
	return (board >> (4 * i)) & 0b1111;
}

inline int64_t set_board_cell(const int64_t board, const int x, const int y, const int64_t piece) {
	int64_t ret = board & ~(int64_t(0b1111) << (4 * (x + 3 * y)));
	return ret | (piece << (4 * (x + 3 * y)));
}

inline int64_t set_board_celli(const int64_t board, const int i, const int64_t piece) {
	int64_t ret = board & ~(int64_t(0b1111) << (4 * i));
	return ret | (piece << (4 * i));
}

inline bool in_board(const int x, const int y) {
	return 0 <= x && x < 3 && 0 <= y && y < 4;
}

inline int64_t board_flip(const int64_t board) {
	int64_t ret = board;
	for (int i = 0; i < 12; i++) {
		int to_i = 11 - i;
		int64_t val = get_board_celli(board, i);
		if (val == EMPTY) {
			ret = set_board_celli(ret, to_i, EMPTY);
		}
		else if (val < B_HIYOKO) {
			ret = set_board_celli(ret, to_i, val + 5);
		}
		else {
			ret = set_board_celli(ret, to_i, val - 5);
		}
	}
	int64_t t = ((board >> 54) xor (board >> 48)) & int64_t(0b111111);
	t |= t << 6;
	t = t << 48;
	ret ^= t;
	return ret;
}

inline int64_t board_normalize(const int64_t board) {
	int64_t ret = board;
	for (int y = 0; y < 4; y++) {
		ret = set_board_cell(ret, 0, y, get_board_cell(board, 2, y));
		ret = set_board_cell(ret, 2, y, get_board_cell(board, 0, y));
	}
	if (ret >= board) {
		return board;
	}
	else {
		return ret;
	}
}

inline constexpr int64_t get_first_board() {
	const int64_t board_array[12] = {
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
	};
	int64_t ret = 0;
	for (int i = 0; i < 12; i++) {
		ret = set_board_celli(ret, i, board_array[i]);
	}
	return ret;
}

const int hiyoko_move[1][2] = {
	{0, -1},
};
const int zou_move[4][2] = {
	{1, 1},
	{1, -1},
	{-1, 1},
	{-1, -1},
};
const int kirin_move[4][2] = {
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1},
};
const int lion_move[8][2] = {
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1},
	{1, 1},
	{1, -1},
	{-1, 1},
	{-1, -1},
};
const int niwatori_move[6][2] = {
	{1, 0},
	{-1, 0},
	{0, 1},
	{0, -1},
	{1, -1},
	{-1, -1},
};

int64_t move(const int64_t board, const int from_i, const int nx, const int ny);

std::tuple<const int64_t, std::vector<int64_t>> get_next_boards(const int64_t board);

std::vector<int64_t> get_prev_boards(const int64_t board);

std::string board_to_string(int64_t board);

int64_t win_lose_check(const int64_t board);

class State {
public:
	std::unordered_set<int64_t> fixed;
	std::unordered_set<int64_t> unfixed;
	std::unordered_set<int64_t> prev_boards; // depth-{depth}
	std::unordered_set<int64_t> next_boards; // depth-{depth+1}
	int depth = 0;
};

bool write_board(const std::unordered_set<int64_t>& boards, const std::string& filename);

void read_board(const std::string& filename, int cnt, State& state);


