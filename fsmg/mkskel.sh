#! /bin/sh

cat <<!
/* File created from `basename $1`.skl via mkskel.sh */


const unsigned char `basename $1`[] = {
!

sed 's/\/\*.*\*\///g' $1.skl | sed "s/\(.\)/'\1',/g" | sed 's/\\/\\\\/g' | sed "s/'''/'\\\\''/g" | sed "s/$/'\\\n',/g" 

cat <<!
0
};
!
