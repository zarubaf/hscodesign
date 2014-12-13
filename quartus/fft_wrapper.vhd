library IEEE;
use IEEE.std_logic_1164.all;
library fft_pkg;
use fft_pkg.all;

ENTITY fft_wrapper is
	PORT (
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

begin
	fft_inst : fft
		port map (
		clk  =>  clk,
		reset_n  =>  reset_n,
		inverse  =>  inverse,
		sink_valid  =>  sink_valid,
		sink_sop  =>  sink_sop,
		sink_eop  =>  sink_eop,
		sink_real  =>  sink_real,
		sink_imag  =>  sink_imag,
		sink_ready  =>  sink_ready,
		sink_error  =>  sink_error,
		source_error  =>  source_error,
		source_ready  =>  source_ready,
		source_sop  =>  source_sop,
		source_eop  =>  source_eop,
		source_valid  =>  source_valid,
		source_exp  =>  source_exp,
		source_real  =>  source_real,
		source_imag  =>  source_imag
	);
end wrapper;