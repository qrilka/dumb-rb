#!/bin/bash
SRC=/tmp/src
DST=/tmp/dst
#dd if=/dev/urandom of=$SRC bs=1M count=2
cp /usr/share/doc/ruby/copyright $SRC
i=0; while [ $((i++)) -ne 500 ]; do
    cat /usr/share/doc/ruby/copyright >> $SRC
done
cat $SRC | ./filter > $DST
cmp $SRC $DST
case "$?" in
    "0")
        echo "Files are equal";;
    "1")
        echo "Files are NOT equal"
        exit 1;;
    "2")
        echo "Problem comparing files"
        exit 2;;
esac