DEVICE="/dev/ttyUSB0"
FW="fw/$1.hex"

if [ -f $FW ]; then
    avrdude -p atmega328p -c arduino -b 57600 -P $DEVICE -D -U flash:w:$FW:i
else
    echo "File $FW not found"
fi
