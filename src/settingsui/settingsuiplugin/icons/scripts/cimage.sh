############################################################################
##
## Copyright (C) 2017 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of the Device Utilities module of the Qt Toolkit.
##
## $QT_BEGIN_LICENSE:GPL$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 or (at your option) any later version
## approved by the KDE Free Qt Foundation. The licenses are as published by
## the Free Software Foundation and appearing in the file LICENSE.GPL3
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
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
