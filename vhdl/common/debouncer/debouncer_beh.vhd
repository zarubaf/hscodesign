library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.all;

architecture beh of debouncer is
  
	signal btn_sync : std_logic := RESET_VALUE;
	
	signal latch0     : std_logic := RESET_VALUE;
	
	signal latch1     : std_logic := RESET_VALUE;
	signal latch1_nxt : std_logic := RESET_VALUE;
	
	signal cnt        : integer range 0 to (DEBOUNCE_CYCLES+1) := 0;
	signal cnt_en     : std_logic := '0';
	signal cnt_rst    : std_logic := '0';
		
begin

	btn_syncronizer: entity work.sync
	generic map
	(
		SYNC_STAGES => 2,
		RESET_VALUE => RESET_VALUE
	)
	port map
	(
		sys_clk   => clk,
		sys_res_n => rst_n,
		
		data_in   => btn_in,
		data_out  => btn_sync
	);
	
	ctrl: process(cnt,btn_sync,latch0,latch1)
	begin
	
		if cnt < DEBOUNCE_CYCLES then
			cnt_en <= '1';
			latch1_nxt <= latch1;
		else
			cnt_en <= '0';
			latch1_nxt <= latch0;
		end if;
		
		cnt_rst <= btn_sync xor latch0;
	
	end process;
	
	btn_out <= latch1;
	
	sync: process(clk, rst_n)
	begin
	
		if rising_edge(clk) then
			if rst_n = '0' then
				latch0 <= RESET_VALUE;
				latch1 <= RESET_VALUE;
			else
				latch0 <= btn_sync;
				latch1 <= latch1_nxt;
			end if;
		end if;
	
	end process;
	
	cnt_proc: process(clk,cnt_rst,cnt_en)
	begin
	
		if rising_edge(clk) then
			if cnt_rst = '1' then
				cnt <= 0;
			elsif cnt_en = '1' then
				cnt <= cnt+1;
			end if;
		end if;
	
	end process;
	
	
end architecture beh;
