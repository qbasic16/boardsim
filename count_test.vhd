library ieee;
use ieee.std_logic_1164.all;

module count_test(input clk, output reg [17:0] leds);
	initial leds = 0;
	always @(posedge clk) begin
		leds = leds + 1;
	end
endmodule
