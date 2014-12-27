library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY extr_bits_1 is
	PORT (		
		------------- register 32 bits -----------
		a : in std_logic_vector(31 downto 0);
		------------------------------------------
		--xxx 21 bits	p,5 bits	n, 6 bits-
		------------------------------------------
		pn : in std_logic_vector(31 downto 0);
		
		-- result 32 bit
		r : out std_logic_vector(31 downto 0)
	);
END extr_bits_1;

architecture rtl of extr_bits_1 is

signal r_tmp : std_logic_vector(31 downto 0);
signal p : unsigned(5 downto 0);
signal n : unsigned(5 downto 0);
signal ovf : signed(6 downto 0);
begin

p <= resize(unsigned(pn(10 downto 6)), p'length);
n <= resize(unsigned(pn(5 downto 0)) - 1, n'length);
ovf <= signed(resize(p + n, ovf'length)) - 31;

	comb : process (p, n, ovf, a, r_tmp)
	begin
	
		r_tmp <= std_logic_vector(shift_left(unsigned(a), to_integer(ovf)));
		
		for i in r'range loop
			if i <= to_integer(n) then
				r(i) <= r_tmp(i);
			else
				r(i) <= '0';
			end if;
		end loop;
		 
	end process comb;	
end rtl;
