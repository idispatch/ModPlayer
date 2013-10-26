#include "lamemp3/lame.h"
#include "modplug/modplug.h"
#include "Mp3Export.hpp"
#include <QFile>
#include <QDebug>
#include <QByteArray>

bool Mp3Export::convert(PlaybackConfig &config,
                        QString const& inputFileName,
                        QString const& outputFileName) {
    bool result = false;
    qDebug() << "Starting converting" << inputFileName << "to" << outputFileName;

    QFile inputFile(inputFileName);
    if(!inputFile.exists()) {
        qDebug() << "Input file" << inputFileName << "does not exist";
        return result;
    }
    if(!inputFile.open(QFile::ReadOnly)) {
        qDebug() << "Failed to open input file" << inputFileName;
        return result;
    }

    QFile outputFile(outputFileName);
    if(outputFile.exists()) {
        qDebug() << "Deleting output file" << outputFileName;
        if(!outputFile.remove()) {
            qDebug() << "Failed to delete output file" << outputFileName;
            return result;
        }
    }

    if(!outputFile.open(QFile::WriteOnly)) {
        qDebug() << "Failed to open output file" << outputFileName;
        return result;
    }

    QByteArray data = inputFile.readAll();
    if(data.size() == 0) {
        qDebug() << "Failed to read file" << inputFileName;
        return result;
    }

    qDebug() << "Read" << data.size() << "bytes from file" << inputFileName;

    ModPlugFile* module = ModPlug_Load(data.data(), data.size());
    if(module == NULL) {
        qDebug() << "Failed to load module" << inputFileName;
        return result;
    }

    qDebug() << "Module" << inputFileName << " loaded successfully";

    int readBytes;
    int writeBytes;
    const int PCM_SIZE = 16384;
    const int MP3_SIZE = 16384;

    char mod_buffer[PCM_SIZE * 2];
    unsigned char mp3_buffer[MP3_SIZE];

    do {
        lame_t lame = lame_init();
        if(lame == NULL) {
            qDebug() << "Failed to initialise lame";
            break;
        }

        qDebug() << "Lame initialised successfully";

        const int frequency = config.frequency();
        const bool isStereo = config.stereo();
        const int sampleBitSize = config.sampleSize();
        const int numBytesPerSample = sampleBitSize >> 3;
        const int numChannels = isStereo ? 2 : 0;

        qDebug() << "Frequency:"
                 << frequency
                 << ", stereo:"
                 << isStereo
                 << ", channels:"
                 << numChannels
                 << ", bytes per sample:"
                 << numBytesPerSample;

        if(-1 == lame_set_in_samplerate(lame, frequency)) {
            qDebug() << "Failed to set input sample rate";
            break;
        }

        if(-1 == lame_set_out_samplerate(lame, frequency)) {
            qDebug() << "Failed to set output sample rate";
            break;
        }

        if(-1 == lame_set_num_channels(lame, numChannels)) {
            qDebug() << "Failed to set number of channels";
            break;
        }

        if(-1 == lame_set_quality(lame, 9)) {
            qDebug() << "Failed to set quality";
            break;
        }

        if(-1 == lame_set_mode(lame, isStereo ? STEREO : MONO)) {
            qDebug() << "Failed to set mode";
            break;
        }

        if(-1 == lame_set_VBR(lame, vbr_default)) {
            qDebug() << "Failed to set variable bit rate";
            break;
        }

        if(0 != lame_init_params(lame)) {
            qDebug() << "Failed to initialise lame parameters";
            break;
        }

        qDebug() << "Starting encoding, input buffer="
                 << sizeof(mod_buffer)
                 << ", output buffer="
                 << sizeof(mp3_buffer);

        do {
            readBytes = ModPlug_Read(module, mod_buffer, sizeof(mod_buffer));
            qDebug() << "Read" << readBytes << "bytes";

            if (readBytes == 0) {
                qDebug() << "Flushing encoder";
                writeBytes = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
            } else {
                int numSamplesPerChannel = readBytes / (numBytesPerSample * numChannels);
                writeBytes = lame_encode_buffer_interleaved(lame,
                                                            reinterpret_cast<short int*>(mod_buffer),
                                                            numSamplesPerChannel,
                                                            mp3_buffer,
                                                            MP3_SIZE);
            }

            int bytesWritten = outputFile.write(reinterpret_cast<const char*>(mp3_buffer),
                                                writeBytes);
            if(writeBytes != bytesWritten) {
                qDebug() << "Failed to write" << writeBytes << "bytes, written:" << bytesWritten;
                break;
            }
            if(!outputFile.flush()) {
                qDebug() << "Failed to flush";
                break;
            }

            qDebug() << "Wrote" << bytesWritten << "bytes";
        } while(readBytes > 0);

        if(0 != lame_close(lame)) {
            qDebug() << "Failed to close lame";
            break;
        }

        result = true;
    } while(0);

    ModPlug_Unload(module);
    return result;
}
