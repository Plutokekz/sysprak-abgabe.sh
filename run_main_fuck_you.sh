EXECNAME=sysprak-client
VALGRIND_LOG=log
ID=1v7x9j6u1935a
PLAYER1=1
CONFIG=tests/config.conf

if [[ $(
  make -s
  echo -n $?
) == "0" ]]; then
  echo "Compilation: OK"
else
  echo "Compilation did not complete without warnings or errors!"
  exit
fi

rm -f $VALGRIND_LOG
valgrind --log-file=$VALGRIND_LOG -q --leak-check=full --show-leak-kinds=all --trace-children=yes ./$EXECNAME -g $ID -p $PLAYER1 -c $CONFIG

if [[ $(cat $VALGRIND_LOG) == "" ]]; then
  echo "Valgrind: OK"
else
  echo "Valgrind did find errors:"
  cat $VALGRIND_LOG
fi

make clean -s
rm log
