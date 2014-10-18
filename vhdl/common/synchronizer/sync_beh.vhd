library ieee;
use ieee.std_logic_1164.all;

architecture beh of sync is
  
  signal sync : std_logic_vector(1 to SYNC_STAGES);
  
begin

	process(sys_clk, sys_res_n)
	begin
	
		if sys_res_n = '0' then
		
			sync <= (others => RESET_VALUE);
			
		elsif rising_edge(sys_clk) then
		
			sync(1) <= data_in;
			
			for i in 2 to SYNC_STAGES loop
				sync(i) <= sync(i-1);
			end loop;
			
		end if;
		
	end process;
	
	data_out <= sync(SYNC_STAGES);
	
end architecture beh;
