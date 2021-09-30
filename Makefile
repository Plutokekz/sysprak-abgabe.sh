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



sysprak-client: Main.c log.o performConnection.o utils.o shareMemory.o thinker.o gamePhase.o cmdPipe.o bitboard.o protocolUtils.o mySignal.o towerBoard.o
	gcc $(EFLAGS) -o sysprak-client Main.c performConnection.o utils.o shareMemory.o thinker.o gamePhase.o cmdPipe.o bitboard.o protocolUtils.o mySignal.o towerBoard.o log.o

sysprak-multi-client: main_vs.c log.o performConnection.o utils.o shareMemory.o thinker.o gamePhase.o cmdPipe.o bitboard.o protocolUtils.o mySignal.o towerBoard.o
	gcc $(EFLAGS) -o sysprak-multi-client main_vs.c performConnection.o utils.o shareMemory.o thinker.o gamePhase.o cmdPipe.o bitboard.o protocolUtils.o mySignal.o towerBoard.o log.o

towerBoard.o: modules/towerBoard.c modules/towerBoard.h
	gcc $(EFLAGS) -c modules/towerBoard.c

performConnection.o: performConnection.c performConnection.h
	gcc $(EFLAGS) -c performConnection.c

bitboard.o: modules/bitboard.c modules/bitboard.h
	gcc $(EFLAGS) -c modules/bitboard.c

shareMemory.o: modules/shareMemory.c modules/shareMemory.h
	gcc $(EFLAGS) -c modules/shareMemory.c

cmdPipe.o: modules/cmdPipe.c modules/cmdPipe.h
	gcc $(EFLAGS) -c modules/cmdPipe.c

utils.o:  modules/utils.c modules/utils.h
	gcc $(EFLAGS) -c modules/utils.c

gamePhase.o: gamePhase.c gamePhase.h
	gcc $(EFLAGS) -c gamePhase.c

thinker.o: thinker.c thinker.h
	gcc $(EFLAGS) -c thinker.c

protocolUtils.o: modules/protocolUtils.c modules/protocolUtils.h
	gcc $(EFLAGS) -c modules/protocolUtils.c

mySignal.o: modules/mySignal.c modules/mySignal.h
	gcc $(EFLAGS) -c modules/mySignal.c

log.o: modules/log.h modules/log.c
	gcc $(EFLAGS) -DLOG_USE_COLOR -c modules/log.c

game_id:
	export G="$(shell curl -s -X POST -H $(headers) -d $(json_data) $(url) |(python3 -c "import sys, json; print(json.load(sys.stdin)['id'])"))"

play: sysprak-client
	./sysprak-client -g $(shell curl -s -X POST -H $(headers) -d $(json_data) $(url) |(python3 -c "import sys, json; print(json.load(sys.stdin)['id'])"))

play_vs: sysprak-multi-client
	./sysprak-multi-client -g $(shell curl -s -X POST -H $(headers) -d $(json_data) $(url) |(python3 -c "import sys, json; print(json.load(sys.stdin)['id'])"))

vg:
	valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes ./sysprak-client -g $(shell curl -s -X POST -H $(headers) -d $(json_data) $(url) |(python3 -c "import sys, json; print(json.load(sys.stdin)['id'])")) -c $(C)

clean:
	rm -f *.o
