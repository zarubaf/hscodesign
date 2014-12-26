	component de0_nano_nios2 is
		port (
			clk_clk                         : in    std_logic                     := 'X';             -- clk
			reset_reset_n                   : in    std_logic                     := 'X';             -- reset_n
			sdram_addr                      : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_ba                        : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_cas_n                     : out   std_logic;                                        -- cas_n
			sdram_cke                       : out   std_logic;                                        -- cke
			sdram_cs_n                      : out   std_logic;                                        -- cs_n
			sdram_dq                        : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_dqm                       : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_ras_n                     : out   std_logic;                                        -- ras_n
			sdram_we_n                      : out   std_logic;                                        -- we_n
			audio_config_SDAT               : inout std_logic                     := 'X';             -- SDAT
			audio_config_SCLK               : out   std_logic;                                        -- SCLK
			audio_ext_interface_ADCDAT      : in    std_logic                     := 'X';             -- ADCDAT
			audio_ext_interface_ADCLRCK     : in    std_logic                     := 'X';             -- ADCLRCK
			audio_ext_interface_BCLK        : in    std_logic                     := 'X';             -- BCLK
			audio_ext_interface_DACDAT      : out   std_logic;                                        -- DACDAT
			audio_ext_interface_DACLRCK     : in    std_logic                     := 'X';             -- DACLRCK
			sd_pins_b_SD_cmd                : inout std_logic                     := 'X';             -- b_SD_cmd
			sd_pins_b_SD_dat                : inout std_logic                     := 'X';             -- b_SD_dat
			sd_pins_b_SD_dat3               : inout std_logic                     := 'X';             -- b_SD_dat3
			sd_pins_o_SD_clock              : out   std_logic;                                        -- o_SD_clock
			altpll_phasedone_conduit_export : out   std_logic;                                        -- export
			altpll_locked_conduit_export    : out   std_logic;                                        -- export
			altpll_areset_conduit_export    : in    std_logic                     := 'X';             -- export
			audio_mclk_clk                  : out   std_logic;                                        -- clk
			sdram_clk_clk                   : out   std_logic                                         -- clk
		);
	end component de0_nano_nios2;

	u0 : component de0_nano_nios2
		port map (
			clk_clk                         => CONNECTED_TO_clk_clk,                         --                      clk.clk
			reset_reset_n                   => CONNECTED_TO_reset_reset_n,                   --                    reset.reset_n
			sdram_addr                      => CONNECTED_TO_sdram_addr,                      --                    sdram.addr
			sdram_ba                        => CONNECTED_TO_sdram_ba,                        --                         .ba
			sdram_cas_n                     => CONNECTED_TO_sdram_cas_n,                     --                         .cas_n
			sdram_cke                       => CONNECTED_TO_sdram_cke,                       --                         .cke
			sdram_cs_n                      => CONNECTED_TO_sdram_cs_n,                      --                         .cs_n
			sdram_dq                        => CONNECTED_TO_sdram_dq,                        --                         .dq
			sdram_dqm                       => CONNECTED_TO_sdram_dqm,                       --                         .dqm
			sdram_ras_n                     => CONNECTED_TO_sdram_ras_n,                     --                         .ras_n
			sdram_we_n                      => CONNECTED_TO_sdram_we_n,                      --                         .we_n
			audio_config_SDAT               => CONNECTED_TO_audio_config_SDAT,               --             audio_config.SDAT
			audio_config_SCLK               => CONNECTED_TO_audio_config_SCLK,               --                         .SCLK
			audio_ext_interface_ADCDAT      => CONNECTED_TO_audio_ext_interface_ADCDAT,      --      audio_ext_interface.ADCDAT
			audio_ext_interface_ADCLRCK     => CONNECTED_TO_audio_ext_interface_ADCLRCK,     --                         .ADCLRCK
			audio_ext_interface_BCLK        => CONNECTED_TO_audio_ext_interface_BCLK,        --                         .BCLK
			audio_ext_interface_DACDAT      => CONNECTED_TO_audio_ext_interface_DACDAT,      --                         .DACDAT
			audio_ext_interface_DACLRCK     => CONNECTED_TO_audio_ext_interface_DACLRCK,     --                         .DACLRCK
			sd_pins_b_SD_cmd                => CONNECTED_TO_sd_pins_b_SD_cmd,                --                  sd_pins.b_SD_cmd
			sd_pins_b_SD_dat                => CONNECTED_TO_sd_pins_b_SD_dat,                --                         .b_SD_dat
			sd_pins_b_SD_dat3               => CONNECTED_TO_sd_pins_b_SD_dat3,               --                         .b_SD_dat3
			sd_pins_o_SD_clock              => CONNECTED_TO_sd_pins_o_SD_clock,              --                         .o_SD_clock
			altpll_phasedone_conduit_export => CONNECTED_TO_altpll_phasedone_conduit_export, -- altpll_phasedone_conduit.export
			altpll_locked_conduit_export    => CONNECTED_TO_altpll_locked_conduit_export,    --    altpll_locked_conduit.export
			altpll_areset_conduit_export    => CONNECTED_TO_altpll_areset_conduit_export,    --    altpll_areset_conduit.export
			audio_mclk_clk                  => CONNECTED_TO_audio_mclk_clk,                  --               audio_mclk.clk
			sdram_clk_clk                   => CONNECTED_TO_sdram_clk_clk                    --                sdram_clk.clk
		);

