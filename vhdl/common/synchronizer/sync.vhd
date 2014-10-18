library ieee;
use ieee.std_logic_1164.all;

entity sync is
  generic
  (
    SYNC_STAGES : integer range 2 to integer'high;
    RESET_VALUE : std_logic
  );
  port
  (
    sys_clk : in std_logic;
    sys_res_n : in std_logic;
    data_in : in std_logic;
    data_out : out std_logic
  );
end entity sync;
