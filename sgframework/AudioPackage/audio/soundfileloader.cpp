#include "soundfileloader.h"
#include <qdebug.h>

#define SWAP_32(value)                 \
        (((((unsigned short)value)<<8) & 0xFF00)   | \
         ((((unsigned short)value)>>8) & 0x00FF))

#define SWAP_16(value)                     \
        (((((unsigned int)value)<<24) & 0xFF000000)  | \
         ((((unsigned int)value)<< 8) & 0x00FF0000)  | \
         ((((unsigned int)value)>> 8) & 0x0000FF00)  | \
         ((((unsigned int)value)>>24) & 0x000000FF))


#define FAIL(X) { XPLMDebugString(X); free(mem); return 0; }

#define RIFF_ID 0x46464952			// 'RIFF'
#define FMT_ID  0x20746D66			// 'FMT '
#define DATA_ID 0x61746164			// 'DATA'


SoundFileLoader::SoundFileLoader()
{

}

SoundFileLoader::~SoundFileLoader()
{

}


    char* SoundFileLoader::find_chunk(char* file_begin, char* file_end, int desired_id, int swapped)
    {
        while (file_begin < file_end)
        {
            chunk_header* h = (chunk_header *)file_begin;
            if (h->id == desired_id && !swapped)
                return file_begin + sizeof(chunk_header);
            if (h->id == SWAP_32(desired_id) && swapped)
                return file_begin + sizeof(chunk_header);
            int chunk_size = swapped ? SWAP_32(h->size) : h->size;
            char* next = file_begin + chunk_size + sizeof(chunk_header);
            if (next > file_end || next <= file_begin)
                return NULL;
            file_begin = next;
        }
        return NULL;
    }


    char* SoundFileLoader::chunk_end(char* chunk_start, int swapped)
    {
        chunk_header* h = (chunk_header *)(chunk_start - sizeof(chunk_header));
        return chunk_start + (swapped ? SWAP_32(h->size) : h->size);
    }


unsigned int SoundFileLoader::loadFile(const QString &filePath)
{
    FILE* fi = fopen(filePath.toUtf8().data(), "rb");
    if (fi == NULL)
    {
        qDebug()<<"WAVE file load failed - could not open.\n";
    }
    fseek(fi, 0, SEEK_END);
    int file_size = ftell(fi);
    fseek(fi, 0, SEEK_SET);
    char* mem = (char*)malloc(file_size);
    if (mem == NULL)
    {
        qDebug()<<"WAVE file load failed - could not allocate memory.\n";
        fclose(fi);
    }
    if (fread(mem, 1, file_size, fi) != file_size)
    {
        qDebug()<<"WAVE file load failed - could not read file.\n";
        free(mem);
        fclose(fi);
    }
    fclose(fi);
    char* mem_end = mem + file_size;

    int swapped = 0;
    char* riff = find_chunk(mem, mem_end, RIFF_ID, 0);
    if (riff == NULL)
    {
        riff = find_chunk(mem, mem_end, RIFF_ID, 1);
        if (riff)
            swapped = 1;
        else
        qDebug()<<"Could not find RIFF chunk in wave file.\n";
    }

    if (riff[0] != 'W' ||
        riff[1] != 'A' ||
        riff[2] != 'V' ||
        riff[3] != 'E')
    qDebug()<<"Could not find WAVE signature in wave file.\n";

    char* format = find_chunk(riff + 4, chunk_end(riff, swapped), FMT_ID, swapped);
    if (format == NULL)
    qDebug()<<"Could not find FMT  chunk in wave file.\n";


    format_info* fmt = (format_info *)format;
    if (swapped)
    {
        fmt->format = SWAP_16(fmt->format);
        fmt->num_channels = SWAP_16(fmt->num_channels);
        fmt->sample_rate = SWAP_32(fmt->sample_rate);
        fmt->byte_rate = SWAP_32(fmt->byte_rate);
        fmt->block_align = SWAP_16(fmt->block_align);
        fmt->bits_per_sample = SWAP_16(fmt->bits_per_sample);
    }

    if (fmt->format != 1)
    qDebug()<<"Wave file is not PCM format data.\n";
    if (fmt->num_channels != 1 && fmt->num_channels != 2)
    qDebug()<<"Must have mono or stereo sound.\n";
    if (fmt->bits_per_sample != 8 && fmt->bits_per_sample != 16)
    qDebug()<<"Must have 8 or 16 bit sounds.\n";
    char* data = find_chunk(riff + 4, chunk_end(riff, swapped), DATA_ID, swapped);
    if (data == NULL)
    qDebug()<<"I could not find the DATA chunk.\n";

    int sample_size = fmt->num_channels * fmt->bits_per_sample / 8;
    int data_bytes = chunk_end(data, swapped) - data;
    int data_samples = data_bytes / sample_size;

    if (fmt->bits_per_sample == 16 && swapped)
    {
        short* ptr = (short *)data;
        int words = data_samples * fmt->num_channels;
        while (words--)
        {
            *ptr = SWAP_16(*ptr);
            ++ptr;
        }
    }


    ALuint buf_id = 0;
    OpenALHelper::funcalGenBuffers(1, &buf_id);
    if (buf_id == 0)
    qDebug()<<"Could not generate buffer id.\n";

    OpenALHelper::funcalBufferData(buf_id, fmt->bits_per_sample == 16 ?
                             (fmt->num_channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16) :
                             (fmt->num_channels == 2 ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8),
                 data, data_bytes, fmt->sample_rate);
    free(mem);
    return buf_id;
}
