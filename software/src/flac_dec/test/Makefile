test: test.c $(wildcard ../*.c) $(wildcard ../*.h)
	gcc -o test -O2 -D TEST -I. test.c ../flac_dec.c ../fft.c ../fft_tabs.c ../utils.c

.PHONY: clean
clean:
	rm test *.pcm *.ppm
