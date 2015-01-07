library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY ashift_ri is
    PORT (
        a : in std_logic_vector(31 downto 0);
        n : in std_logic_vector(4 downto 0);

        r : out std_logic_vector(31 downto 0)
    );
END ashift_ri;

architecture rtl of ashift_ri is

begin
   r <= std_logic_vector(shift_right(signed(a), to_integer(unsigned(n))));
end rtl;

