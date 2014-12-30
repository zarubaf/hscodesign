library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY shift_l is
    PORT (
        a : in std_logic_vector(31 downto 0);
        b : in std_logic_vector(31 downto 0);

        r : out std_logic_vector(31 downto 0)
    );
END shift_l;

architecture rtl of shift_l is
    signal j : std_logic_vector(63 downto 0);
    signal n : unsigned(4 downto 0);
begin
    j <= a & x"00000000";
    n <= resize(unsigned(b(4 downto 0)), n'length);

shift_l : process(j, b)
begin
    case n is
        when "00000" => r <= j(63 downto 32);
        when "00001" => r <= j(62 downto 31);
        when "00010" => r <= j(61 downto 30);
        when "00011" => r <= j(60 downto 29);
        when "00100" => r <= j(59 downto 28);
        when "00101" => r <= j(58 downto 27);
        when "00110" => r <= j(57 downto 26);
        when "00111" => r <= j(56 downto 25);
        when "01000" => r <= j(55 downto 24);
        when "01001" => r <= j(54 downto 23);
        when "01010" => r <= j(53 downto 22);
        when "01011" => r <= j(52 downto 21);
        when "01100" => r <= j(51 downto 20);
        when "01101" => r <= j(50 downto 19);
        when "01110" => r <= j(49 downto 18);
        when "01111" => r <= j(48 downto 17);
        when "10000" => r <= j(47 downto 16);
        when "10001" => r <= j(46 downto 15);
        when "10010" => r <= j(45 downto 14);
        when "10011" => r <= j(44 downto 13);
        when "10100" => r <= j(43 downto 12);
        when "10101" => r <= j(42 downto 11);
        when "10110" => r <= j(41 downto 10);
        when "10111" => r <= j(40 downto 9);
        when "11000" => r <= j(39 downto 8);
        when "11001" => r <= j(38 downto 7);
        when "11010" => r <= j(37 downto 6);
        when "11011" => r <= j(36 downto 5);
        when "11100" => r <= j(35 downto 4);
        when "11101" => r <= j(34 downto 3);
        when "11110" => r <= j(33 downto 2);
        when "11111" => r <= j(32 downto 1);
    end case;
end process;
end rtl;
