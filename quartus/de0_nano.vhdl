
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity de0_nano is

	port
	(
		clk       		: in std_logic;
		
		buttons   		: in std_logic_vector(1 downto 0);
		leds      		: out std_logic_vector(7 downto 0);
		SD_CMD	 		: inout std_logic;
		SD_CLK	 		: out std_logic;
		SD_DAT	 		: inout std_logic_vector(3 downto 0);
		AUD_ADCLRCK 	: in std_logic;
		AUD_ADCDAT 		: in std_logic;
		AUD_DACLRCK		: in std_logic;
		AUD_DACDAT		: out std_logic;
		AUD_BCLK			: in std_logic;
		I2C_SCLK			: out std_logic;
		I2C_SDAT			: inout std_logic;
		AUD_XCK			: out std_logic;
		sdram_addr                      	: out   	std_logic_vector(12 downto 0);                    -- addr
		sdram_ba                        	: out   	std_logic_vector(1 downto 0);                     -- ba
		sdram_cas_n                     	: out   	std_logic;                                        -- cas_n
		sdram_cke                       	: out   	std_logic;                                        -- cke
		sdram_cs_n                      	: out   	std_logic;                                        -- cs_n
		sdram_dq                        	: inout 	std_logic_vector(15 downto 0) := (others => 'X'); -- dq
		sdram_dqm                       	: out   	std_logic_vector(1 downto 0);                     -- dqm
		sdram_ras_n                     	: out   	std_logic;                                        -- ras_n
		sdram_we_n                      	: out   	std_logic;                                        -- we_n
		sdram_clk                       	: out   	std_logic                                         -- clk
	);

end entity;

architecture arch of de0_nano is

    component de0_nano_nios2 is
        port (
				clk_clk                         	: in    	std_logic                     := 'X';             -- clk
            reset_reset_n                   	: in    	std_logic                     := 'X';             -- reset_n
            altpll_phasedone_conduit_export 	: out   	std_logic;                                        -- export
            altpll_locked_conduit_export    	: out   	std_logic;                                        -- export
            altpll_areset_conduit_export    	: in    	std_logic                     := 'X';             -- export
            sdram_addr                      	: out   	std_logic_vector(12 downto 0);                    -- addr
            sdram_ba                        	: out   	std_logic_vector(1 downto 0);                     -- ba
            sdram_cas_n                     	: out   	std_logic;                                        -- cas_n
            sdram_cke                       	: out   	std_logic;                                        -- cke
            sdram_cs_n                      	: out   	std_logic;                                        -- cs_n
            sdram_dq                        	: inout 	std_logic_vector(15 downto 0) := (others => 'X'); -- dq
            sdram_dqm                       	: out   	std_logic_vector(1 downto 0);                     -- dqm
            sdram_ras_n                     	: out   	std_logic;                                        -- ras_n
            sdram_we_n                      	: out   	std_logic;                                        -- we_n
            sdram_clk_clk                   	: out   	std_logic;                                        -- clk
				audio_mclk_clk 						: out 	std_logic;
				audio_config_SDAT 					: inout 	std_logic;
				audio_config_SCLK 					: out 	std_logic;				
				sd_pins_b_SD_cmd 						: inout std_logic;
				sd_pins_b_SD_dat 						: inout std_logic;
				sd_pins_b_SD_dat3 					: inout std_logic;
				sd_pins_o_SD_clock 					: out std_logic;
				audio_ext_interface_ADCDAT 		: in 		std_logic;
				audio_ext_interface_ADCLRCK 		: in 		std_logic;
				audio_ext_interface_BCLK 			: in 		std_logic;
				audio_ext_interface_DACDAT 		: out 	std_logic;
				audio_ext_interface_DACLRCK 		: in 		std_logic
        );
		  
    end component ;

begin

    u0 : component de0_nano_nios2
        port map (
            clk_clk       => clk,
            reset_reset_n => buttons(0),
			altpll_phasedone_conduit_export 	=> open, -- altpll_phasedone_conduit.export
            altpll_locked_conduit_export    	=> open,    --    altpll_locked_conduit.export
            altpll_areset_conduit_export    	=> '0',     --    altpll_areset_conduit.export	
--				inputs_button => buttons(1),  -- button.button
--				outputs_leds  => leds,         --   leds.leds
				
			-- spi
			sd_pins_o_SD_clock => SD_CLK,
			sd_pins_b_SD_dat => SD_DAT(0),
			sd_pins_b_SD_dat3 => SD_DAT(3),
			sd_pins_b_SD_cmd => SD_CMD,
			-- audio
			audio_ext_interface_ADCDAT  =>  AUD_ADCDAT,
			audio_ext_interface_ADCLRCK	=> AUD_ADCLRCK,
			audio_ext_interface_BCLK	=> AUD_BCLK,
		    audio_ext_interface_DACDAT => AUD_DACDAT, 
			audio_ext_interface_DACLRCK => AUD_DACLRCK,
			-- audio config
			audio_config_SCLK =>	I2C_SCLK,
			audio_config_SDAT => I2C_SDAT,
			audio_mclk_clk		=> AUD_XCK,
			sdram_addr                      	=> sdram_addr,                      --                    sdram.addr
            sdram_ba                        	=> sdram_ba,                        --                         .ba
            sdram_cas_n                     	=> sdram_cas_n,                     --                         .cas_n
            sdram_cke                       	=> sdram_cke,                       --                         .cke
            sdram_cs_n                      	=> sdram_cs_n,                      --                         .cs_n
            sdram_dq                        	=> sdram_dq,                        --                         .dq
            sdram_dqm                       	=> sdram_dqm,                       --                         .dqm
            sdram_ras_n                     	=> sdram_ras_n,                     --                         .ras_n
            sdram_we_n                      	=> sdram_we_n,                      --                         .we_n
            sdram_clk_clk                   	=> sdram_clk
        );

end architecture;




