

module spi_dpi(
    output reg spi_clk_o,
    output reg spi_mosi_o,
    output reg spi_cs_o,
    input spi_miso_i,
    input sys_clk
);







// import "DPI-C" context function int spi_dpi( output bit spi_clk,
//                                              output bit spi_mosi,
//                                              output bit spi_cs,
//                                              input  bit spi_miso
// );

import "DPI-C" context function     create_socket_andbind() ;
import "DPI-C" context function     receive();
import "DPI-C" context function     send_to_client();
import "DPI-C" context function     close_server()  ;


initial 
    begin 
        spi_cs_o    =0;
        spi_clk_o   =1;
        spi_mosi_o  =0;
        create_socket_andbind();
    end

always @(posedge sys_clk)
begin
    if(receive()<= 0)
        begin
             $display("Error initializing.");
             $finish;
        end;
    send_to_client();
    
end