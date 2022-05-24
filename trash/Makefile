cc=gcc
flags=-Wall -pedantic -Wextra -g  -Wno-unused-parameter 
ldlibs=`pkgconf --libs libusb-1.0`
exe=evofox
install_dir=/usr/local/bin

evofox: evofox.c
	${cc} ${flags} -o ${exe} evofox.c ${ldlibs}

install: ${exe}
	cp ./${exe} ${install_dir}/${exe}

uninstall:
	rm ${install_dir}/${exe}

clean:
	rm  *.o
