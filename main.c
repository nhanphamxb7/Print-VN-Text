#include "stdio.h"
#include "usbstk5515.h"
#include "usbstk5515_i2c.h"
#include "usbstk5515_gpio.h"
#include "lcd.h"

#define OSD9616_I2C_ADDR 0x3C

Int16 i;
Uint8 cmd[10];

Int16 OSD9616_send(Uint16 comdat, Uint16 data);
Int16 OSD9616_multiSend(Uint8* data, Uint16 len);
Int16 printLetter(Uint16 l1, Uint16 l2, Uint16 l3, Uint16 l4);
void init_oled();
void write_page(int page);

void main( void )
{
	init_oled();  
	
	/* Set vertical and horizontal scrolling */
    cmd[0] = 0x00;
    cmd[1] = 0x29;  // Vertical and Right Horizontal Scroll
    cmd[2] = 0x00;  // Dummy byte
    cmd[3] = 0x00;  // Define start page address
    cmd[4] = 0x01;  // Set time interval between each scroll step
    cmd[5] = 0x01;  // Define end page address
    cmd[6] = 0x01;  // Vertical scrolling offset
    OSD9616_multiSend( cmd, 7 );
    OSD9616_send(0x00,0x2f);
    
    write_page(0);
    
	for(i = 0; i < 5; i++) OSD9616_send(0x40,0x00);  // Spaces
    printLetter(48,  8,  8, 56);  // n
    printLetter(32, 50, 41, 58);  // aa
	printLetter(48,  8,  8, 62);  // h
    printLetter(62,  8,	 4,	62);  // N
    
    for(i = 0; i < 5; i++) OSD9616_send(0x40,0x00);  // Spaces
    printLetter(48,  8,  8, 62);  // h
    printLetter(48,  8,  8, 56);  // n
    printLetter(32, 48, 40, 56);  // a
	printLetter(48,  8,  8, 62);  // h
    printLetter(2, 62, 2, 0);     // T
    
    for(i = 0; i < 5; i++) OSD9616_send(0x40,0x00);  // Spaces
    printLetter(56, 56, 8, 56);   // m
    printLetter(32, 48, 168, 56); // aj
	printLetter(48,  8,  8, 62);  // h
    printLetter(0, 14, 10, 62);  // P
    
    write_page(1);
    
	for(i = 0; i < 5; i++) OSD9616_send(0x40,0x00);  // Spaces
    printLetter(48,  8,  8, 56);  // n
    printLetter(32, 50, 41, 58);  // aa
	printLetter(48,  8,  8, 62);  // h
    printLetter(62,  8,	 4,	62);  // N
    
    for(i = 0; i < 5; i++) OSD9616_send(0x40,0x00);  // Spaces
    printLetter(48,  8,  8, 62);  // h
    printLetter(48,  8,  8, 56);  // n
    printLetter(32, 48, 40, 56);  // a
	printLetter(48,  8,  8, 62);  // h
    printLetter(2, 62, 2, 0);     // T
    
    for(i = 0; i < 5; i++) OSD9616_send(0x40,0x00);  // Spaces
    printLetter(56, 56, 8, 56);   // m
    printLetter(32, 48, 168, 56); // aj
	printLetter(48,  8,  8, 62);  // h
    printLetter(0, 14, 10, 62);  // P
    
    
    
    
    
    /* Keep first 8 rows from vertical scrolling  
    cmd[0] = 0x00;
    cmd[1] = 0xa3;  // Set Vertical Scroll Area
    cmd[2] = 0x010;  // Set No. of rows in top fixed area
    cmd[3] = 0x08;  // Set No. of rows in scroll area
    OSD9616_multiSend( cmd, 4 );*/
    
	SW_BREAKPOINT;
}

Int16 OSD9616_send(Uint16 comdat, Uint16 data)
{
	Uint8 cmd[2];
    cmd[0] = comdat & 0x00FF;
    cmd[1] = data;
    return USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 2 );
}

Int16 OSD9616_multiSend(Uint8* data, Uint16 len)
{
	Uint16 x;
    Uint8 cmd[10];
    for(x = 0; x < len; x++) 
    	cmd[x] = data[x];
    return USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, len );
}

Int16 printLetter(Uint16 l1, Uint16 l2, Uint16 l3, Uint16 l4)
{
	OSD9616_send(0x40,l1);
    OSD9616_send(0x40,l2);
    OSD9616_send(0x40,l3);
    OSD9616_send(0x40,l4);
    OSD9616_send(0x40,0x00);
    return 0;
}

void init_oled()
{
	USBSTK5515_init();
	/* Initialize I2C */
    USBSTK5515_I2C_init( );
    
    /* Initialize LCD power */
    USBSTK5515_GPIO_setDirection( 12, 1 );  // Output
    USBSTK5515_GPIO_setOutput( 12, 1 );     // Enable 13V 
    
    /* Initialize OSD9616 display */
    OSD9616_send(0x00,0x00); // Set low column address
    OSD9616_send(0x00,0x10); // Set high column address
    OSD9616_send(0x00,0x40); // Set start line address

    cmd[0] = 0x00 & 0x00FF;  // Set contrast control register
    cmd[1] = 0x81;
    cmd[2] = 0x7f;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xa1); // Set segment re-map 95 to 0
    OSD9616_send(0x00,0xa6); // Set normal display

    cmd[0] = 0x00 & 0x00FF;  // Set multiplex ratio(1 to 16)
    cmd[1] = 0xa8; 
    cmd[2] = 0x0f;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xd3); // Set display offset
    OSD9616_send(0x00,0x00); // Not offset
    OSD9616_send(0x00,0xd5); // Set display clock divide ratio/oscillator frequency
    OSD9616_send(0x00,0xf0); // Set divide ratio

    cmd[0] = 0x00 & 0x00FF;  // Set pre-charge period
    cmd[1] = 0xd9;
    cmd[2] = 0x22;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    cmd[0] = 0x00 & 0x00FF;  // Set com pins hardware configuration
    cmd[1] = 0xda;
    cmd[2] = 0x02;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xdb); // Set vcomh
    OSD9616_send(0x00,0x49); // 0.83*vref
    
    cmd[0] = 0x00 & 0x00FF;  //--set DC-DC enable
    cmd[1] = 0x8d;
    cmd[2] = 0x14;
    USBSTK5515_I2C_write( OSD9616_I2C_ADDR, cmd, 3 );

    OSD9616_send(0x00,0xaf); // Turn on oled panel
}

void write_page(int page)
{
	/* Fill page 0 */ 
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+page); // Set page for page 0 to page 5
    for(i=0;i<128;i++) OSD9616_send(0x40,0xff);

    /* Write to page 0 */
    OSD9616_send(0x00,0x00);   // Set low column address
    OSD9616_send(0x00,0x10);   // Set high column address
    OSD9616_send(0x00,0xb0+page); // Set page for page 0 to page 5	
}
