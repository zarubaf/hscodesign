library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY shift_l is
	PORT (		
		a : in std_logic_vector(4 downto 0);
		b : in std_logic_vector(4 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END shift_l;

architecture rtl of shift_l is

begin
	r <= std_logic_vector(resize(shift_left(unsigned(b), to_integer(unsigned(a(4 downto 0)))),32));

end rtl;
