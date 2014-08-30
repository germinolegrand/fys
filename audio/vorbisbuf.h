#pragma once

#include <streambuf>
#include <vector>
#include <memory>

#include "stb_vorbis.h"


class vorbisbuf: public std::streambuf
{
public:
    class channelbuf: public std::streambuf
    {
    private:
        int_type underflow() override;
        std::streamsize xsgetn(char_type* s, std::streamsize count) override;

    private: friend class vorbisbuf;
        channelbuf(vorbisbuf& src);
        void push_data(char_type const* data_begin, size_t data_size);

        std::vector<char_type> m_decoded_buffer;

    private:
        vorbisbuf& m_src;
        unsigned int m_debug_push_data_count = 0;
        unsigned int m_debug_underflow_count = 0;
    };

    vorbisbuf(std::streambuf* streambuf);

    channelbuf& channel(unsigned int id);

    unsigned int channels_nb();
    unsigned int sample_rate();

private:///std::streambuf override
    int_type underflow() override;

private:
    struct stb_vorbis_deleter{ void operator()(stb_vorbis*); };

    std::streambuf* m_streambuf;
    std::vector<char_type> m_buffer;
    decltype(m_buffer)::iterator m_current;
    std::unique_ptr<stb_vorbis, stb_vorbis_deleter> m_vorbis;
    unsigned int m_sample_rate = 0;

    std::vector<std::unique_ptr<channelbuf>> m_channels;

    unsigned int m_pcm_current_channel = 0;

    bool decode_next_frame();
};


