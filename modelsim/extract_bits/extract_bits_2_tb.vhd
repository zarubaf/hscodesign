library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.all;

entity extr_bit_2_tb is
end extr_bit_2_tb;

architecture behaviour of extr_bit_2_tb is

	constant  cc		: TIME := 20 ns; 

	signal a:  std_logic_vector(31 downto 0);
	signal pn :  std_logic_vector(31 downto 0);
	signal r, r_1 :  std_logic_vector(31 downto 0);
	signal res : std_logic_vector(31 downto 0);

begin

	uut: entity work.extr_bits_2
		port map 
		(
			a       => a,
			pn   => pn,
			r   => r			
		);

	uut_1: entity work.extr_bits_1
		port map 
		(
			a       => a,
			pn   => pn,
			r   => r_1			
		);


	test : process
	begin
	
		report "starting simulation";
		wait for 1 ns;
		--a <= "11101111111111110111111111111111";
		--pn <= "00000000000000000000000010000111";
		--wait for 1 ns;
		a <= "11101111111111110111111111111111";
		pn <= "00000000000000000000011111100000";

		res <= r or r_1;
		wait for 10 ns;

		a <= "11101111111111110111111111111111";
		
		pn <= "00000000000000000000011111000110";
		res <= r or r_1;
		wait for 1 ms;
		
		report "simulation done";

		wait;
	end process test;

end behaviour; 


