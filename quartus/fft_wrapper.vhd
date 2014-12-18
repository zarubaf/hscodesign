library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.ALL;
use IEEE.std_logic_unsigned.ALL;
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
		source_data	: OUT STD_LOGIC_VECTOR (15 DOWNTO 0)
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

signal source_real_reorder : STD_LOGIC_VECTOR(15 DOWNTO 0);
signal source_imag_reorder : STD_LOGIC_VECTOR(15 DOWNTO 0);

signal source_real_sqare : STD_LOGIC_VECTOR(15 DOWNTO 0);
signal source_imag_sqare : STD_LOGIC_VECTOR(15 DOWNTO 0);
signal source_squared : STD_LOGIC_VECTOR(15 DOWNTO 0);

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
		sink_error  =>  sink_error,
		source_error  =>  source_error,
		source_ready  =>  source_ready,
		source_sop  =>  source_sop,
		source_eop  =>  source_eop,
		source_valid  =>  source_valid,
		source_exp  =>  open,
		source_real  =>  source_real_reorder,--source_data(0 DOWNTO 15),
		source_imag  =>  source_imag_reorder--source_data(16 DOWNTO 31)
	);
	
	-------------- input byte swap -------------	
	sink_real_reorder(15 downto 8) <= sink_real(7 downto 0);	
	sink_real_reorder(7 downto 0) <= sink_real(15 downto 8);
	
	-------------- calculation sum ----------------
	source_real_sqare <= signed(source_real_reorder)*signed(source_real_reorder);
	source_imag_sqare <= signed(source_imag_reorder)*signed(source_imag_reorder);
	source_squared <= signed(source_real_sqare) + signed(source_imag_sqare);
	
	-------------- output byte swap --------------
	source_data(15 downto 8) <= source_squared(7 downto 0);	
	source_data(7 downto 0) <= source_squared(15 downto 8);
	
end wrapper;