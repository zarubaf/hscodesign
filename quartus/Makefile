
DEVICE_FAMILY="Cyclone IV E"
DEVICE=EP4CE22F17C6N
DEVICE_SPEEDGRADE=7 
LOG_FILE=BUILD_$(shell date '+%d.%m.%y-%H_%M_%S').log


PROJECT_NAME=de0_nano_system
PROJECT_DIR=$(shell pwd)
OUTPUT_DIR=$(PROJECT_DIR)/$(PROJECT_NAME)
SYNTHESIS_DIR=$(OUTPUT_DIR)/synthesis

all: qsys quartus

qsys:
	@echo "---------------------------------------------" >> $(LOG_FILE)
	@echo "-                 QSYS                      -" >> $(LOG_FILE)
	@echo "---------------------------------------------" >> $(LOG_FILE)
	ip-generate \
	--project-directory=$(PROJECT_DIR)   \
	--output-directory=$(OUTPUT_DIR)     \
	--report-file=bsf:$(PROJECT_DIR)/$(PROJECT_NAME).bsf \
	--system-info=DEVICE_FAMILY=$(DEVICE_FAMILY) \
	--system-info=DEVICE=$(DEVICE) \
	--system-info=DEVICE_SPEEDGRADE=$(DEVICE_SPEEDGRADE) \
	--component-file=$(PROJECT_DIR)/$(PROJECT_NAME).qsys 2>&1 | tee -a $(LOG_FILE) 
	ip-generate \
	--project-directory=$(PROJECT_DIR) \
	--output-directory=$(SYNTHESIS_DIR) \
	--file-set=QUARTUS_SYNTH \
	--report-file=sopcinfo:$(PROJECT_DIR)/$(PROJECT_NAME).sopcinfo \
	--report-file=html:$(PROJECT_DIR)/$(PROJECT_NAME).html \
	--report-file=qip:$(SYNTHESIS_DIR)/$(PROJECT_NAME).qip \
	--report-file=cmp:$(PROJECT_DIR)/$(PROJECT_NAME).cmp \
	--system-info=DEVICE_FAMILY=$(DEVICE_FAMILY) \
	--system-info=DEVICE=$(DEVICE) \
	--system-info=DEVICE_SPEEDGRADE=$(DEVICE_SPEEDGRADE) \
	--component-file=$(PROJECT_DIR)/$(PROJECT_NAME).qsys 2>&1 | tee -a $(LOG_FILE) 

quartus:
	@echo ""
	@echo "---------------------------------------------" >> $(LOG_FILE)
	@echo "-                  QUARTUS                  -" >> $(LOG_FILE)
	@echo "---------------------------------------------" >> $(LOG_FILE)
	quartus_sh --flow compile displaydriver 2>&1 | tee -a $(LOG_FILE) 
	
download:
	quartus_pgm -m jtag -c USB-Blaster[1-1.4] -o"p;output_files/"$(PROJECT_NAME)".sof"
	
	
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

