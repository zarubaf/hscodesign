OBJECTS=ff_flac.o flac_dec.o utils.o

.PHONY: all
all: ff_flac

%.flac: ff_flac
	./ff_flac $@ out.pcm
	cat dummy_header.wav out.pcm > out.wav

run: ff_flac
	./ff_flac ../../hscodesign/matlab/flac/marvin.flac out.pcm
	cat dummy_header.wav out.pcm > out.wav

ff_flac: $(OBJECTS)
	gcc -o ff_flac $(OBJECTS)

%.o: %.c
	gcc -c -o $@ $<

.PHONY: clean
clean:
	rm out.wav out.pcm ff_flac *.o
