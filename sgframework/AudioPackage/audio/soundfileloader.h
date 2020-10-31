#ifndef SOUNDFILELOADER_H
#define SOUNDFILELOADER_H
#include <QString>
#include "openalhelper.h"

class SoundFileLoader
{
public:
    SoundFileLoader();
    ~SoundFileLoader();

    //!
    //! \brief loadFile Läd Audiodatei und erstellt OpenAL Buffer
    //! \param filePath Pfad zur Audiodatei(Nur WAV PCM 16 Bit)
    //! \return ID des OpenAL Buffers
    //!
    unsigned int loadFile(const QString &filePath);

private:

    struct chunk_header
    {
        int id;
        int size;
    };

    // WAVE file format info.  We pass this through to OpenAL so we can support mono/stereo, 8/16/bit, etc.
    //!
    //! \brief format_info struct Informationen die für OpenAL benötigt werden
    //!
    struct format_info
    {
        short format;
        short num_channels;
        int sample_rate;
        int byte_rate;
        short block_align;
        short bits_per_sample;
    };

    //!
    //! \brief find_chunk findet Anfang der Daten für einen für angegebenen Bereich an Bytes
    // swapped if the machine is not the same endian as the file.

    //! \param file_begin Anfang der Datei
    //! \param file_end Ende der Datei
    //! \param desired_id Bereich
    //! \param swapped Angabe ob Endian auf der Machine anderst als im WAV File. 1 für ja
    //! \return
    //!
     char* find_chunk(char* file_begin, char* file_end, int desired_id, int swapped);

     //!
     //! \brief chunk_end findet das Ende indem zurück zum Header gegangen wird
     //! \param chunk_start Anfang des Chunks
     //! \param swapped Angabe ob Endian auf der Machine anderst als im WAV File. 1 für ja
     //! \return
     //!
     char* chunk_end(char* chunk_start, int swapped);
};

#endif // SOUNDFILELOADER_H
