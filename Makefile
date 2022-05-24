src_file=evofox-phantom.py
exe_file=$(shell echo ${src_file} | cut -d . -f 1)
install_dir=/usr/local/bin

install: ${src_file}
	cp ./${src_file} ${install_dir}/${exe_file}

uninstall:
	rm ${install_dir}/${exe_file}
