#! /bin/sh
ALL_SONG_TYPES='\.669$|\.ahx$|\.hvl$|\.it$|\.med$|\.mo3$|\.mod$|\.mtm$|\.oct$|\.okt$|\.s3m$|\.stm$|\.xm$'
find . -type f | grep -E $ALL_SONG_TYPES
