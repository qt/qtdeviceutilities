# Copyright (C) 2017 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#!/bin/bash

for dir in "$@"
do
    echo "$dir"
    scripts/cimage.sh $dir qt
    scripts/cimage.sh $dir black
    scripts/cimage.sh $dir white
    scripts/cimage.sh $dir lightgray
done
