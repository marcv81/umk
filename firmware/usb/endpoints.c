// Endpoint configuration register 0 constants
#define ENDPOINT_CFG0_TYPE_CONTROL   0b00000000
#define ENDPOINT_CFG0_TYPE_INTERRUPT 0b11000000
#define ENDPOINT_CFG0_DIRECTION_OUT  0b00000000
#define ENDPOINT_CFG0_DIRECTION_IN   0b00000001

// Endpoint configuration register 1 constants
#define ENDPOINT_CFG1_SIZE_8      0b00000000
#define ENDPOINT_CFG1_SIZE_16     0b00010000
#define ENDPOINT_CFG1_SIZE_32     0b00100000
#define ENDPOINT_CFG1_SIZE_64     0b00110000
#define ENDPOINT_CFG1_SIZE_128    0b01000000
#define ENDPOINT_CFG1_SIZE_256    0b01010000
#define ENDPOINT_CFG1_SIZE_512    0b01100000
#define ENDPOINT_CFG1_BANK_SINGLE 0b00000000
#define ENDPOINT_CFG1_BANK_DOUBLE 0b00000100
#define ENDPOINT_CFG1_ALLOCATE    0b00000010

// Control endpoint configuration
#define ENDPOINT0_SIZE 64
#define ENDPOINT0_CFG0 ( \
    ENDPOINT_CFG0_TYPE_CONTROL | \
    ENDPOINT_CFG0_DIRECTION_OUT)
#define ENDPOINT0_CFG1 ( \
    ENDPOINT_CFG1_SIZE_64 | \
    ENDPOINT_CFG1_BANK_SINGLE | \
    ENDPOINT_CFG1_ALLOCATE)

// Keyboard endpoint configuration
#define ENDPOINT1_SIZE 8
#define ENDPOINT1_CFG0 ( \
    ENDPOINT_CFG0_TYPE_INTERRUPT | \
    ENDPOINT_CFG0_DIRECTION_IN)
#define ENDPOINT1_CFG1 ( \
    ENDPOINT_CFG1_SIZE_8 | \
    ENDPOINT_CFG1_BANK_DOUBLE | \
    ENDPOINT_CFG1_ALLOCATE)

#define MASK_UENUM 0b00000111

// Select endpoint
#define select_endpoint(e) (UENUM = (e & MASK_UENUM))

// Configure endpoint
static void init_endpoint(uint8_t endpoint, uint8_t cfg0, uint8_t cfg1)
{
    select_endpoint(endpoint);
    // Enable endpoint
    set_bit(UECONX, EPEN);
    // Set endpoint configuration registers
    UECFG0X = cfg0;
    UECFG1X = cfg1;
    // Wait until endpoint is configured
    while (!read_bit(UESTA0X, CFGOK));
}
