# Copyright (C) 2017 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
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
