library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY shift_li is
	PORT (		
		a : in std_logic_vector(31 downto 0);
		n : in std_logic_vector(4 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END shift_li;

architecture rtl of shift_li is

begin
   r <= std_logic_vector(shift_left(unsigned(a), to_integer(unsigned(n))));
end rtl;

