OBJECTS=ff_flac.o flac_dec.o flac_dsp.o golomb.o utils.o

all: run

run: ff_flac
	./ff_flac ../../hscodesign/matlab/flac/marvin.flac out.pcm
	cat dummy_header.wav out.pcm > out.wav

ff_flac: $(OBJECTS)
	gcc -o ff_flac $(OBJECTS)

%.o: %.c
	gcc -c -o $@ $<

clean:
	rm out.wav out.pcm ff_flac *.o
