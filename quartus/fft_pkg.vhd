library IEEE;
use IEEE.std_logic_1164.all;

package fft_pkg is
	component fft is
		port (
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
end fft_pkg;