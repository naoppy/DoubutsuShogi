#include "Board.h"
#include <iostream>
#include <fstream>

int64_t move(const int64_t board, const int from_i, const int nx, const int ny) {
	int64_t from_piece = get_board_celli(board, from_i);
	int64_t to_piece = get_board_cell(board, nx, ny);
	int64_t ret = set_board_celli(board, from_i, EMPTY);
	ret = set_board_cell(ret, nx, ny, from_piece);
	// にわとりになる
	if (from_piece == A_HIYOKO && ny == 0) {
		ret = set_board_cell(ret, nx, ny, A_NIWATORI);
	}
	if (to_piece == EMPTY) {
		return ret;
	}
	else if (to_piece == B_HIYOKO || to_piece == B_NIWATORI) {
		return A_hiyoko_inc(ret);
	}
	else if (to_piece == B_ZOU) {
		return A_zou_inc(ret);
	}
	else if (to_piece == B_KIRIN) {
		return A_kirin_inc(ret);
	}
	else if (to_piece == B_LION) {
		// キャッチで勝ち
		return ret;
	}
	else {
		// 自分の駒のある場所には移動できない
		// NO_REACHABLE
		std::cout << "NO_REACHABLE\n" << std::endl;
		return get_first_board();
	}
}

std::tuple<const int64_t, std::vector<int64_t>> get_next_boards(const int64_t board) {
	std::vector<int64_t> ret;
	bool win = false;
	for (int i = 0; i < 12; i++) {
		int64_t piece = get_board_celli(board, i);
		if (piece == EMPTY) {
			// 駒を置く
			if (get_A_hiyoko(board) > 0) {
				ret.push_back(A_hiyoko_dec(set_board_celli(board, i, A_HIYOKO)));
			}
			if (get_A_zou(board) > 0) {
				ret.push_back(A_zou_dec(set_board_celli(board, i, A_ZOU)));
			}
			if (get_A_kirin(board) > 0) {
				ret.push_back(A_kirin_dec(set_board_celli(board, i, A_KIRIN)));
			}
		}
		else if (piece == A_HIYOKO) {
			int x = i % 3, y = i / 3;
			for (int j = 0; j < 1; j++) {
				int nx = x + hiyoko_move[j][0], ny = y + hiyoko_move[j][1];
				if (in_board(nx, ny)) {
					int64_t to_piece = get_board_cell(board, nx, ny);
					if (to_piece == EMPTY or to_piece >= B_HIYOKO)
						ret.push_back(move(board, i, nx, ny));
					if (to_piece == B_LION)
						win = true;
				}
			}
		}
		else if (piece == A_ZOU) {
			int x = i % 3, y = i / 3;
			for (int j = 0; j < 4; j++) {
				int nx = x + zou_move[j][0], ny = y + zou_move[j][1];
				if (in_board(nx, ny)) {
					int64_t to_piece = get_board_cell(board, nx, ny);
					if (to_piece == EMPTY or to_piece >= B_HIYOKO)
						ret.push_back(move(board, i, nx, ny));
					if (to_piece == B_LION)
						win = true;
				}
			}
		}
		else if (piece == A_KIRIN) {
			int x = i % 3, y = i / 3;
			for (int j = 0; j < 4; j++) {
				int nx = x + kirin_move[j][0], ny = y + kirin_move[j][1];
				if (in_board(nx, ny)) {
					int64_t to_piece = get_board_cell(board, nx, ny);
					if (to_piece == EMPTY or to_piece >= B_HIYOKO)
						ret.push_back(move(board, i, nx, ny));
					if (to_piece == B_LION)
						win = true;
				}
			}
		}
		else if (piece == A_LION) {
			int x = i % 3, y = i / 3;
			for (int j = 0; j < 8; j++) {
				int nx = x + lion_move[j][0], ny = y + lion_move[j][1];
				if (in_board(nx, ny)) {
					int64_t to_piece = get_board_cell(board, nx, ny);
					if (to_piece == EMPTY or to_piece >= B_HIYOKO)
						ret.push_back(move(board, i, nx, ny));
					if (to_piece == B_LION)
						win = true;
				}
			}
		}
		else if (piece == A_NIWATORI) {
			int x = i % 3, y = i / 3;
			for (int j = 0; j < 6; j++) {
				int nx = x + niwatori_move[j][0], ny = y + niwatori_move[j][1];
				if (in_board(nx, ny)) {
					int64_t to_piece = get_board_cell(board, nx, ny);
					if (to_piece == EMPTY or to_piece >= B_HIYOKO)
						ret.push_back(move(board, i, nx, ny));
					if (to_piece == B_LION)
						win = true;
				}
			}
		}
	}
	if (win) {
		return std::make_tuple(WIN, std::vector<int64_t>());
	}
	for (int x = 0; x < 3; x++) {
		int64_t piece = get_board_cell(board, x, 3);
		if (piece == B_LION) {
			return std::make_tuple(LOSE, std::vector<int64_t>());
		}
	}
	return std::make_tuple(UNKNOWN, ret);
}

// pieceがfromからtoへ移動したと想定し、移動後のboardが与えられたとき、移動前のboardをretに追加する
void move_back(std::vector<int64_t>& ret, const int64_t board, int to_x, int to_y, int from_x, int from_y, int64_t piece) {
	// check move is valid
	const int(*move_vecs)[2];
	int move_vecs_size = 0;
	if (piece == A_HIYOKO) {
		move_vecs = hiyoko_move;
		move_vecs_size = 1;
	}
	else if (piece == A_ZOU) {
		move_vecs = zou_move;
		move_vecs_size = 4;
	}
	else if (piece == A_KIRIN) {
		move_vecs = kirin_move;
		move_vecs_size = 4;
	}
	else if (piece == A_LION) {
		move_vecs = lion_move;
		move_vecs_size = 8;
	}
	else if (piece == A_NIWATORI) {
		move_vecs = niwatori_move;
		move_vecs_size = 6;
	}
	else {
		return;
	}
	bool ok = false;
	for (int i = 0; i < move_vecs_size; i++) {
		if (move_vecs[i][0] == to_x - from_x and move_vecs[i][1] == to_y - from_y) {
			ok = true;
			break;
		}
	}
	if (!ok) {
		return;
	}
	const int64_t modoru = set_board_cell(board, from_x, from_y, piece);
	ret.push_back(modoru);
	// 相手の駒を取って移動した
	if (get_A_hiyoko(modoru) > 0) {
		ret.push_back(A_hiyoko_dec(set_board_cell(modoru, to_x, to_y, B_HIYOKO)));
		ret.push_back(A_hiyoko_dec(set_board_cell(modoru, to_x, to_y, B_NIWATORI)));
	}
	if (get_A_zou(modoru) > 0) {
		ret.push_back(A_zou_dec(set_board_cell(modoru, to_x, to_y, B_ZOU)));
	}
	if (get_A_kirin(modoru) > 0) {
		ret.push_back(A_kirin_dec(set_board_cell(modoru, to_x, to_y, B_KIRIN)));
	}
	return;
}

// 呼び出し先でSet化すること
std::vector<int64_t> get_prev_boards(const int64_t board) {
	std::vector<int64_t> ret;
	int64_t before = board_flip(board);
	// 一手前を考えると、駒を回収するor隣接する空白の場所に動かす
	for (int i = 0; i < 12; i++) {
		const int64_t piece = get_board_celli(before, i);
		if (piece == EMPTY or piece >= B_HIYOKO)
			continue;
		const int64_t kaisyu = set_board_celli(before, i, EMPTY);
		int x = i % 3, y = i / 3;
		// 直接置いたパターン
		if (piece != A_LION and piece != A_NIWATORI) {
			if (piece == A_HIYOKO) {
				ret.push_back(A_hiyoko_inc(kaisyu));
			}
			else if (piece == A_ZOU) {
				ret.push_back(A_zou_inc(kaisyu));
			}
			else if (piece == A_KIRIN) {
				ret.push_back(A_kirin_inc(kaisyu));
			}
		}
		// 隣接する空白の場所に動かして戻す
		// 相手の駒を取って移動した可能性と、空白の場所に移動した可能性あり
		for (int j = 0; j < 8; j++) {
			int nx = x + lion_move[j][0], ny = y + lion_move[j][1];
			if (in_board(nx, ny) && get_board_cell(kaisyu, nx, ny) == EMPTY) {
				move_back(ret, kaisyu, x, y, nx, ny, piece);
			}
		}
		// にわとりに関しては、にわとりの位置が最奥の場合、ひよこに戻るパターンがあることに注意
		if (piece == A_NIWATORI && y == 0 && get_board_cell(before, x, 1) == EMPTY) {
			move_back(ret, kaisyu, x, 0, x, 1, A_HIYOKO);
		}
	}
	for (int i = 0; i < ret.size(); i++) {
		ret[i] = board_normalize(ret[i]);
	}
	return ret;
}

std::string board_to_string(int64_t board) {
	std::ostringstream oss;
	oss << "-------" << std::endl;
	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 3; x++) {
			int64_t piece = get_board_cell(board, x, y);
			oss << "|";
			if (piece == EMPTY) {
				oss << " ";
			}
			else if (piece == A_HIYOKO) {
				oss << "H";
			}
			else if (piece == A_ZOU) {
				oss << "Z";
			}
			else if (piece == A_KIRIN) {
				oss << "K";
			}
			else if (piece == A_LION) {
				oss << "L";
			}
			else if (piece == A_NIWATORI) {
				oss << "N";
			}
			else if (piece == B_HIYOKO) {
				oss << "h";
			}
			else if (piece == B_ZOU) {
				oss << "z";
			}
			else if (piece == B_KIRIN) {
				oss << "k";
			}
			else if (piece == B_LION) {
				oss << "l";
			}
			else if (piece == B_NIWATORI) {
				oss << "n";
			}
		}
		oss << "|" << std::endl << "-------" << std::endl;
	}
	oss << std::endl;
	return oss.str();
}



int64_t win_lose_check(int64_t board) {
	for (int i = 0; i < 12; i++) {
		int64_t piece = get_board_celli(board, i);
		// 敵のライオンをキャッチできる場所に自分の駒がある
		if (piece == B_LION) {
			int x = i % 3, y = i / 3;
			for (int j = 0; j < 8; j++) {
				int nx = x - lion_move[j][0], ny = y - lion_move[j][1]; // マイナスに注意
				if (in_board(nx, ny)) {
					int64_t to_piece = get_board_cell(board, nx, ny);
					const int (*move_vecs)[2];
					int move_vecs_size = 0;
					if (to_piece == A_HIYOKO) {
						move_vecs = hiyoko_move;
						move_vecs_size = 1;
					}
					else if (to_piece == A_ZOU) {
						move_vecs = zou_move;
						move_vecs_size = 4;
					}
					else if (to_piece == A_KIRIN) {
						move_vecs = kirin_move;
						move_vecs_size = 4;
					}
					else if (to_piece == A_LION) {
						move_vecs = lion_move;
						move_vecs_size = 8;
					}
					else if (to_piece == A_NIWATORI) {
						move_vecs = niwatori_move;
						move_vecs_size = 6;
					}
					else {
						move_vecs = nullptr;
						move_vecs_size = 0;
					}
					for (int k = 0; k < move_vecs_size; k++) {
						if (move_vecs[k][0] == lion_move[j][0] and move_vecs[k][1] == lion_move[j][1]) {
							return WIN;
						}
					}
				}
			}
		}
	}
	for (int x = 0; x < 3; x++) {
		int64_t piece = get_board_cell(board, x, 3);
		if (piece == B_LION) {
			return LOSE;
		}
	}
	return UNKNOWN;
}

bool write_board(const std::unordered_set<int64_t>& boards, const std::string& filename) {
	std::ofstream file(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (file.is_open()) {
		// do something
		for (int64_t board : boards) {
			file.write((char*)&board, sizeof(int64_t));
		}
		file.close();
		return true;
	}
	else {
		std::cout << "Unable to open file" << std::endl;
		return false;
	}
}

void read_board(const std::string& filename, int cnt, State& state) {
	std::cout << "Load Boards..." << std::endl;
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);
	char buf[8] = {};
	for (int i = 0; i < cnt; i++) {
		ifs.read(buf, 8);
		int64_t board = *(int64_t*)buf;
		int64_t result = win_lose_check(board);
		if (result == LOSE) {
			state.fixed.insert(board);
			state.prev_boards.insert(board);
		}
		else {
			state.unfixed.insert(board);
			if (result == WIN) {
				state.next_boards.insert(board);
			}
		}
	}
	std::cout << "DONE! Loaded boards: " << cnt << std::endl;
}