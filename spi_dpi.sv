

module spi_dpi(
    output reg spi_clk_o,
    output reg spi_mosi_o,
    output reg spi_cs_o,
    input spi_miso_i
    input sys_clk
);







import "DPI-C" context function int spi_dpi( output bit spi_clk,
                                             output bit spi_mosi,
                                             output bit spi_cs,
                                             input  bit spi_miso
);




initial 
    begin 
        spi_cs_o    =0;
        spi_clk_o   =1;
        spi_mosi_o  =0;
    end

always @(posedge sys_clk)
begin
    spi_dpi(spi_clk_o,spi_mosi_o,spi_cs_o,spi_miso);
end