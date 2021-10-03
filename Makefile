.EXPORT_ALL_VARIABLES:

EFLAGS = -Wall -Wextra -Werror -g
G=HOLZ
C=tests/config.conf
P=1
url=http://sysprak.priv.lab.nm.ifi.lmu.de/api/v1/matches
headers="Content-Type: application/json"
json_data='{"type":"Bashni","errorMsg":"","gameGeneric":{"name":"","timeout":3000},"gameSpecific":{},"players":[{"name":"","type":"COMPUTER","gameSpecific":{}},{"name":"","type":"COMPUTER","gameSpecific":{}}]}'
export C
export G



sysprak-client: main.c log.o performConnection.o utils.o shareMemory.o thinker.o gamePhase.o cmdPipe.o old_bitboard.o protocolUtils.o mySignal.o towerBoard.o
	gcc $(EFLAGS) -o sysprak-client main.c perform_connection.o utils.o share_memory.o thinker.o game_phase.o cmd_pipe.o old_bitboard.o protocol_utils.o my_signal.o tower_board.o log.o

sysprak-multi-client: main_vs.c log.o performConnection.o utils.o shareMemory.o thinker.o gamePhase.o cmdPipe.o old_bitboard.o protocolUtils.o mySignal.o towerBoard.o
	gcc $(EFLAGS) -o sysprak-multi-client main_vs.c perform_connection.o utils.o share_memory.o thinker.o game_phase.o cmd_pipe.o old_bitboard.o protocol_utils.o my_signal.o tower_board.o log.o

towerBoard.o: modules/tower_board.c modules/tower_board.h
	gcc $(EFLAGS) -c modules/tower_board.c

performConnection.o: perform_connection.c perform_connection.h
	gcc $(EFLAGS) -c perform_connection.c

old_bitboard.o: modules/old_bitboard.c modules/old_bitboard.h
	gcc $(EFLAGS) -c modules/old_bitboard.c

shareMemory.o: modules/share_memory.c modules/share_memory.h
	gcc $(EFLAGS) -c modules/share_memory.c

cmdPipe.o: modules/cmd_pipe.c modules/cmd_pipe.h
	gcc $(EFLAGS) -c modules/cmd_pipe.c

utils.o:  modules/utils.c modules/utils.h
	gcc $(EFLAGS) -c modules/utils.c

gamePhase.o: game_phase.c game_phase.h
	gcc $(EFLAGS) -c game_phase.c

thinker.o: thinker.c thinker.h
	gcc $(EFLAGS) -c thinker.c

protocolUtils.o: modules/protocol_utils.c modules/protocol_utils.h
	gcc $(EFLAGS) -c modules/protocol_utils.c

mySignal.o: modules/my_signal.c modules/my_signal.h
	gcc $(EFLAGS) -c modules/my_signal.c

log.o: modules/log.h modules/log.c
	gcc $(EFLAGS) -DLOG_USE_COLOR -c modules/log.c

game_id:
	export G="$(shell curl -s -X POST -H $(headers) -d $(json_data) $(url) |(python3 -c "import sys, json; print(json.load(sys.stdin)['id'])"))"

test_connection:
	ping -c 4 sysprak.priv.lab.nm.ifi.lmu.de

play: sysprak-client
	./sysprak-client -g $(shell curl -s -X POST -H $(headers) -d $(json_data) $(url) |(python3 -c "import sys, json; print(json.load(sys.stdin)['id'])"))

play_vs: sysprak-multi-client
	./sysprak-multi-client -g $(shell curl -s -X POST -H $(headers) -d $(json_data) $(url) |(python3 -c "import sys, json; print(json.load(sys.stdin)['id'])"))

vg:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./sysprak-client -g $(shell curl -s -X POST -H $(headers) -d $(json_data) $(url) |(python3 -c "import sys, json; print(json.load(sys.stdin)['id'])")) -c $(C)

clean:
	rm -f *.o
