library IEEE;
use IEEE.std_logic_1164.all;
library fft_pkg;
use fft_pkg.all;

ENTITY fft_wrapper is
	PORT (
		clk	: IN STD_LOGIC;
		reset_n	: IN STD_LOGIC;
		sink_valid	: IN STD_LOGIC;
		sink_sop	: IN STD_LOGIC;
		sink_eop	: IN STD_LOGIC;
		sink_real	: IN STD_LOGIC_VECTOR (15 DOWNTO 0);
		sink_error	: IN STD_LOGIC_VECTOR (1 DOWNTO 0);
		sink_ready	: OUT STD_LOGIC;
		source_ready	: IN STD_LOGIC;
		source_error	: OUT STD_LOGIC_VECTOR (1 DOWNTO 0);
		source_sop	: OUT STD_LOGIC;
		source_eop	: OUT STD_LOGIC;
		source_valid	: OUT STD_LOGIC;
		source_data	: OUT STD_LOGIC_VECTOR (31 DOWNTO 0)
	);
	
END fft_wrapper;

architecture wrapper of fft_wrapper is

	component fft is
		port(
			clk	: IN STD_LOGIC;
			reset_n	: IN STD_LOGIC;
			inverse	: IN STD_LOGIC;
			sink_valid	: IN STD_LOGIC;
			sink_sop	: IN STD_LOGIC;
			sink_eop	: IN STD_LOGIC;
			sink_real	: IN STD_LOGIC_VECTOR (15 DOWNTO 0);
			sink_imag	: IN STD_LOGIC_VECTOR (15 DOWNTO 0);
			sink_error	: IN STD_LOGIC_VECTOR (1 DOWNTO 0);
			source_ready	: IN STD_LOGIC;
			sink_ready	: OUT STD_LOGIC;
			source_error	: OUT STD_LOGIC_VECTOR (1 DOWNTO 0);
			source_sop	: OUT STD_LOGIC;
			source_eop	: OUT STD_LOGIC;
			source_valid	: OUT STD_LOGIC;
			source_exp	: OUT STD_LOGIC_VECTOR (5 DOWNTO 0);
			source_real	: OUT STD_LOGIC_VECTOR (15 DOWNTO 0);
			source_imag	: OUT STD_LOGIC_VECTOR (15 DOWNTO 0)	
			);
	end component fft;
signal sink_real_reorder : STD_LOGIC_VECTOR(15 DOWNTO 0);
signal sink_error_reorder : STD_LOGIC_VECTOR(1 DOWNTO 0);

begin
	fft_inst : fft
		port map (
		clk  =>  clk,
		reset_n  =>  reset_n,
		inverse  =>  '0',
		sink_valid  =>  sink_valid,
		sink_sop  =>  sink_sop,
		sink_eop  =>  sink_eop,
		sink_real  =>  sink_real,
		sink_imag  =>  (others => '0'),
		sink_ready  =>  sink_ready,
		sink_error  =>  sink_error_reorder,
		source_error  =>  source_error,
		source_ready  =>  source_ready,
		source_sop  =>  source_sop,
		source_eop  =>  source_eop,
		source_valid  =>  source_valid,
		source_exp  =>  open,
		source_real  =>  source_data(0 DOWNTO 15),
		source_imag  =>  source_data(16 DOWNTO 31)
	);
	
	sink_error_reorder(1) <= sink_error(0);
	sink_error_reorder(0) <= sink_error(1);
	
	sink_real_reorder(15) <= sink_real(0);
	sink_real_reorder(14) <= sink_real(1);
	sink_real_reorder(13) <= sink_real(2);
	sink_real_reorder(12) <= sink_real(3);
	sink_real_reorder(11) <= sink_real(4);
	sink_real_reorder(10) <= sink_real(5);
	sink_real_reorder(9) <= sink_real(6);
	sink_real_reorder(8) <= sink_real(7);
	sink_real_reorder(7) <= sink_real(8);
	sink_real_reorder(6) <= sink_real(9);
	sink_real_reorder(5) <= sink_real(10);
	sink_real_reorder(4) <= sink_real(11);
	sink_real_reorder(3) <= sink_real(12);
	sink_real_reorder(2) <= sink_real(13);
	sink_real_reorder(1) <= sink_real(14);
	sink_real_reorder(0) <= sink_real(15);
	
end wrapper;