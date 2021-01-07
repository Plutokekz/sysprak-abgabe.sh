EXECNAMECONFIG=sysprak-client-tests-config
EXECNAMEPIPE=sysprak-client-tests-pipe
EXECNAMESHM=sysprak-client-tests-shm
VALGRIND_LOG=log

#
# check if the project compiles without errors or warnings
#
echo "Config"
if [[ $(make RUN=0 -s; echo -n $?) = "0" ]]; then
	echo "Compilation: OK"
else
	echo "Compilation did not complete without warnings or errors!"
	exit;
fi
#
# check Valgrind for PLAYER2
#
rm -f $VALGRIND_LOG
valgrind --log-file=$VALGRIND_LOG -q --leak-check=full --show-leak-kinds=all --trace-children=yes ./$EXECNAMECONFIG

#
# check valgrind errors
#
if [[ $(cat $VALGRIND_LOG) == "" ]]; then
    echo "Valgrind: OK"
else
    echo "Valgrind did find errors:"
    cat $VALGRIND_LOG
fi

echo "CmdPipe"
if [[ $(make RUN=1 -s ; echo -n $?) = "0" ]]; then
	echo "Compilation: OK"
else
	echo "Compilation did not complete without warnings or errors!"
	exit;
fi

#
# check Valgrind for PLAYER2
#
rm -f $VALGRIND_LOG
valgrind --log-file=$VALGRIND_LOG -q --leak-check=full --show-leak-kinds=all --trace-children=yes ./$EXECNAMEPIPE

#
# check valgrind errors
#
if [[ $(cat $VALGRIND_LOG) == "" ]]; then
    echo "Valgrind: OK"
else
    echo "Valgrind did find errors:"
    cat $VALGRIND_LOG
fi

echo "SHM"
if [[ $(make RUN=2 -s ; echo -n $?) = "0" ]]; then
	echo "Compilation: OK"
else
	echo "Compilation did not complete without warnings or errors!"
	exit;
fi

#
# check Valgrind for PLAYER2
#
rm -f $VALGRIND_LOG
valgrind --log-file=$VALGRIND_LOG -q --leak-check=full --show-leak-kinds=all --trace-children=yes ./$EXECNAMESHM

#
# check valgrind errors
#
if [[ $(cat $VALGRIND_LOG) == "" ]]; then
    echo "Valgrind: OK"
else
    echo "Valgrind did find errors:"
    cat $VALGRIND_LOG
fi

# shellcheck disable=SC2091
make clean -s

rm log
# rm sysprak-client-tests
rm sysprak-client-tests-config
rm sysprak-client-tests-pipe
rm sysprak-client-tests-shm