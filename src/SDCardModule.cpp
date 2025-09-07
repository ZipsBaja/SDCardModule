#include <ZipsLib.h>
#include <SDCardModule.h>

#include <string.h>
#include <algorithm>

namespace uazips
{

    std::vector<sd_card_t*> SDCardModule::SDCards = {};
    std::vector<spi_t*> SDCardModule::SPIs = {};

    SDCardModule::SDCardModule(const char* sys_name, const PinSetup& pins) : Module(), sys_name(sys_name), pins(pins), current_file_name(""), is_file_open(0)
    {
    }

    SDCardModule::~SDCardModule()
    {
        if (is_file_open)
            Close();

        auto sd_card_end = std::remove(SDCards.begin(), SDCards.end(), m_sd_card);
        auto spi_end = std::remove(SPIs.begin(), SPIs.end(), m_spi);

        SDCards.erase(sd_card_end, SDCards.end());
        SPIs.erase(spi_end, SPIs.end());

        delete m_sd_card;
        delete m_spi;
    }

    bool SDCardModule::Initialize()
    {
        m_sd_card = new sd_card_t;
        m_spi = new spi_t;

        if (!(m_sd_card || m_spi))
        {
            LOG("Out of memory.\n");
            return 0;
        }

        memset(m_sd_card, 0, sizeof(sd_card_t));
        memset(m_spi, 0, sizeof(spi_t));

        m_spi->sck_gpio = pins.sclk;
        m_spi->mosi_gpio = pins.mosi;
        m_spi->miso_gpio = pins.miso;
        m_spi->baud_rate = 12500 * 1000;

        m_sd_card->pcName = sys_name;
        m_sd_card->spi = m_spi;
        m_sd_card->ss_gpio = pins.cs;
        m_sd_card->use_card_detect = 0;
        
        SDCards.push_back(m_sd_card);
        SPIs.push_back(m_spi);

        return 1;
    }

    void SDCardModule::Mount()
    {
        m_result = f_mount(&m_sd_card->fatfs, m_sd_card->pcName, 1);
        if (m_result != FR_OK)
        {
            f_unmount(m_sd_card->pcName);
            THROW("Failed to mount SD Card \"%s\".\n", m_sd_card->pcName);
        }
    }

    void SDCardModule::OpenFile(const char* file_name)
    {
        m_result = f_open(&m_file, file_name, FA_OPEN_APPEND | FA_WRITE | FA_READ);
        if (m_result != FR_OK || m_result != FR_EXIST)
        {
            f_unmount(m_sd_card->pcName);
            THROW("Could not open file \"%s\".\n", file_name);
        }
        current_file_name = file_name;
        is_file_open = true;
    }

    void SDCardModule::WriteString(const char* str)
    {
        f_puts(str, &m_file);
    }

    void SDCardModule::WriteChar(char c)
    {
        f_putc(c, &m_file);
    }

    void SDCardModule::WriteBuff(const void* buff, size_t byte_count)
    {
        size_t bytes_written;
        f_write(&m_file, buff, byte_count, &bytes_written);
        if (bytes_written < byte_count)
        {
            Close();
            LOG("Disk space is full! Closing file \"%s\".\n", current_file_name);
        }
    }

    void SDCardModule::Close()
    {
        m_result = f_close(&m_file);
        if (m_result != FR_OK)
            THROW("Could not close file \"%s\".\n", current_file_name);
        current_file_name = "";
        is_file_open = false;
    }
    
}

size_t sd_get_num()
{
    return uazips::SDCardModule::SDCards.size();
}

sd_card_t* sd_get_by_num(size_t num)
{
    return uazips::SDCardModule::SDCards[num];
}

size_t spi_get_num()
{
    return uazips::SDCardModule::SPIs.size();
}

spi_t* spi_get_by_num(size_t num)
{
    return uazips::SDCardModule::SPIs[num];
}