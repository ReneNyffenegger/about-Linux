insmod tq84_kobj.ko

echo  42 > /sys/kernel/tq84_kobj/filename_1
echo  99 > /sys/kernel/tq84_kobj/filename_2
echo -37 > /sys/kernel/tq84_kobj/filename_3

cat        /sys/kernel/tq84_kobj/filename_1
cat        /sys/kernel/tq84_kobj/filename_2
cat        /sys/kernel/tq84_kobj/filename_3

rmmod  tq84_kobj.ko
