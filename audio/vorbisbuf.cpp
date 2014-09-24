#include "vorbisbuf.h"

#include <system_error>

namespace fys
{

#define ERROR_MESSAGE_FROM_ENUM(e) case e: return #e;

std::error_category& vorbisbuf_error_category()
{
    static struct : public std::error_category
    {
        const char* name() const noexcept override
        {
            return "vorbisbuf";
        }

        std::string message(int e) const override
        {
            switch(e)
            {
                ERROR_MESSAGE_FROM_ENUM(VORBIS_need_more_data)               // not a real error

                ERROR_MESSAGE_FROM_ENUM(VORBIS_invalid_api_mixing)           // can't mix API modes
                ERROR_MESSAGE_FROM_ENUM(VORBIS_outofmem)                     // not enough memory
                ERROR_MESSAGE_FROM_ENUM(VORBIS_feature_not_supported)        // uses floor 0
                ERROR_MESSAGE_FROM_ENUM(VORBIS_too_many_channels)            // STB_VORBIS_MAX_CHANNELS is too small
                ERROR_MESSAGE_FROM_ENUM(VORBIS_file_open_failure)            // fopen() failed
                ERROR_MESSAGE_FROM_ENUM(VORBIS_seek_without_length)          // can't seek in unknown-length file

                ERROR_MESSAGE_FROM_ENUM(VORBIS_unexpected_eof)               // file is truncated?
                ERROR_MESSAGE_FROM_ENUM(VORBIS_seek_invalid)                 // seek past EOF

                // decoding errors (corrupt/invalid stream) -- you probably
                // don't care about the exact details of these

                // vorbis errors:
                ERROR_MESSAGE_FROM_ENUM(VORBIS_invalid_setup)
                ERROR_MESSAGE_FROM_ENUM(VORBIS_invalid_stream)

                // ogg errors:
                ERROR_MESSAGE_FROM_ENUM(VORBIS_missing_capture_pattern)
                ERROR_MESSAGE_FROM_ENUM(VORBIS_invalid_stream_structure_version)
                ERROR_MESSAGE_FROM_ENUM(VORBIS_continued_packet_flag_invalid)
                ERROR_MESSAGE_FROM_ENUM(VORBIS_incorrect_stream_serial_number)
                ERROR_MESSAGE_FROM_ENUM(VORBIS_invalid_first_page)
                ERROR_MESSAGE_FROM_ENUM(VORBIS_bad_packet_type)
                ERROR_MESSAGE_FROM_ENUM(VORBIS_cant_find_last_page)
                ERROR_MESSAGE_FROM_ENUM(VORBIS_seek_failed)

            default:
                return "";
            }
        }
    } _category;

    return _category;
}


vorbisbuf::channelbuf::channelbuf(vorbisbuf& src):
    m_src(src)
{}

auto vorbisbuf::channelbuf::underflow() -> int_type
{
    ++m_debug_underflow_count;

    if(!m_src.decode_next_frame())
        return traits_type::eof();

    if(m_decoded_buffer.empty())
        return traits_type::eof();

    return traits_type::not_eof(m_decoded_buffer[0]);
}

std::streamsize vorbisbuf::channelbuf::xsgetn(char_type* s, std::streamsize count)
{
    size_t data_copied = 0;
    while(1)
    {
        size_t data_to_copy = std::min(static_cast<size_t>(std::distance(gptr(), egptr())), count - data_copied);

        memcpy(s + data_copied, gptr(), data_to_copy);
        data_copied += data_to_copy;

        setg(eback(), gptr() + data_to_copy, egptr());

        if(data_copied == static_cast<size_t>(count))
        {
            return data_copied;
        }

        if(underflow() == traits_type::eof())
            return data_copied;
    }
}


void vorbisbuf::channelbuf::push_data(char_type const* data_begin, size_t data_size)
{
    ++m_debug_push_data_count;

    size_t old_data_still_valid = std::distance(gptr(), egptr());
    size_t final_size = old_data_still_valid + data_size;

    if(final_size <= m_decoded_buffer.capacity())
    {
        memmove(m_decoded_buffer.data(), gptr(), old_data_still_valid);
        m_decoded_buffer.resize(final_size);
        memcpy(m_decoded_buffer.data() + old_data_still_valid, data_begin, data_size);
    }
    else
    {
        decltype(m_decoded_buffer) new_buffer(final_size);
        memcpy(new_buffer.data(), gptr(), old_data_still_valid);
        memcpy(new_buffer.data() + old_data_still_valid, data_begin, data_size);
        std::swap(m_decoded_buffer, new_buffer);
    }

    setg(&*begin(m_decoded_buffer), &*begin(m_decoded_buffer), &*end(m_decoded_buffer));
}



void vorbisbuf::stb_vorbis_deleter::operator()(stb_vorbis* v)
{
    stb_vorbis_close(v);
}


vorbisbuf::vorbisbuf(std::streambuf* streambuf):
    m_streambuf(streambuf),
    m_buffer(32, 0)
{
    //! Remember to open the file like this std::ifstream file("file.ogg", std::ios::in | std::ios::binary); or you might have problem with EOF in the middle of nowhere.

    auto next = m_buffer.begin();

    try_open_vorbis_header:
    {
        std::streamsize get_result = m_streambuf->sgetn(&*next, std::distance(next, m_buffer.end()));

        if(get_result == 0)
            throw std::system_error(VORBIS_unexpected_eof, vorbisbuf_error_category());

        next += get_result;

        int data_used = 0;
        int error = 0;

        m_vorbis.reset(stb_vorbis_open_pushdata(reinterpret_cast<unsigned char*>(m_buffer.data()), std::distance(m_buffer.begin(), next), &data_used, &error, nullptr));

        if(error == STBVorbisError::VORBIS_need_more_data)
        {
            size_t n = std::distance(m_buffer.begin(), next);
            m_buffer.resize(m_buffer.size()*2);//Grow by powers of 2
            next = m_buffer.begin() + n;

            goto try_open_vorbis_header;
        }

        if(error)
            throw std::system_error(error, vorbisbuf_error_category());

        memmove(&*begin(m_buffer), &*(begin(m_buffer) + data_used), m_buffer.size() - data_used);
        m_current = begin(m_buffer) + m_buffer.size() - data_used;
    }

    stb_vorbis_info infos = stb_vorbis_get_info(m_vorbis.get());

    m_sample_rate = infos.sample_rate;

    for(int i = 0; i < infos.channels; ++i)
        m_channels.emplace_back(new channelbuf(*this));
}

auto vorbisbuf::channel(unsigned int id) -> channelbuf&
{
    return *m_channels.at(id);
}

unsigned int vorbisbuf::channels_nb()
{
    return m_channels.size();
}

unsigned int vorbisbuf::sample_rate()
{
    return m_sample_rate;
}


auto vorbisbuf::underflow() -> int_type
{
    if(channels_nb() == 0)
        return traits_type::eof();

    ///TODO à refaire, un appel à setg() est nécessaire

    size_t pcm_current = m_pcm_current_channel;

    (++m_pcm_current_channel) %= channels_nb();

    return m_channels[pcm_current]->sbumpc();
}

bool vorbisbuf::decode_next_frame()
{
    auto next = m_current == end(m_buffer) ? begin(m_buffer) : m_current;

    try_decode_frame:
    {
        std::streamsize get_result = m_streambuf->sgetn(&*next, std::distance(next, m_buffer.end()));

        next += get_result;

        int data_used = 0;
        int channels = 0;
        float** decoded_output = nullptr;
        int samples = 0;

        data_used = stb_vorbis_decode_frame_pushdata(
                        m_vorbis.get(),
                        reinterpret_cast<unsigned char*>(m_buffer.data()),
                        std::distance(begin(m_buffer), next),
                        &channels,
                        &decoded_output,
                        &samples
                    );

        if(data_used == 0)//(need more data)
        {
            int error = stb_vorbis_get_error(m_vorbis.get());
            if(error && error != STBVorbisError::VORBIS_need_more_data)
                throw std::system_error(error, vorbisbuf_error_category());

            if(get_result == 0)
                return false;//EOF

            size_t n = std::distance(m_buffer.begin(), next);
            m_buffer.resize(m_buffer.size()*2);//Grow by powers of 2
            next = m_buffer.begin() + n;

            goto try_decode_frame;
        }

        size_t data_ready = std::distance(begin(m_buffer) + data_used, next);
        memmove(&*begin(m_buffer), &*(begin(m_buffer) + data_used), data_ready);
        m_current = begin(m_buffer) + data_ready;

        if(samples == 0)//(resynching the stream, keep going)
        {
            next = m_current == end(m_buffer) ? begin(m_buffer) : m_current;

            goto try_decode_frame;
        }

        //For each channel output transform float32 to int16 & push it to the corresponding channelbuf buffer
        for(int channel_id = 0; channel_id < channels; ++channel_id)
        {
            char* decoded_buffer_f = reinterpret_cast<char*>(decoded_output[channel_id]);

            //Rewrite the decoded float buffer with int16_t equivalent
            {
                char* write = decoded_buffer_f;
                float* read = reinterpret_cast<float*>(decoded_buffer_f);

                for(; write != decoded_buffer_f + samples*sizeof(int16_t); write += sizeof(int16_t), ++read)
                {
                    //x(s) = x(f)*[max(s) - min(s)]/[max(f) - min(f)] + min(s) - min(f)*[max(s) - min(s)]/[max(f) - min(f)]
                    //For equivalence, we take max(s) = -min(s), so that there are as many + numbers than - numbers
                    //In case x(f) == 1.f, x(s) would be max(s) + 1, so we round it up to max(s)
                    *reinterpret_cast<int16_t*>(write) = (*read) >= 1.f ? 0x7FFF : (*read) < -1.f ? 0x8000 : static_cast<int16_t>((*read)*0x8000);
                }
            }

            m_channels[channel_id]->push_data(decoded_buffer_f, samples*sizeof(int16_t));
        }
    }

    return true;
}

}
