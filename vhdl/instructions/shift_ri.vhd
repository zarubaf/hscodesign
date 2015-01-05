library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY shift_ri is
	PORT (		
		a : in std_logic_vector(31 downto 0);
		n : in std_logic_vector(4 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END shift_ri;

architecture rtl of shift_ri is

begin
   r <= std_logic_vector(shift_right(unsigned(a), to_integer(unsigned(n))));
end rtl;

