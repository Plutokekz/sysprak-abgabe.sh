EXECNAME=sysprak-client-tests
VALGRIND_LOG=log

#
# check if the project compiles without errors or warnings
#
if [[ $(make -s; echo -n $?) = "0" ]]; then
	echo "Compilation: OK"
else
	echo "Compilation did not complete without warnings or errors!"
	exit;
fi

#
# check Valgrind for PLAYER2
#
rm -f $VALGRIND_LOG
valgrind --log-file=$VALGRIND_LOG -q --leak-check=full --trace-children=yes ./$EXECNAME

#
# check valgrind errors
#
if [[ $(cat $VALGRIND_LOG) == "" ]]; then
    echo "Valgrind: OK"
else
    echo "Valgrind did find errors:"
    cat $VALGRIND_LOG
fi