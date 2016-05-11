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

for dir in "$@"
do
    echo "$dir"
    scripts/cimage.sh $dir qt
    scripts/cimage.sh $dir black
    scripts/cimage.sh $dir white
    scripts/cimage.sh $dir lightgray
done
