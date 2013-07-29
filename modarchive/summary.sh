#! /bin/sh

for f in 669 ahx hvl it med mo3 mod mtm oct okt s3m stm xm; do
 echo $f
 pushd $f >/dev/null
 ../all-songs.sh | wc -l
 popd >/dev/null
done

echo Total files:
./all-songs.sh | wc -l

echo Total songs in database:
sqlite3 $1 'SELECT count(id) FROM songs;'
