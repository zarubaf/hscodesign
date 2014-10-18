library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library work;
use work.all;

entity avalon_demo is
	generic
	(
		DATA_WIDTH  : integer := 32;
		LED_NUM     : integer := 8;
		
		T_STEP   : integer := (100 ms)/(20 ns)
	);
	port
	(
		clk			: in  std_logic;
		reset_n		: in  std_logic;
		
		address 	: in  std_logic_vector(3 downto 0);

		write_n		: in  std_logic;
		writedata   : in  std_logic_vector(DATA_WIDTH-1 downto 0);
		
		button      : in  std_logic;
		
		irq			: out std_logic;
		readdata	: out std_logic_vector(DATA_WIDTH-1 downto 0);
		
		leds        : out std_logic_vector(LED_NUM-1 downto 0)
	);
end avalon_demo;

architecture behaviour of avalon_demo is
	
	type REG_TYPE is record
		ctrl   : std_logic_vector(DATA_WIDTH-1 downto 0); 
		status : std_logic_vector(DATA_WIDTH-1 downto 0);
	end record;
	
	constant REG_RST : REG_TYPE := (others => (others => '0'));
	
	signal r,r_nxt   : REG_TYPE := REG_RST; 
	
	signal btn_deb     : std_logic := '1';
	signal btn_deb_buf : std_logic := '1';

begin

	kitt: entity work.knightrider
	generic map
	(
		LED_NUM => LED_NUM,
		T_STEP  => T_STEP
	)
	port map
	(
		clk    => clk,
		rst_n  => reset_n,

		en     => r.ctrl(0), -- bit 0 of control register enables knightrider
		leds   => leds
	);

	btn_debouncer: entity work.debouncer
	generic map
	(
		RESET_VALUE     => '1',
		DEBOUNCE_CYCLES => 50
	)
	port map
	(
		clk     => clk,
		rst_n   => reset_n,

		btn_in  => button,
		btn_out => btn_deb
	);
	
	reg_proc: process(r,write_n,address,writedata,btn_deb_buf,btn_deb)
		variable v_set_irq : std_logic := '0';
	begin
	
		r_nxt <= r;
		
		-- set interrupt 		
		v_set_irq := btn_deb_buf and not(btn_deb);
		if v_set_irq = '1' then
			r_nxt.status(0) <= '1'; -- interrupt status flag is assigned to status register bit 0
		end if;
		
		readdata <= (others => '0');
		
		if address = "0000" then
		
			readdata <= r.ctrl;
			
			if write_n = '0' then
				r_nxt.ctrl <= writedata;
			end if;
		
		elsif address = "0001" then
		
			readdata <= r.status;
			
			if write_n = '0' then
				for i in DATA_WIDTH-1 downto 0 loop
					if writedata(i) = '1' then
						r_nxt.status(i) <= '0';
					end if;
				end loop;
			end if;
		
		end if;
	
	end process;
	
	irq <= r.status(0);
	
	syn: process(clk,reset_n)
	begin
	
		if rising_edge(clk) then
			if reset_n = '0' then
				btn_deb_buf <= '1';
				r           <= REG_RST;
			else
				btn_deb_buf <= btn_deb;
				r           <= r_nxt;
			end if;
		end if;
	
	end process;

end behaviour;
