
ifndef QUARTUS_ROOTDIR
$(error The environment variable QUARTUS_ROOTDIR is NOT set. \
This indicates that you are not running the Nios2 Command Shell. \
Execute /path/to/altera_dir/nios2eds/nios2_command_shell.sh and run make again. \
On TI-LAB computers the Nios2 Command Shell is should be located in /opt/altera/quartus-12.1_177/nios2eds/nios2_command_shell.sh )
endif



PROJECT_NAME=de0_nano_system
SOPC_FILE=../quartus/de0_nano_nios2.sopcinfo
BSP_DIR=./$(PROJECT_NAME)_bsp
QUARTUS_PROJECT_DIR=../quartus

export PROJECT_NAME
export SOPC_FILE
export BSP_DIR
export QUARTUS_PROJECT_DIR

ELF_FILE=$(PROJECT_NAME)/$(PROJECT_NAME).elf


all: $(ELF_FILE) app-makefile bsp
	

run:
	nios2-download -g $(ELF_FILE) && nios2-terminal
	

$(ELF_FILE): app-makefile
	make -C $(PROJECT_NAME)

app-makefile: $(PROJECT_NAME)/Makefile 

$(PROJECT_NAME)/Makefile: $(BSP_DIR) 
	./create-app-makefile


bsp: $(BSP_DIR)

$(BSP_DIR): $(SOPC_FILE)
	./create-bsp
	


clean:
	rm -rf $(BSP_DIR)
	rm -rf $(PROJECT_NAME)
	
.PHONY: bsp
