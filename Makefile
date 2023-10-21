SRC_DIR := ./doubutsusyogi
TEST_DIR := ./tests

fmt:
	# isort
	ruff check ${SRC_DIR} ${TEST_DIR} --select I --fix-only --exit-zero
	# black
	black ${SRC_DIR} ${TEST_DIR}
	# ruff
	ruff check ${SRC_DIR} ${TEST_DIR} --fix --exit-zero
lint:
	ruff check ${SRC_DIR} ${TEST_DIR}
	mypy ${SRC_DIR} ${TEST_DIR}
test:
	pytest ${TEST_DIR}