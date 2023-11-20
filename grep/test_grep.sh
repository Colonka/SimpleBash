#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE_1="test.txt"
TEST_FILE_2="2_test.txt"
PATTERN="ABC"
PATTERN_2="Qty"
PATTERN_FILE_1="pattern.txt"
PATTERN_FILE_2="2_pattern.txt"
echo "" > log_grep.txt

for var in "$PATTERN $TEST_FILE_1" "$PATTERN $TEST_FILE_1 $TEST_FILE_2"
do
echo "$var"  
./s21_grep $var > s21_grep.txt
grep $var > grep.txt
DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
  if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
    then 
      (( COUNTER_SUCCESS++ ))
    else
      echo "$var" >> log_grep.txt
      (( COUNTER_FAIL++ ))
  fi
rm s21_grep.txt grep.txt
done

for var in -e -i -v -c -l -n -h -f -o
do
    if [ "$var" == "-f" ]
        then 
            for var2 in "$var $PATTERN_FILE_1 $TEST_FILE_1" "$var$PATTERN_FILE_1 $var $PATTERN_FILE_2 $TEST_FILE_1 $TEST_FILE_2"
            do
            echo "$var2"  
            ./s21_grep $var2 > s21_grep.txt
            grep $var2 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
                if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
                    then 
                (( COUNTER_SUCCESS++ ))
                    else
                echo "$var2" >> log_grep.txt
                (( COUNTER_FAIL++ ))
                fi
            rm s21_grep.txt grep.txt
            done
    elif [ "$var" == "-e" ]
        then
            for var3 in "$var $PATTERN $TEST_FILE_1" "$var$PATTERN $var $PATTERN_2 $TEST_FILE_1 $TEST_FILE_2"
        do
        echo "$var3"  
            ./s21_grep $var3 > s21_grep.txt
            grep $var3 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
                if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
                    then 
                (( COUNTER_SUCCESS++ ))
                    else
                echo "$var3" >> log_grep.txt
                (( COUNTER_FAIL++ ))
                fi
            rm s21_grep.txt grep.txt
        done
        else
            for var4 in "$var $PATTERN $TEST_FILE_1" "$var $PATTERN $TEST_FILE_1 $TEST_FILE_2"
        do
        echo "$var4"  
            ./s21_grep $var4 > s21_grep.txt
            grep $var4 > grep.txt
            DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
                if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
                    then 
                (( COUNTER_SUCCESS++ ))
                    else
                echo "$var4" >> log_grep.txt
                (( COUNTER_FAIL++ ))
                fi
            rm s21_grep.txt grep.txt
        done
    fi
done
for var in i v c l n h
do
    for var2 in i v c l n h o e f
    do
        if [ "$var" != "$var2" ]
            then
                if [ "$var2" == "f" ]
                    then 
                        for var3 in "-$var$var2 $PATTERN_FILE_1 $TEST_FILE_1" "-$var$var2$PATTERN_FILE_1 -$var2 $PATTERN_FILE_2 $TEST_FILE_1 $TEST_FILE_2"
                        do
                        echo "$var3"  
                        ./s21_grep $var3 > s21_grep.txt
                        grep $var3 > grep.txt
                        DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
                            if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
                                then 
                                (( COUNTER_SUCCESS++ ))
                                else
                                echo "$var3" >> log_grep.txt
                                (( COUNTER_FAIL++ ))
                            fi
                        rm s21_grep.txt grep.txt
                        done
                elif [ "$var2" == "e" ]
                    then
                        for var4 in "-$var$var2 $PATTERN $TEST_FILE_1" "-$var$var2$PATTERN -$var2 $PATTERN_2 $TEST_FILE_1 $TEST_FILE_2"
                        do
                        echo "$var4"  
                        ./s21_grep $var4 > s21_grep.txt
                        grep $var4 > grep.txt
                        DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
                            if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
                                then 
                                (( COUNTER_SUCCESS++ ))
                                else
                                echo "$var4" >> log_grep.txt
                                (( COUNTER_FAIL++ ))
                            fi
                        rm s21_grep.txt grep.txt
                        done
                else
                    for var5 in "-$var$var2 $PATTERN $TEST_FILE_1" "-$var$var2 $PATTERN $TEST_FILE_1 $TEST_FILE_2"
                    do
                    echo "$var5"  
                    ./s21_grep $var5 > s21_grep.txt
                    grep $var5 > grep.txt
                    DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
                        if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
                            then 
                            (( COUNTER_SUCCESS++ ))
                            else
                            echo "$var5" >> log_grep.txt
                            (( COUNTER_FAIL++ ))
                        fi
                    rm s21_grep.txt grep.txt
                    done
                fi 
        fi
    done
done
echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"