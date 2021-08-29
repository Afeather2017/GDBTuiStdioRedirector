make:rdicgdberr rdicgdbout test 
	echo -n done
rdicgdbout:rdicgdbout.c gdbout_api.h
	gcc rdicgdbout.c -o rdicgdbout -g
rdicgdberr:rdicgdberr.c gdbout_api.h
	gcc rdicgdberr.c -o rdicgdberr -g
test:test.c gdbout_api.h
	gcc test.c -o test -g
clear:
	rm rdicgdberr rdicgdbout test 
install:make
	cp ./gdbout_api.h /usr/include
install_all:install
	cp ./rdicgdberr ./rdicgdbout ./serverrun.sh /usr/bin
