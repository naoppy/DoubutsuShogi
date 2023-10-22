#include "../DoubutsuSyogi/Board.h"
#include <iostream>
#include <unordered_set>
#include <deque>
#include <fstream>
#include <format>


void enumerate_next_boards(State& s) {
	// 今、s.depthの盤面がs.prev_boardsに入っている。
	// s.prev.boardsは直前に勝敗が確定した盤面。
	// それらの盤面から、次に確定する盤面s.next_boardsを作る。
	// s.next_boardsはs.depth+1の盤面の集合。
	// s.next_boardsはs.unfixedに入っている盤面から選ぶ
	// depthが偶数だと、負け確定の盤面
	// depthが奇数だと、勝ち確定の盤面
	if (s.prev_boards.size() * 4 < s.unfixed.size()) {
	//if (false) {
		// 前回更新された盤面を使って次に確定する盤面を作る
		if (s.depth % 2 == 0) { // 直前で新たな負けが確定した
			for (int64_t prev_kakutei : s.prev_boards) {
				// 直前で新たに確定した負け(prev)にいければ新たに勝ち確定
				std::vector<int64_t> reach_lose = get_prev_boards(prev_kakutei);
				for (int64_t cand : reach_lose) {
					if (s.unfixed.find(cand) != s.unfixed.end()) {
						s.next_boards.insert(cand);
					}
				}
			}
		}
		else { // 直前で新たに勝ちが確定した
			for (int64_t prev_kakutei : s.prev_boards) {
				// 直前で新たに確定した勝ち(prev)にいけるかつ、その全ての行き先が勝ち(fixed)なら新たに負け確定
				std::vector<int64_t> reach_win = get_prev_boards(prev_kakutei);
				for (int64_t cand : reach_win) {
					if (s.unfixed.find(cand) != s.unfixed.end()) {
						// 全ての行き先が勝ち(fixed)か
						auto [_, cand_nexts] = get_next_boards(cand);
						for (int64_t cand_next : cand_nexts) {
							cand_next = board_normalize(board_flip(cand_next));
							if (s.fixed.find(cand_next) == s.fixed.end()) {
								// ひとつでも不明ならダメ
								goto no_next2;
							}
						}
						s.next_boards.insert(cand);
					no_next2:
						;
					}
				}
			}
		}
	}
	else {
		// s.unfixedから次に確定する盤面を作る
		for (int64_t board : s.unfixed) {
			auto tuple = get_next_boards(board);
			const int64_t result = std::get<0>(tuple);
			const std::vector<int64_t> next_boards = std::get<1>(tuple);
			if (result != UNKNOWN)
				continue;

			if (s.depth % 2 == 0) { // 直前で新たな負けが確定した
				// 直前で新たに確定した負け(s.prev_boards)にいければ新たに勝ち確定
				for (int64_t next_board : next_boards) {
					next_board = board_normalize(board_flip(next_board));
					if (s.prev_boards.find(next_board) != s.prev_boards.end()) {
						s.next_boards.insert(board);
						break;
					}
				}
			}
			else { // 直前で新たな勝ちが確定した
				// 直前で新たに確定した勝ち(s.prev_boards)を含みつつ、全て勝ち盤面なら新たに負け確定
				// 実は、全ての行き先がfixedなら、その盤面の行き先は全て勝ち盤面で、負けが確定する
				// これはアルゴリズムを追えばわかる
				for (int64_t next_board : next_boards) {
					next_board = board_normalize(board_flip(next_board));
					if (s.fixed.find(next_board) == s.fixed.end()) {
						goto no_next;
					}
				}
				s.next_boards.insert(board);
			no_next:
				;
			}
		}
	}
}

int main() {
	int first_board_depth = -1;
	size_t lose_win_cnt[2] = { 0, 0 };
	int64_t first_board = board_normalize(get_first_board());

	State s;
	read_board("../DoubutsuSyogi/result_246803167.txt", 246803167, s);

	// まだ更新が続くなら更新し続ける
	while (s.prev_boards.size() > 0) {
		lose_win_cnt[s.depth % 2] += s.prev_boards.size();

		std::cout << "depth: " << s.depth << " " <<
			"prev_boards: " << s.prev_boards.size() << " " <<
			"unfixed boards: " << s.unfixed.size() << std::endl;

		enumerate_next_boards(s);

		for (int64_t board : s.next_boards) {
			s.fixed.insert(board);
			s.unfixed.erase(board);
			if (board == first_board) {
				first_board_depth = s.depth + 1;
			}
		}

		s.prev_boards = s.next_boards;
		s.next_boards.clear();
		// C++ではできない: s.unfixed.shrink_to_fit();
		// 代わりに、以下を使う
		if (s.unfixed.load_factor() < 0.8) {
			s.unfixed.rehash(s.unfixed.size());
		}
		s.depth++;
	}

	std::cout << "win board:   " << lose_win_cnt[1] << std::endl;
	std::cout << "lose board:  " << lose_win_cnt[0] << std::endl;
	std::cout << "draw board:  " << s.unfixed.size() << std::endl;
	std::cout << "first board depth: " << first_board_depth << std::endl;
	std::cout << "max depth:   " << s.depth - 1 << std::endl;
	return 0;
}