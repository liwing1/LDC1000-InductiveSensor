#define SS 10
#define RD 0x80

#define DVID_REG 0x00
#define RMAX_REG 0x01
#define RMIN_REG 0x02

#define WDTF_REG 0x03
#define LDCC_REG 0x04
#define CLKC_REG 0x05

#define CTHL_REG 0x06
#define CTHM_REG 0x07
#define CTLL_REG 0x08
#define CTLM_REG 0x09

#define INTB_REG 0x0A
#define POWR_REG 0x0B
#define STTS_REG 0x20

#define PRXL_REG 0x21
#define PRXM_REG 0x22

#define FQCL_REG 0x23
#define FQCB_REG 0x24
#define FQCM_REG 0x25


void ldcConfig( void );
void clkConfig( void );
uint32_t readRegister(uint8_t add, uint8_t nBytes);
void writeRegister(uint8_t addr, uint8_t data);
float medeProx( void );
