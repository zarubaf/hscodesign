library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.all;

entity SDCard_Clock_tb is
end SDCard_Clock_tb;

architecture behaviour of SDCard_Clock_tb is

	constant  cc		: TIME := 20 ns; 

	signal a :  std_logic_vector(31 downto 0);
	signal 	pn :  std_logic_vector(31 downto 0);
	signal	r :  std_logic_vector(31 downto 0);

begin

	uut: entity work.extr_bits_1
		port map 
		(
			a       => a,
			pn   => pn,
			r   => r			
		);

	test : process
	begin
	
		report "starting simulation";
		wait for 10 ns;
		a <= "10001000100010001000100010001000";
		pn <= "00000000000000000000000011100000";
		wait for 10 ns;

		wait for 1 ms;
		
		report "simulation done";

		wait;
	end process test;

end behaviour; 

