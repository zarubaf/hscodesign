vlib work
vmap work work

set ALTERA_LIB_PATH /opt/altera/13.1/quartus/eda/sim_lib

vcom  -work altera_mf $ALTERA_LIB_PATH/altera_mf_components.vhd
vcom  -work altera_mf $ALTERA_LIB_PATH/altera_mf.vhd

#vcom  -work stratixii $ALTERA_LIB_PATH/stratixii_atoms.vhd
#vcom  -work stratixii $ALTERA_LIB_PATH/stratixii_components.vhd

#do compile.do
