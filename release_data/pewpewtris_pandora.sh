#!/bin/bash

nub0_before=$(cat /proc/pandora/nub0/mode)
nub1_before=$(cat /proc/pandora/nub1/mode)

/usr/pandora/scripts/op_nubchange.sh absolute absolute

LD_LIBRARY_PATH="$LD_LIBRARY_PATH:." ./pewpewtris.elf

/usr/pandora/scripts/op_nubchange.sh $nub0_before $nub1_before

