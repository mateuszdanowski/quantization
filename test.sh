#!/bin/bash

if [ $# != 2 ]; then
  echo "Potrzebne są dwa argumenty: nazwa programu i folder z testami."
  exit
fi

PROG=$1
DIR=$2

VALGRINDFLAGS="--error-exitcode=15 --leak-check=full \
--show-leak-kinds=all --errors-for-leak-kinds=all \
--main-stacksize=999999999 -q"

CORRECT=0
WRONG=0
NUM_OF_TESTS=0

for f in $DIR/*.in; do

  OUTPUT="$(mktemp)"

  valgrind $VALGRINDFLAGS ./$PROG <$f 1>$OUTPUT.out 2>$OUTPUT.err
  EXIT_CODE=$?

  diff ${f%in}out $OUTPUT.out &>/dev/null
  OUT_DIFF=$?

  diff ${f%in}err $OUTPUT.err &>/dev/null
  ERR_DIFF=$?

  echo -n "OUT:"
  if [ $OUT_DIFF -eq 0 ]; then
    echo -n " OK"
  else
    echo -n " WA"
  fi

  echo -n "   ERR:"
  if [ $ERR_DIFF -eq 0 ]; then
    echo -n " OK"
  else
    echo -n " WA"
  fi

  echo -n "   Kod wyjścia: $EXIT_CODE"

  echo "   dla testu ${f#$DIR/}"

  if [ $OUT_DIFF -eq 0 -a $ERR_DIFF -eq 0 -a $EXIT_CODE -eq 0 ]; then
    ((CORRECT += 1))
  else
    ((WRONG += 1))
  fi

  ((NUM_OF_TESTS += 1))

  rm -f $OUTPUT.out $OUTPUT.err

done

echo "Zaliczone testy: $CORRECT/$NUM_OF_TESTS"
echo "Niezaliczone testy: $WRONG/$NUM_OF_TESTS"
