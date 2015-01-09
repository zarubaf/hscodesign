
ifndef QUARTUS_ROOTDIR
$(error The environment variable QUARTUS_ROOTDIR is NOT set. \
This indicates that you are not running the Nios2 Command Shell. \
Execute /path/to/altera_dir/nios2eds/nios2_command_shell.sh and run make again. \
To find the path to the nios2_command_shell you can run the command "which quartus". \
This should give you "/path/to/altera_dir/quartus/bin/quartus" )
endif


# PROJECT_NAME must contain the name of the quartus project
PROJECT_NAME=de0_nano_nios2
SOPC_FILE=../quartus/$(PROJECT_NAME).sopcinfo
BSP_SETTINGS_FILE=./settings.bsp
BSP_DIR=./bsp
QUARTUS_PROJECT_DIR=`pwd`/../../quartus
APPLICATION_MAKEFILE=$(PROJECT_NAME)/Makefile 
ELF_FILE=$(PROJECT_NAME)/$(PROJECT_NAME).elf

#used in the create-app shell script
export PROJECT_NAME
export QUARTUS_PROJECT_DIR
export BSP_DIR


#info: The application makefile automatically runs the bsp makefile

all: $(ELF_FILE) 
	

run: $(ELF_FILE) 
	nios2-download -g $(ELF_FILE) && nios2-terminal

download: 
	nios2-download -g $(ELF_FILE)

term:
	nios2-terminal

edit_bsp:
	nios2-bsp-editor &

$(ELF_FILE): $(APPLICATION_MAKEFILE)
	make -C $(PROJECT_NAME)


$(APPLICATION_MAKEFILE): $(BSP_DIR) 
	./create-app-makefile


$(BSP_DIR): $(SOPC_FILE) $(BSP_SETTINGS_FILE)
	mkdir -p $(BSP_DIR)
	nios2-bsp-update-settings --settings $(BSP_SETTINGS_FILE) --sopc $(SOPC_FILE)
	nios2-bsp-generate-files --settings $(BSP_SETTINGS_FILE) --bsp-dir $(BSP_DIR)
	cp src/system.h bsp/system.h -f

clean:
	rm -rf $(BSP_DIR)
	rm -rf $(PROJECT_NAME)
	rm -rf *.bak
	rm -rf *~


.PHONY: bsp_edit
.PHONY: term
.PHONY: download
.PHONY: run


