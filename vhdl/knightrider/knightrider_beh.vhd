library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

architecture beh of knightrider is
	
	type REG_TYPE is record
		leds     : std_logic_vector(LED_NUM-1 downto 0);
		
		dir      : std_logic;
		step_cnt : integer;
		
	end record;
	
	constant REG_RST : REG_TYPE := ((LED_NUM-1 => '1', others => '0'),'1',0);
	
	signal reg     : REG_TYPE := REG_RST;
	signal reg_nxt : REG_TYPE := REG_RST;
  
begin

	nxt: process(reg,en)
		variable v_shift : std_logic := '0';
	begin
	
		reg_nxt <= reg;
		v_shift := '0';
		
		if reg.step_cnt = T_STEP then
			reg_nxt.step_cnt <= 0;
			v_shift := '1';
		elsif en = '1' then
			reg_nxt.step_cnt <= reg.step_cnt+1;
		end if;
		
		if v_shift = '1' then
			if reg.dir = '1' then
				reg_nxt.leds <= '0'&reg.leds(LED_NUM-1 downto 1);
			else
				reg_nxt.leds <= reg.leds(LED_NUM-2 downto 0)&'0';
			end if;
			
			if reg.leds(1) = '1' then
				reg_nxt.dir <= '0';
			end if;
			
			if reg.leds(LED_NUM-2) = '1' then
				reg_nxt.dir <= '1';
			end if;
			
		end if;
		
	end process;
	
	leds <= reg.leds;

	sync: process(clk,rst_n)
	begin
	
		if rising_edge(clk) then
			if rst_n = '0' then
				reg <= REG_RST;
			else
				reg <= reg_nxt;
			end if;
		end if;
	
	end process;
	
end architecture beh;
