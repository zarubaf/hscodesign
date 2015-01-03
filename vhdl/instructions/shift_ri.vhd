library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY shift_ri is
	PORT (		
		a : in std_logic_vector(31 downto 0);
		n : in std_logic_vector(7 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END shift_ri;

architecture rtl of shift_ri is

begin
	decide : process (a,n)
	begin
		if n = "00110000" then		
			r <= "0" & a(31 downto 1);
		elsif n = "00110001" then
			r <= "00" & a(31 downto 2);
		elsif n = "00110010" then
			r <= "000" & a(31 downto 3);
		elsif n = "00110011" then
			r <= "0000" & a(31 downto 4);
		elsif n = "00110100" then
			r <= "00000" & a(31 downto 5);
		elsif n = "00110101" then
			r <= "000000" & a(31 downto 6);
		elsif n = "00110111" then
			r <= "0000000" & a(31 downto 7);
		elsif n = "00111000" then
			r <= "00000000" & a(31 downto 8);
		elsif n = "00111001" then
			r <= "000000000" & a(31 downto 9);
		elsif n = "00111010" then
			r <= "0000000000" & a(31 downto 10);
		elsif n = "00111011" then
			r <= "00000000000" & a(31 downto 11);
		elsif n = "00111100" then
			r <= "000000000000" & a(31 downto 12);
		elsif n = "00111101" then
			r <= "0000000000000" & a(31 downto 13);
		elsif n = "00111110" then
			r <= "00000000000000" & a(31 downto 14);
		elsif n = "00111111" then
			r <= "000000000000000" & a(31 downto 14);
		elsif n = "01000000" then
			r <= "0000000000000000" & a(31 downto 15);
		elsif n = "01000001" then
			r <= "00000000000000000" & a(31 downto 16);
		elsif n = "01000010" then
			r <= "000000000000000000" & a(31 downto 17);
		elsif n = "01000011" then
			r <= "0000000000000000000" & a(31 downto 18);
		elsif n = "01000100" then
			r <= "00000000000000000000" & a(31 downto 19);
		elsif n = "01000101" then
			r <= "000000000000000000000" & a(31 downto 20);
		elsif n = "01000110" then
			r <= "0000000000000000000000" & a(31 downto 21);
		elsif n = "01000111" then
			r <= "00000000000000000000000" & a(31 downto 22);
		elsif n = "01001000" then
			r <= "000000000000000000000000" & a(31 downto 23);
		elsif n = "01001001" then
			r <= "0000000000000000000000000" & a(31 downto 24);
		elsif n = "01001010" then
			r <= "00000000000000000000000000" & a(31 downto 25);
		elsif n = "01001011" then
			r <= "000000000000000000000000000" & a(31 downto 26);
		elsif n = "01001100" then
			r <= "0000000000000000000000000000" & a(31 downto 27);
		elsif n = "01001101" then
			r <= "00000000000000000000000000000" & a(31 downto 28);
		elsif n = "01001110" then
			r <= "000000000000000000000000000000" & a(31 downto 29);
		elsif n = "01001111" then
			r <= "0000000000000000000000000000000" & a(31);
		end if;
	end process;
end rtl;
