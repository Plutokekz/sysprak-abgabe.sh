EXECNAME=sysprak-client
VALGRIND_LOG=log
ID=0000000000000
PLAYER1=1
CONFIG=tests/config.conf

if [[ $(make -s; echo -n $?) = "0" ]]; then
	echo "Compilation: OK"
else
	echo "Compilation did not complete without warnings or errors!"
	exit;
fi

rm -f $VALGRIND_LOG
valgrind --log-file=$VALGRIND_LOG -q --leak-check=full --trace-children=yes ./$EXECNAME -g $ID -p $PLAYER1 -c $CONFIG

if [[ $(cat $VALGRIND_LOG) == "" ]]; then
	echo "Valgrind: OK"
else
	echo "Valgrind did find errors:"
	cat $VALGRIND_LOG
fi
