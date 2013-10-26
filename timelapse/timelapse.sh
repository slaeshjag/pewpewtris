#!/bin/bash


while [ 1 ]; do
	echo "snapsnap"
	scrot screenshot.png
	convert screenshot.png -crop '1920x1080+1920+0' edit.png
	rm screenshot.png
	mv edit.png frames/`date "+%d-_-%H-%M-%S.png"`
	sleep 4
done
