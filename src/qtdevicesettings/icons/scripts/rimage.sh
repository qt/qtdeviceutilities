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

postix="_1x"
convert $fullfile -resize x16 $filename$postix.$extension
postix="_2x"
convert $fullfile -resize x32 $filename$postix.$extension
postix="_3x"
convert $fullfile -resize x48 $filename$postix.$extension
postix="_4x"
convert $fullfile -resize x64 $filename$postix.$extension
postix="_5x"
convert $fullfile -resize x80 $filename$postix.$extension
postix="_6x"
convert $fullfile -resize x96 $filename$postix.$extension

