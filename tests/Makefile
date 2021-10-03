CFLAGS=-Wall -Wextra -Werror -g

test:
	make test-queue
	make test-bitboard
	make test-pipes
	make test-config
	make test-shm

test-queue: test_queue.c queue.o
	gcc $(CFLAGS) -o queue test_queue.c queue.o
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./queue

test-bitboard: test_bitboard.c bitboard.o log.o
	gcc $(CFLAGS) -o bitboard test_bitboard.c bitboard.o log.o
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./bitboard

test-pipes: test_cmd_pipe.c cmd_pipe.o
	gcc $(CFLAGS) -o pipes test_cmd_pipe.c cmd_pipe.o
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./pipes

tests-config: test_config.c config.o
	gcc -g -o config test_config.c config.o
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./config

tests-shm: test_shm.c share_memory.o
	gcc -g -o shm test_shm.c share_memory.o
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./shm


bitboard.o: ../modules/bitboard.h ../modules/bitboard.c
	gcc $(CFLAGS) -c ../modules/bitboard.c

log.o: ../modules/log.h ../modules/log.c
	gcc $(CFLAGS) -c ../modules/log.c

queue.o: ../modules/queue.h ../modules/queue.c
	gcc $(CFLAGS) -c ../modules/queue.c

cmd_pipe.o: ../module/cmd_pipe.c ../module/cmd_pipe.h
	gcc $(CFLAGS) -c ../cmd_pipe.c

config.o: ../module/config.c ../module/config.h
	gcc $(CFLAGS) -c ../config.c

share_memory.o: ../module/share_memory.c ../module/share_memory.h
	gcc $(CFLAGS) -c ../share_memory.c

clean:
	rm *.o queue bitboard pipes config shm
