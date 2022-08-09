#/bin/bash

mount /dev/mmcblk0p1 /media/mtca/Test

for counter in {1..10}
do
   cp /home/mtca/SDKiller/test.txt /media/mtca/Test/test.txt
   rm /media/mtca/Test/test.txt

   cp /home/mtca/SDKiller/test1.txt /media/mtca/Test/test1.txt
   diff /home/mtca/SDKiller/test1.txt /media/mtca/Test/test1.txt >> /home/mtca/test.log
   rm /media/mtca/Test/test1.txt
done

now=$(date +"%F %r")

echo "Current date: $now" >> /home/mtca/SDKiller/test.log

umount /media/mtca/Test
