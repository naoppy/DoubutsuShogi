#include "Board.h"
#include <iostream>
#include <unordered_set>
#include <deque>
#include <format>

int main()
{
	int64_t first_board = board_normalize(get_first_board());
	std::cout << first_board << std::endl;

	std::unordered_set<int64_t> visited;
	visited.insert(first_board);
	std::deque<int64_t> queue;
	queue.push_back(first_board);

	int count = 0;

	int lose_count = 0;
	int win_count = 0;
	int unknown_count = 0;
	size_t max_degree = 0;
	std::vector<int64_t> max_degree_boards;

	while (!queue.empty()) {
		count++;
		if (count % 1000000 == 0) {
			std::cout << "visited: " << visited.size() << std::endl;
			std::cout << "queue size: " << queue.size() << std::endl;
		}
		if (count % 100000)
			queue.shrink_to_fit();

		int64_t board = queue.front();
		queue.pop_front();

		auto tuple = get_next_boards(board);
		const int64_t result = std::get<0>(tuple);
		const std::vector<int64_t> next_boards = std::get<1>(tuple);


		// 勝ち確定の場面では見逃さない
		if (result == WIN) {
			win_count++;
			continue;
		}
		// 自分がどう動いてもトライで負ける場合は負け確定
		else if (result == LOSE) {
			lose_count++;
			continue;
		}
		// next_boardsにキャッチで勝ちがなく、動いてもトライで負けない場合
		unknown_count++;
		size_t degree = next_boards.size();
		if (degree > max_degree) {
			max_degree = degree;
			max_degree_boards.clear();
			max_degree_boards.push_back(board);
		}
		else if (degree == max_degree) {
			max_degree_boards.push_back(board);
		}
		for (int64_t next_board : next_boards) {
			next_board = board_normalize(board_flip(next_board));
			if (!visited.contains(next_board)) {
				visited.insert(next_board);
				queue.push_back(next_board);
			}
		}
	}


	std::cout << "DONE\n";
	std::cout << "visited size: " << visited.size() << std::endl;
	std::cout << "win count: " << win_count << std::endl;
	std::cout << "lose count: " << lose_count << std::endl;
	std::cout << "unknown count: " << unknown_count << std::endl;
	std::cout << "max degree: " << max_degree << std::endl;
	std::cout << "max degree boards: " << std::endl;
	for (int64_t e : max_degree_boards) {
		std::cout << e << ", ";
	}

	/*
	6分30秒程度で計算が終わる。ファイルへの書き出しは1分程度。
	visited size: 246803167
	win count: 140298614
	lose count: 7018985
	unknown count: 99485568
	max degree: 38
	*/
	write_board(visited, std::format("result_{}.txt", visited.size()));
	return 0;
}
