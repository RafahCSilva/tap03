#!/bin/bash

TESTDIR=/tmp/test
mkdir -p $TESTDIR

START=$(date +%s)
TOTAL=10026
GRADE=0
for i in {1..100}; do
    echo -n "."
    OUTPUT=$(gtimeout 2s ./a.out < "./in/file$i" | head -n 1)
    EXPECT=$(cat "./out/file$i" | head -n 1)
    CURRENT=$(date +%s)
    if (( CURRENT - START > 60 )); then
	    echo "Time limit exceeded (60s)."
	    break
    fi
    if [[ "$EXPECT" =~ "CORRETO" ]]; then
        if [[ "$OUTPUT" =~ "CORRETO" ]]; then
            GRADE=$((GRADE+80))
        fi
        if [[ "$OUTPUT" == "$EXPECT" ]]; then
            GRADE=$((GRADE+48))
        fi
    else
        if [[ "$OUTPUT" == "ERRO" ]]; then
            GRADE=$((GRADE+80))
        fi
    fi
    if [[ "$OUTPUT" == "$EXPECT" ]]; then
        echo " OK $i : r $OUTPUT | g $EXPECT"
    else
        echo " Fail $i : r $OUTPUT | g $EXPECT         <<------ $i"
    fi
done

NTOTAL=$(echo "scale=2; 10*$GRADE/$TOTAL" | bc)
echo ""
echo "Comment :=>> Nota do projeto: $NTOTAL de 10.00"
echo "Grade :=>> $NTOTAL"
