#define USER_SETUP_INFO "User_Setup"

// ##################################################################################
//
// Section 1. Call up the right driver file and any options for it
//
// ##################################################################################

#define ST7789_DRIVER      // Full configuration option, define additional parameters below for this display
#define TFT_WIDTH  320
#define TFT_HEIGHT 240


// ##################################################################################
//
// Section 2. Define the pins that are used to interface with the display here
//
// ##################################################################################

#define TFT_SCLK 5
#define TFT_MOSI 19
#define TFT_CS   32  // Chip select control pin
#define TFT_DC   33  // Data Command control pin
#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
//#define TFT_BL   45


// ##################################################################################
//
// Section 3. Define the fonts that are to be used here
//
// ##################################################################################

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

// Comment out the #define below to stop the SPIFFS filing system and smooth font code being loaded
// this will save ~20kbytes of FLASH
//#define SMOOTH_FONT


// ##################################################################################
//
// Section 4. Other options
//
// ##################################################################################

// For ST7735, ST7789 and ILI9341 ONLY, define the colour order IF the blue and red are swapped on your display
// Try ONE option at a time to find the correct colour order for your display

#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

#define SPI_FREQUENCY  27000000

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  20000000

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY  2500000
