library ieee;
use ieee.std_logic_1164.all;

package knightrider_pkg is
	component knightrider is
	generic
	(
		T_STEP  : integer := (200 ms)/(20 ns);
		LED_NUM : integer range 4 to integer'high:= 8
	);
	port
	(
		clk    : in  std_logic;
		rst_n  : in  std_logic;
		
		en     : in  std_logic;
		leds   : out std_logic_vector(LED_NUM-1 downto 0)
	);
	end component knightrider;
end package knightrider_pkg;
