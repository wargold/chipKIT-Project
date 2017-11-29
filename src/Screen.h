extern const uint8_t const font[128 * 8];
void spi_sendData(uint8_t byte);
void delay(int ms);
void display_Update(uint8_t *displayMem, uint8_t startPage, uint8_t endPage);
void charToDisplayValues(char *str, int strLen, uint8_t *val, int startIndx);
void init_disp( void );
