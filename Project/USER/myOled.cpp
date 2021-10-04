#include "myOled.h"
#define OLED_DC(x)   gpio_set (OLED_DC_PIN	, x)
#define OLED_RST(x)  gpio_set (OLED_RST_PIN	, x)
#define OLED_CS(x)   gpio_set (OLED_CS_PIN  , x)
uint32 oledBund;
uint8 recData;
void MyOLED_Init(void)
{
    gpio_init (OLED_RST_PIN, GPO, 1);
	gpio_init (OLED_CS_PIN, GPO, 1);
    gpio_init (OLED_DC_PIN, GPO, 1);
    port_init_NoAlt (OLED_RST_PIN, PULLUP );
    port_init_NoAlt (OLED_DC_PIN, PULLUP );
	port_init_NoAlt (OLED_CS_PIN, PULLUP );
	spi_init(spi0, SPI_CS_NOT_USE, MASTER, 10 * 1000 * 1000); //硬件SPI初始化
	systick_delay_ms(20);
	OLED_RST(0);
	systick_delay_ms(50);
	OLED_RST(1);
    MyOLED_WrCmd(0xae);//--turn off oled panel
    MyOLED_WrCmd(0x00);//---set low column address
    MyOLED_WrCmd(0x10);//---set high column address
    MyOLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    MyOLED_WrCmd(0x81);//--set contrast control register
    MyOLED_WrCmd(0xcf); // Set SEG Output Current Brightness
    MyOLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    MyOLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    MyOLED_WrCmd(0xa6);//--set normal display
    MyOLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
    MyOLED_WrCmd(0x3f);//--1/64 duty
    MyOLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    MyOLED_WrCmd(0x00);//-not offset
    MyOLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
    MyOLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    MyOLED_WrCmd(0xd9);//--set pre-charge period
    MyOLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    MyOLED_WrCmd(0xda);//--set com pins hardware configuration
    MyOLED_WrCmd(0x12);
    MyOLED_WrCmd(0xdb);//--set vcomh
    MyOLED_WrCmd(0x40);//Set VCOM Deselect Level
    MyOLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    MyOLED_WrCmd(0x02);//
    MyOLED_WrCmd(0x8d);//--set Charge Pump enable/disable
    MyOLED_WrCmd(0x14);//--set(0x10) disable
    MyOLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    MyOLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
    MyOLED_WrCmd(0xaf);//--turn on oled panel
	MyOLED_Fill(0x00);  //初始清屏
    MyOLED_Set_Pos(0, 0);
}

void MyOLED_WrCmd(uint8 cmd)
{
  	OLED_DC(0);
	OLED_CS(0);
	OLED_RST(1);
  	cmd |= 0x00;
  	spi_mosi(spi0,SPI_PCS2,&cmd,NULL,1);
	OLED_CS(1);
}

void MyOLED_WrDat(uint8 data)
{
  	OLED_CS(0);
  	OLED_DC(1);
	OLED_RST(1);
  	spi_mosi(spi0,SPI_PCS2,&data,NULL,1);
	OLED_CS(1);
}

void MyOLED_Set_Pos(uint8 x, uint8 y)
{
    MyOLED_WrCmd(0xb0 + y);
    MyOLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
    MyOLED_WrCmd((x & 0x0f) | 0x00);
}

void MyOLED_Fill(uint8 bmp_data)
{
    uint8 y, x;
    for(y = 0; y < 8; y++)
    {
        MyOLED_WrCmd(0xb0 + y);
        MyOLED_WrCmd(0x01);
        MyOLED_WrCmd(0x10);
        for(x = 0; x < 128; x++)	MyOLED_WrDat(bmp_data);
    }
}

void MyOLED_P6x8Str(uint8 x, uint8 y, uint8 ch[])
{
    uint8 c = 0, i = 0, j = 0;
    while (ch[j] != '\0')
    {
        c = ch[j] - 32;
        if(x > 126)
        {
            x = 0;
            y++;
        }
        MyOLED_Set_Pos(x, y);
        for(i = 0; i < 6; i++)	MyOLED_WrDat(F6x8[c][i]);
        x += 6;
        j++;
    }
}

void MyOLED_PrintFloatValue(unsigned char x, unsigned char y, float data)
{
	int temp;

	temp = (int)data;
	MyOLED_Put6x8Char(x, y, ((temp % 1000) / 100) + 48);
	MyOLED_Put6x8Char(x + 6, y, ((temp % 100) / 10) + 48);
	MyOLED_Put6x8Char(x + 12, y, (temp % 10) + 48);

	MyOLED_Put6x8Char(x + 18, y, '.');

	data = data * 100000 + 1;
	temp = (int)data;
	temp %= 100000;
	MyOLED_Put6x8Char(x + 24, y, (temp / 10000) + 48);
	MyOLED_Put6x8Char(x + 30, y, ((temp % 10000) / 1000) + 48);
	MyOLED_Put6x8Char(x + 36, y, ((temp % 1000) / 100) + 48);
}

void MyOLED_Print_Num1(uint8 x, uint8 y, int16 num)
{
    uint8_t *ch1, ch[7];
    if(num < 0)
    {
        num = -num;
        MyOLED_P6x8Str(x, y, (uint8 *)"-");
    }
    else         MyOLED_P6x8Str(x, y, (uint8 *)" ");
    x += 6;
    MyOLED_HEXACSII(num, ch);
    ch1 = &ch[1];
    while(*ch1 != '\0')
    {
        MyOLED_P6x8Str(x, y, ch1);	//显示数字
        x += 6;
        ch1++;
    }
}
void MyOLED_HEXACSII(uint16 hex, uint8 *Print)
{
    uint8 hexcheck ;
    uint8 TEMP ;
    TEMP = 6 ;
    Print[TEMP ] = '\0';
    while(TEMP)
    {
        TEMP -- ;
        hexcheck  =  hex % 10 ;
        hex   /= 10 ;
        Print[TEMP ]  = hexcheck + 0x30 ;
    }
}

void MyOLED_Put6x8Char(unsigned char x, unsigned char y, unsigned char ch)
{
	unsigned char c = 0, i = 0;

	c = ch - 32;

	if(x > 122)
	{
		x = 0;
		y++;
	}
	MyOLED_Set_Pos(x, y);

	for(i = 0 ; i < 6 ; i++)
		MyOLED_WrDat(F6x8[c][i]);
}
