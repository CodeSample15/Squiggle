#include <string>

#include "frontend.hpp"
#include "screen.hpp"

using namespace Frontend;

void throwFrontendError(std::string message);

#if BUILD_FOR_RASPI
    //arduino port for frontend
    #include <wiringPi.h>
    #include "../lib/ST7735_TFT_RPI/include/ST7735_TFT.hpp"

    //code taken from Display_Lib_RPI GitHub: https://github.com/gavinlyonsrepo/Display_Lib_RPI/blob/main/examples/st7735/Hello_world_SWSPI/main.cpp
    ST7735_TFT myTFT;

    // GPIO
    int8_t RST_TFT  = 25;
    int8_t DC_TFT   = 24;
    int8_t SCLK_TFT = 6;
    int8_t SDIN_TFT = 5;
    int8_t CS_TFT   = 21;
    int  GPIO_CHIP_DEV = 4; // RPI 5 = 4 , other RPIs = 0

    uint8_t OFFSET_COL = 0;  // 2, These offsets can be adjusted for any issues->
    uint8_t OFFSET_ROW = 0; // 3, with manufacture tolerance/defects at edge of display
    uint16_t TFT_WIDTH = SCREEN_WIDTH;// Screen width in pixels (128)
    uint16_t TFT_HEIGHT = SCREEN_HEIGHT; // Screen height in pixels (160)

    int SPI_DEV = 0; // A SPI device, >= 0. which SPI interface to use
    int SPI_CHANNEL = 0; // A SPI channel, >= 0. Which Chip enable pin to use
    int SPI_SPEED = 8000000; // The speed of serial communication in bits per second.
    int SPI_FLAGS = 0;

    uint16_t SWSPI_CommDelay = 0; //uS GPIO SW SPI delay

    uint8_t SetupSWSPI(void); // setup + user options for software SPI

    Screen lastScreen;

    void Frontend::init() {
        if(SetupSWSPI()!=0)
            throwFrontendError("Unable to initialize SPI screen!");

        myTFT.fillScreen(RDLC_BLACK);
    }

    void Frontend::cleanUp() {
        myTFT.fillScreen(RDLC_BLACK);
        myTFT.TFTPowerDown();
    }

    void Frontend::drawScreen() {
        std::vector<screen_loc> changedPixels = screen.changedPixels(lastScreen);

        uint16_t color;
        for(screen_loc& loc : changedPixels) {
            color = myTFT.Color565(((int16_t)screen.screenBuff[loc.x][loc.y][0])<<8, ((int16_t)screen.screenBuff[loc.x][loc.y][1])<<8, ((int16_t)screen.screenBuff[loc.x][loc.y][2])<<8);
            myTFT.drawPixel(loc.x, loc.y, color);
        }

        lastScreen = screen;
    }

    float Frontend::getHorAxis() {
        return 0.0;
    }

    float Frontend::getVertAxis() {
        return 0.0;
    }

    bool Frontend::getABtn() {
        return false;
    }

    bool Frontend::getBBtn() {
        return false;
    }

    bool Frontend::getExitBtn() {
        return false;
    }

    uint8_t SetupSWSPI(void)
    {
        // ** USER OPTION 1 GPIO/SPI TYPE SW **
        myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
        //********************************************
        // ** USER OPTION 2 Screen Setup **
        myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW , TFT_WIDTH , TFT_HEIGHT);
        // ***********************************
        // ** USER OPTION 3 PCB_TYPE **
        // pass enum to param1 ,4 choices,see README
        if(myTFT.TFTInitPCBType(myTFT.TFT_ST7735R_Red, SPI_DEV, SPI_CHANNEL, SPI_SPEED, SPI_FLAGS, GPIO_CHIP_DEV) != rpiDisplay_Success)
        {
            return 3;
        }
        //*****************************
        return 0;
    }
#else
    //PC port using opencv
    #include <opencv2/opencv.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
    #include <opencv2/core/utils/logger.hpp>

    void Frontend::init() {}
    void Frontend::cleanUp() {}

    void Frontend::drawScreen() {
        //convert virtual screen to opencv Mat
        cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

        cv::Mat display(SCREEN_HEIGHT, SCREEN_WIDTH, CV_8UC3, screen.screenBuff);
        cv::cvtColor(display, display, cv::COLOR_RGB2BGR);
        
        //draw Mat in window
        cv::imshow("Squiggly Project", display);
        cv::waitKey(SCREEN_REFRESH_DELAY);
    }

    //getting keboard input
    #if _WIN32
        #include <windows.h>

        float Frontend::getHorAxis() {
            if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
                return 1.0;
            else if(GetAsyncKeyState(VK_LEFT) & 0x8000)
                return -1.0;
            else
                return 0.0;
        }

        float Frontend::getVertAxis() {
            if(GetAsyncKeyState(VK_UP) & 0x8000)
                return 1.0;
            else if(GetAsyncKeyState(VK_DOWN) & 0x8000)
                return -1.0;
            else
                return 0.0;
        }

        bool Frontend::getABtn() {
            return GetAsyncKeyState(WIN_A_BTN_CODE) & 0x8000;
        }

        bool Frontend::getBBtn() {
            return GetAsyncKeyState(WIN_B_BTN_CODE) & 0x8000;
        }

        bool Frontend::getExitBtn() {
            return GetAsyncKeyState(VK_ESCAPE) & 0x8000;
        }
    #else //todo: implement keyboard input for mac and linux
        float Frontend::getHorAxis() {
            return 0.0;
        }

        float Frontend::getVertAxis() {
            return 0.0;
        }

        bool Frontend::getABtn() {
            return false;
        }

        bool Frontend::getBBtn() {
            return false;
        }

        bool Frontend::getExitBtn() {
            return false;
        }
    #endif
#endif

void throwFrontendError(std::string message) {
    throw std::runtime_error("Program frontend failed! : " + message);
}