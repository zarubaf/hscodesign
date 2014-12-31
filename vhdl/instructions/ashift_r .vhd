library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY ashift_r is
	PORT (		
		a : in std_logic_vector(31 downto 0);
		b : in std_logic_vector(31 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END ashift_r;

architecture rtl of ashift_r is

begin
	r <= std_logic_vector(shift_right(signed(a), to_integer(unsigned(b(31 downto 0)))));

end rtl;
