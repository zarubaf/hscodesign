
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity DE2_115 is

	port
	(
		clk       : in std_logic;
		
		buttons   : in std_logic_vector(1 downto 0);
		leds      : out std_logic_vector(7 downto 0)
	);

end entity;

architecture arch of DE2_115 is

    component de2_115_nios2 is
        port (
				clk_clk       : in  std_logic                    := 'X'; -- clk
				reset_reset_n : in  std_logic                    := 'X'; -- reset_n
				
				inputs_button : in  std_logic                    := 'X'; -- button
				outputs_leds  : out std_logic_vector(7 downto 0)         -- leds
        );
    end component ;

begin

    u0 : component de2_115_nios2
        port map (
            clk_clk       => clk,
            reset_reset_n => buttons(0),
				
				inputs_button => buttons(1),  -- button.button
				outputs_leds  => leds         --   leds.leds
        );

end architecture;





