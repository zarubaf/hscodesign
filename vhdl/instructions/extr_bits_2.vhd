library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY extr_bits_2 is
	PORT (		
		a : in std_logic_vector(31 downto 0);
		pn : in std_logic_vector(31 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END extr_bits_2;

architecture rtl of extr_bits_2 is

signal p : unsigned(5 downto 0);
signal n : unsigned(5 downto 0);
signal ovf : signed(6 downto 0);

begin

p <= resize(unsigned(pn(10 downto 6)), p'length);
n <= resize(unsigned(pn(5 downto 0)) - 1, n'length);
ovf <= signed(resize(p + n, ovf'length)) - 31;

	comb : process (p, n, ovf, a)
	begin
		
		r <= std_logic_vector(shift_right(unsigned(a), to_integer(32 - ovf)));
	
	end process comb;	

end rtl;
