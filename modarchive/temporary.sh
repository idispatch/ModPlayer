 for f in `find . -type d | grep -v '^.$'`; do pushd $f; cat `ls | grep 'index'` > all.html; popd;  done

 for f in `find . -type d | grep -v '^.$'`; do pushd $f>/dev/null; mv all.html all-$(echo $(ls | grep 'index' | head -n 1) | sed -ne 's:.*query=\([0-9]*\).*:\1:p').html;  popd>/dev/null; done