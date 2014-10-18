onerror {resume}
quietly WaveActivateNextPane {} 0

add wave -divider "Testbench"
add wave sim:/avalon_demo_tb/*
add wave -divider "UUT"
add wave sim:/avalon_demo_tb/uut/*
add wave -divider "Knightrider"
add wave sim:/avalon_demo_tb/uut/kitt/*

TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {4475000 ps} 0}
configure wave -namecolwidth 347
configure wave -valuecolwidth 185
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
update
WaveRestoreZoom {4333956 ps} {5517692 ps}

run 10 us

view wave
