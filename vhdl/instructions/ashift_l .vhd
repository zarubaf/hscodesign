library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY ashift_l is
	PORT (		
		a : in std_logic_vector(31 downto 0);
		b : in std_logic_vector(31 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END ashift_l;

architecture rtl of ashift_l is

begin
	r <= std_logic_vector(shift_left(signed(a), to_integer(unsigned(b(31 downto 0)))));
end rtl;
