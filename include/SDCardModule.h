#pragma once

#include <Module.h> // Assuming ZipsPicoLib is included in project.

#include <hw_config.h>

namespace uazips
{

    class SDCardModule : public Module
    {
    public:
        static std::vector<sd_card_t*> SDCards;
        static std::vector<spi_t*> SPIs;

        // MISO = RX, MOSI = TX
        struct PinSetup
        {
            uint8_t sclk; // Serial clock
            uint8_t mosi; // Serial data input
            uint8_t miso; // Serial data output
            uint8_t cs;   // Slave select
        };

    private:
        sd_card_t* m_sd_card;
        spi_t* m_spi;
        const char* sys_name;
        PinSetup pins;
        FRESULT m_result;
        FIL m_file;
        const char* current_file_name;
        bool is_file_open;

    public:
        SDCardModule(const char* sys_name, const PinSetup& pins);
        virtual ~SDCardModule();

        virtual bool Initialize() override;

        void Mount();
        void OpenFile(const char* file_name);
        void WriteString(const char* str);
        void WriteChar(char c);
        void WriteBuff(const void* buff, size_t byte_count);
        void Close();
    };

}