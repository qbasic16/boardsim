library ieee;
use ieee.std_logic_1164.all;

entity DE2_top is
	port(
		switches	: in	std_logic_vector(17 downto 0);
		buttons		: in	std_logic_vector(3 downto 0);
		leds		: out	std_logic_vector(17 downto 0)
	);
end entity DE2_top;

architecture main of DE2_top is

	attribute foreign of main : architecture is “app_init app.sl”;
	signal clk: std_logic;

begin

	sw : process(switches, buttons)
	begin
			$DE2_handle_input;
			switches = $DE2_switches;
			buttons = $DE2_buttons;
			$DE2_leds(leds);
			$DE2_render;
	end process sw;

	clkgen : process
	begin

		clk <= '0';
		wait for clock_period/2;
		clk <= '1';
		wait for clock_period/2;

	end process clkgen;

end architecture main;
