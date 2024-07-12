# Copyright (C) 2017 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
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

