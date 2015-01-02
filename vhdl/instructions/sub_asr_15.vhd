library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

ENTITY sub_asr_15 is
	PORT (		
		a : in std_logic_vector(31 downto 0);
		b : in std_logic_vector(31 downto 0);

		r : out std_logic_vector(31 downto 0)
	);
END sub_asr_15;

architecture rtl of sub_asr_15 is

begin
	r <= std_logic_vector(shift_right(signed(a) - signed(b), 15));
end rtl;
