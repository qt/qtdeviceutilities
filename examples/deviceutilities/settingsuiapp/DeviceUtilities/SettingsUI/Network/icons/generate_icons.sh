# Copyright (C) 2017 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#!/bin/bash
#
# This script resizes and sets correct brand colors for the icons in ref folder
#

./scripts/cimages.sh ./ref/*.png
./scripts/cimage.sh ./ref/extra/Alert.png red
./scripts/cimage.sh ./ref/extra/Alert.png yellow
./scripts/rimages.sh *.png
