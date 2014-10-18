

all: compile

compile: 
	vsim -c -do "do compile.do;quit -f"

compilelib: 
	vsim -c -do "do compile_all.do;quit -f"

sim: 
	vsim -c -do "do sim.do;run 10 us;quit -f"



clean:
	rm -fr work
	rm -f transcript
	rm -f *.wlf
	rm -f modelsim.ini
	

.PHONY: clean
.PHONY: compile
.PHONY: sim
.PHONY: compilelib
