

PROJECT_NAME=de2_115_nios2

LOG_FILE=BUILD_$(shell date '+%d.%m.%y-%H_%M_%S').log
PROJECT_DIR=$(shell pwd)
OUTPUT_DIR=$(PROJECT_DIR)/$(PROJECT_NAME)
SOPC_INFO_FILE=$(PROJECT_NAME).sopcinfo
QSYS_FILE=$(PROJECT_NAME).qsys

SOF_FILE=output_files/$(PROJECT_NAME).sof

all: $(SOF_FILE)

quartus: $(SOF_FILE)

# starts the quartus GUI and loads the quartus project
quartus_gui:
	quartus --64bit $(PROJECT_NAME).qpf 

$(SOF_FILE): $(QSYS_FILE) $(PROJECT_NAME).qsf $(PROJECT_NAME).qpf
	@echo ""
	@echo "---------------------------------------------" >> $(LOG_FILE)
	@echo "-                  QUARTUS                  -" >> $(LOG_FILE)
	@echo "---------------------------------------------" >> $(LOG_FILE)
	quartus_sh --flow compile $(PROJECT_NAME) 2>&1 | tee -a $(LOG_FILE) 
	
download:
	quartus_pgm -m jtag -o"p;$(SOF_FILE)"
	
	
clean:
	rm -f $(PROJECT_NAME).sopcinfo
	rm -f $(PROJECT_NAME).html
	rm -f $(PROJECT_NAME).cmp
	rm -f *.rpt
	rm -f *.qws
	rm -fr $(OUTPUT_DIR)
	rm -fr output_files
	rm -fr db
	rm -f *.bsf
	rm -f *.dpf
	rm -f *.srf
	rm -fr incremental_db
	rm -f *.jdi
	rm -f *.html
	rm -f *.smsg
	rm -f *.summary
	rm -f *.qdf

.PHONY: quartus
.PHONY: quartus_gui
.PHONY: download
.PHONY: clean

