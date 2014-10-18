

all:
	make -C quartus
	make -C software
	make -C quartus download
	make -C software run


clean:
	make -C quartus clean
	make -C software clean


