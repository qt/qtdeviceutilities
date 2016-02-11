############################################################################
##
## Copyright (C) 2015 Digia Plc
## All rights reserved.
## For any questions to Digia, please use the contact form at
## http://www.qt.io
##
## This file is part of Qt Enterprise Embedded.
##
## Licensees holding valid Qt Enterprise licenses may use this file in
## accordance with the Qt Enterprise License Agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and Digia.
##
## If you have questions regarding the use of this file, please use
## the contact form at http://www.qt.io
##
#############################################################################
#!/bin/bash

fullfile=$1
filename=$(basename "$fullfile")
extension="${filename##*.}"
filename="${filename%.*}"

color=$2
if [ "$color" = "qt" ]; then
    fill='rgb(128,195,66)'
elif [ "$color" = "black" ]; then
    fill='rgb(32,40,42)'
elif [ "$color" = "white" ]; then
    fill='rgb(255,255,255)'
elif [ "$color" = "lightgray" ]; then
    fill='rgb(214,214,214)'
elif [ "$color" = "red" ]; then
    fill='rgb(228,30,37)'
elif [ "$color" = "yellow" ]; then
    fill='rgb(250,206,32)'
else
    fill="black"
fi

separator="_"

convert $fullfile -fuzz 100% -fill $fill -opaque black $filename$separator$color.$extension
