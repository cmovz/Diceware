PROGRAM_DEST="/usr/local/bin/"
DATA_DEST="/usr/local/share/cmovz/diceware"

mkdir -p $DATA_DEST
cp dictionary.dice $DATA_DEST
cp diceware $PROGRAM_DEST
