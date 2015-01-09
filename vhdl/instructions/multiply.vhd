library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY multiply is
    PORT (
        a : in std_logic_vector(31 downto 0);
        b : in std_logic_vector(31 downto 0);

        r : out std_logic_vector(31 downto 0)
    );
END multiply;

architecture rtl of multiply is

begin
    r <= std_logic_vector(signed(a(15 downto 0))*signed(b(15 downto 0)));

end rtl;
