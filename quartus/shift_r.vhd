library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY shift_r is
	PORT (		
		a : in std_logic_vector(31 downto 0);
		b : in std_logic_vector(31 downto 0);

		r : out std_logic_vector(63 downto 0)
	);
END shift_r;

architecture rtl of shift_r is

begin
	r <= std_logic_vector(shift_right(unsigned(b), to_integer(unsigned(a(31 downto 0)))));

end rtl;