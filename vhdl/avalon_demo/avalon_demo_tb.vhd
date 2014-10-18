library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity avalon_demo_tb is
end avalon_demo_tb;

architecture behaviour of avalon_demo_tb is

	constant  cc		: TIME := 20 ns; 

	signal clk		: std_logic := '0';
	signal reset_n	: std_logic := '1';
	signal address	: std_logic_vector(3 downto 0) := (others => '0');
	signal write_n	: std_logic := '1';
	signal writedata: std_logic_vector(31 downto 0) := (others => '0');
	signal button	: std_logic := '1';

	signal irq		: std_logic;
	signal readdata	: std_logic_vector (31 downto 0);

	signal leds		: std_logic_vector (7 downto 0);
  
begin

	uut: entity work.avalon_demo
		generic map
		(
			T_STEP   => 5
		)
		port map 
		(
			clk       => clk,
			reset_n   => reset_n,
			address   => address,
			write_n   => write_n,
			writedata => writedata,
			button    => button,
			irq       => irq,
			readdata  => readdata,
			leds      => leds
		);

	clkgen : process
	begin
		clk <= '1';
		wait for cc/2;
		clk <= '0'; 
		wait for cc/2;
	end process clkgen;

	test: process
		procedure icwait(cycles: Natural) is
		begin 
			for i in 1 to cycles loop 
				wait until clk= '0' and clk'event;
			end loop;
		end ;

	begin
	
		report "starting simulation";
		reset_n <= '0';
		icwait(10);
		reset_n <= '1';
		write_n <= '1';
		icwait(5);

		icwait(3);
	
		for i in 0 to 10 loop
			button <= '1';
			icwait(3);
			button <= '0';
			icwait(3);
		end loop;
		
		wait until irq = '1';
		
		icwait(5);
		
		address   <= "0001";
		writedata <= x"00000001";
		write_n   <= '0';
		icwait(1);
		write_n   <= '1';
		
		icwait(1);
		
		address   <= "0000";
		writedata <= x"00000001";
		write_n   <= '0';
		icwait(1);
		write_n   <= '1';
	
		icwait(120);
	
		for i in 0 to 10 loop
			button <= '0';
			icwait(3);
			button <= '1';
			icwait(3);
		end loop;

		report "simulation done";

		wait;
	end process test;

end behaviour; 

