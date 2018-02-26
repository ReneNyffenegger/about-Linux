#!/bin/bash
#
# Create a udev-rules file.
# 
# Such a file is necessary if the /dev/resrever node
# is required to have rw permissions for everyone (mode = 0666).
#
# The file consists of three parameters: KERNEL, SUBSYSTEM and MODE.
#
# The required values for KERNEL and SUBSYSTEM can be found with udevadm after
# the module was loaded.
#
#    udevadm info -a -p /sys/class/tq84_class/resrever
#

rule_file=/etc/udev/rules.d/99-resrever.rules

udevadm info -a -p /sys/class/tq84_class/resrever | 
  grep -e 'KERNEL' -e 'SUBSYSTEM'  > $rule_file

echo 'MODE="0666"' >> $rule_file

cat $rule_file

# cat <<RULE > /etc/udev/rules.d/99-resrever
# KERNEL="resrever"
# SUBSYSTEM="tq84_class"
# MODE="0666"
# RULE
