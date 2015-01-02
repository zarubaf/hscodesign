library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY ashift_r_15 is
	PORT (		
		a : in std_logic_vector(31 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END ashift_r_15;

architecture rtl of ashift_r_15 is

begin
	--r <= std_logic_vector(shift_right(signed(a), to_integer(unsigned(std_logic_vector(15)))));
	decide : process (a)
	begin
		if a(31) = '1' then
			r <= "1111111111111111" & a(30 downto 15);
		else
			r <= "0000000000000000" & a(30 downto 15);
		end if;
	end process;
end rtl;
