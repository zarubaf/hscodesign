library IEEE;
use IEEE.std_logic_1164.all;

Entity bshift is   -- barrel shifter
      port (n		: in  std_logic
			left    : in  std_logic; -- '1' for left, '0' for right
            logical : in  std_logic; -- '1' for logical, '0' for arithmetic
            shift   : in  std_logic_vector(4 downto 0);  -- shift count
            input   : in  std_logic_vector (31 downto 0);
            output  : out std_logic_vector (31 downto 0) );
end entity bshift;


architecture behavior of bshift is
  function to_integer(sig : std_logic_vector) return integer is
    variable num : integer := 0;  -- descending sig as integer
  begin
    for i in sig'range loop
      if sig(i)='1' then
        num := num*2+1;
      else
        num := num*2;
      end if;
    end loop;  -- i
    return num;
  end function to_integer;

begin  -- behavior
  shft32: process(left, logical, input, shift)
            variable shft : integer;
            variable out_right_arithmetic : std_logic_vector(31 downto 0);
            variable out_right_logical    : std_logic_vector(31 downto 0);
            variable out_left_logical     : std_logic_vector(31 downto 0);
          begin
            shft := to_integer(shift);
            if logical = '0' then
              out_right_arithmetic := (31 downto 32-shft => input(31)) &
                                      input(31 downto shft);
              output <= out_right_arithmetic after 250 ps;
            else
              if left = '1' then
                out_left_logical := input(31-shft downto 0) &
                                    (shft-1 downto 0 => '0');
                output <= out_left_logical after 250 ps;
              else
                out_right_logical := (31 downto 32-shft => '0') &
                                     input(31 downto shft);
                output <= out_right_logical after 250 ps;
              end if;
            end if;
          end process shft32;
end architecture behavior;  -- of bshift


architecture circuits of bshift is
  signal LR   : std_logic_vector(31 downto 0);
  signal L1s  : std_logic_vector(31 downto 0);
  signal L2s  : std_logic_vector(31 downto 0);
  signal L4s  : std_logic_vector(31 downto 0);
  signal L8s  : std_logic_vector(31 downto 0);
  signal L16s : std_logic_vector(31 downto 0);
  signal L1   : std_logic_vector(31 downto 0);
  signal L2   : std_logic_vector(31 downto 0);
  signal L4   : std_logic_vector(31 downto 0);
  signal L8   : std_logic_vector(31 downto 0);
  signal L16  : std_logic_vector(31 downto 0);
  signal R1s  : std_logic_vector(31 downto 0);
  signal R2s  : std_logic_vector(31 downto 0);
  signal R4s  : std_logic_vector(31 downto 0);
  signal R8s  : std_logic_vector(31 downto 0);
  signal R16s : std_logic_vector(31 downto 0);
  signal R1   : std_logic_vector(31 downto 0);
  signal R2   : std_logic_vector(31 downto 0);
  signal R4   : std_logic_vector(31 downto 0);
  signal R8   : std_logic_vector(31 downto 0);
  signal R16  : std_logic_vector(31 downto 0);
  signal A1s  : std_logic_vector(31 downto 0);
  signal A2s  : std_logic_vector(31 downto 0);
  signal A4s  : std_logic_vector(31 downto 0);
  signal A8s  : std_logic_vector(31 downto 0);
  signal A16s : std_logic_vector(31 downto 0);
  signal A1   : std_logic_vector(31 downto 0);
  signal A2   : std_logic_vector(31 downto 0);
  signal A4   : std_logic_vector(31 downto 0);
  signal A8   : std_logic_vector(31 downto 0);
  signal A16  : std_logic_vector(31 downto 0);
  signal input2s : std_logic_vector(1 downto 0);
  signal input4s : std_logic_vector(3 downto 0);
  signal input8s : std_logic_vector(7 downto 0);
  signal input16s : std_logic_vector(15 downto 0);

  component mux_32
      port(in0    : in  std_logic_vector (31 downto 0);
           in1    : in  std_logic_vector (31 downto 0);
           ctl    : in  std_logic;
           result : out std_logic_vector (31 downto 0));
  end component;
begin  -- circuits
  L1w:  L1s <= input(30 downto 0) & '0'; -- just wiring
  L1m:  mux_32 port map (in0=>input, in1=>L1s, ctl=>shift(0), result=>L1);
  L2w:  L2s <= L1(29 downto 0) & "00"; -- just wiring
  L2m:  mux_32 port map (in0=>L1, in1=>L2s, ctl=>shift(1), result=>L2);
  L4w:  L4s <= L2(27 downto 0) & "0000"; -- just wiring
  L4m:  mux_32 port map (in0=>L2, in1=>L4s, ctl=>shift(2), result=>L4);
  L8w:  L8s <= L4(23 downto 0) & "00000000"; -- just wiring
  L8m:  mux_32 port map (in0=>L4, in1=>L8s, ctl=>shift(3), result=>L8);
  L16w: L16s <= L8(15 downto 0) & "0000000000000000"; -- just wiring
  L16m: mux_32 port map (in0=>L8, in1=>L16s, ctl=>shift(4), result=>L16);
  R1w:  R1s <= '0' & input(31 downto 1); -- just wiring
  R1m:  mux_32 port map (in0=>input, in1=>R1s, ctl=>shift(0), result=>R1);
  R2w:  R2s <= "00" & R1(31 downto 2); -- just wiring
  R2m:  mux_32 port map (in0=>R1, in1=>R2s, ctl=>shift(1), result=>R2);
  R4w:  R4s <= "0000" & R2(31 downto 4); -- just wiring
  R4m:  mux_32 port map (in0=>R2, in1=>R4s, ctl=>shift(2), result=>R4);
  R8w:  R8s <= "00000000" & R4(31 downto 8); -- just wiring
  R8m:  mux_32 port map (in0=>R4, in1=>R8s, ctl=>shift(3), result=>R8);
  R16w: R16s <= "0000000000000000" & R8(31 downto 16); -- just wiring
  R16m: mux_32 port map (in0=>R8, in1=>R16s, ctl=>shift(4), result=>R16);
  A1w:  A1s <= input(31)&input(31 downto 1); -- just wiring
  A1m:  mux_32 port map (in0=>input, in1=>A1s, ctl=>shift(0), result=>A1);
  A2w:  A2s <= input2s&A1(31 downto 2);    -- just wiring
  A2m:  mux_32 port map (in0=>A1, in1=>A2s, ctl=>shift(1), result=>A2);
  A4w:  A4s <= input4s&A2(31 downto 4);    -- just wiring
  A4m:  mux_32 port map (in0=>A2, in1=>A4s, ctl=>shift(2), result=>A4);
  A8w:  A8s <= input8s&A4(31 downto 8);    -- just wiring
  A8m:  mux_32 port map (in0=>A4, in1=>A8s, ctl=>shift(3), result=>A8);
  A16w: A16s <= input16s&A8(31 downto 16); -- just wiring
  A16m: mux_32 port map (in0=>A8, in1=>A16s, ctl=>shift(4), result=>A16);
  AS2:  input2s <= input(31) & input(31);  -- just wiring
  AS4:  input4s <= input2s & input2s;      -- just wiring
  AS8:  input8s <= input4s & input4s;      -- just wiring
  AS16: input16s <= input8s & input8s;     -- just wiring
  OLR:  mux_32 port map (in0=>R16, in1=>L16, ctl=>left, result=>LR);
  LOG:  mux_32 port map (in0=>A16, in1=>LR, ctl=>logical, result=>output);
end architecture circuits;  -- of bshift




