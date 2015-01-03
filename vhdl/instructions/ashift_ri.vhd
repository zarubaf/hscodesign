library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY ashift_ri is
	PORT (		
		a : in std_logic_vector(31 downto 0);
		n : in std_logic_vector(7 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END ashift_ri;

architecture rtl of ashift_ri is

begin
	decide : process (a,n)
	begin
		if n = "00010000" then			
			if a(31) = '1' then
				r <= "1" & a(30 downto 1);
			else
				r <= "0" & a(30 downto 1);
			end if;
		elsif n = "00010001" then
			if a(31) = '1' then
				r <= "11" & a(30 downto 2);
			else
				r <= "00" & a(30 downto 2);
			end if;
		elsif n = "00010010" then
			if a(31) = '1' then
				r <= "111" & a(30 downto 3);
			else
				r <= "000" & a(30 downto 3);
			end if;
		elsif n = "00010011" then
			if a(31) = '1' then
				r <= "1111" & a(30 downto 4);
			else
				r <= "0000" & a(30 downto 4);
			end if;
		elsif n = "00010100" then
			if a(31) = '1' then
				r <= "11111" & a(30 downto 5);
			else
				r <= "00000" & a(30 downto 5);
			end if;
		elsif n = "00010101" then
			if a(31) = '1' then
				r <= "111111" & a(30 downto 6);
			else
				r <= "000000" & a(30 downto 6);
			end if;
		elsif n = "00010111" then
			if a(31) = '1' then
				r <= "1111111" & a(30 downto 7);
			else
				r <= "0000000" & a(30 downto 7);
			end if;
		elsif n = "00011000" then
			if a(31) = '1' then
				r <= "11111111" & a(30 downto 8);
			else
				r <= "00000000" & a(30 downto 8);
			end if;
		elsif n = "00011001" then
			if a(31) = '1' then
				r <= "111111111" & a(30 downto 9);
			else
				r <= "000000000" & a(30 downto 9);
			end if;
		elsif n = "00011010" then
			if a(31) = '1' then
				r <= "1111111111" & a(30 downto 10);
			else
				r <= "0000000000" & a(30 downto 10);
			end if;
		elsif n = "00011011" then
			if a(31) = '1' then
				r <= "11111111111" & a(30 downto 11);
			else
				r <= "00000000000" & a(30 downto 11);
			end if;
		elsif n = "00011100" then
			if a(31) = '1' then
				r <= "111111111111" & a(30 downto 12);
			else
				r <= "000000000000" & a(30 downto 12);
			end if;
		elsif n = "00011101" then
			if a(31) = '1' then
				r <= "1111111111111" & a(30 downto 13);
			else
				r <= "0000000000000" & a(30 downto 13);
			end if;
		elsif n = "00011110" then
			if a(31) = '1' then
				r <= "11111111111111" & a(30 downto 14);
			else
				r <= "00000000000000" & a(30 downto 14);
			end if;
		elsif n = "00011111" then
			if a(31) = '1' then
				r <= "111111111111111" & a(30 downto 14);
			else
				r <= "000000000000000" & a(30 downto 14);
			end if;
		elsif n = "00100000" then
			if a(31) = '1' then
				r <= "1111111111111111" & a(30 downto 15);
			else
				r <= "0000000000000000" & a(30 downto 15);
			end if;
		elsif n = "00100001" then
			if a(31) = '1' then
				r <= "11111111111111111" & a(30 downto 16);
			else
				r <= "00000000000000000" & a(30 downto 16);
			end if;
		elsif n = "00100010" then
			if a(31) = '1' then
				r <= "111111111111111111" & a(30 downto 17);
			else
				r <= "000000000000000000" & a(30 downto 17);
			end if;
		elsif n = "00100011" then
			if a(31) = '1' then
				r <= "1111111111111111111" & a(30 downto 18);
			else
				r <= "0000000000000000000" & a(30 downto 18);
			end if;
		elsif n = "00100100" then
			if a(31) = '1' then
				r <= "11111111111111111111" & a(30 downto 19);
			else
				r <= "00000000000000000000" & a(30 downto 19);
			end if;
		elsif n = "00100101" then
			if a(31) = '1' then
				r <= "111111111111111111111" & a(30 downto 20);
			else
				r <= "000000000000000000000" & a(30 downto 20);
			end if;
		elsif n = "00100110" then
			if a(31) = '1' then
				r <= "1111111111111111111111" & a(30 downto 21);
			else
				r <= "0000000000000000000000" & a(30 downto 21);
			end if;
		elsif n = "00100111" then
			if a(31) = '1' then
				r <= "11111111111111111111111" & a(30 downto 22);
			else
				r <= "00000000000000000000000" & a(30 downto 22);
			end if;
		elsif n = "00101000" then
			if a(31) = '1' then
				r <= "111111111111111111111111" & a(30 downto 23);
			else
				r <= "000000000000000000000000" & a(30 downto 23);
			end if;
		elsif n = "00101001" then
			if a(31) = '1' then
				r <= "1111111111111111111111111" & a(30 downto 24);
			else
				r <= "0000000000000000000000000" & a(30 downto 24);
			end if;
		elsif n = "00101010" then
			if a(31) = '1' then
				r <= "11111111111111111111111111" & a(30 downto 25);
			else
				r <= "00000000000000000000000000" & a(30 downto 25);
			end if;
		elsif n = "00101011" then
			if a(31) = '1' then
				r <= "111111111111111111111111111" & a(30 downto 26);
			else
				r <= "000000000000000000000000000" & a(30 downto 26);
			end if;
		elsif n = "00101100" then
			if a(31) = '1' then
				r <= "1111111111111111111111111111" & a(30 downto 27);
			else
				r <= "0000000000000000000000000000" & a(30 downto 27);
			end if;
		elsif n = "00101101" then
			if a(31) = '1' then
				r <= "11111111111111111111111111111" & a(30 downto 28);
			else
				r <= "00000000000000000000000000000" & a(30 downto 28);
			end if;
		elsif n = "00101110" then
			if a(31) = '1' then
				r <= "111111111111111111111111111111" & a(30 downto 29);
			else
				r <= "000000000000000000000000000000" & a(30 downto 29);
			end if;
		elsif n = "00101111" then
			if a(31) = '1' then
				r <= "1111111111111111111111111111111" & a(30);
			else
				r <= "0000000000000000000000000000000" & a(30);
			end if;
		end if;
	end process;
end rtl;
