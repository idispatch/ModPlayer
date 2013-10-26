#include "lamemp3/lame.h"
#include "modplug/modplug.h"
#include "Mp3Export.hpp"
#include <QFile>
#include <QDebug>
#include <QByteArray>

#define DETAILED_LOG

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

#ifdef DETAILED_LOG
    qDebug() << "Read" << data.size() << "bytes from file" << inputFileName;
#endif

    ModPlugFile* module = ModPlug_Load(data.data(), data.size());
    if(module == NULL) {
        qDebug() << "Failed to load module" << inputFileName;
        return result;
    }

#ifdef DETAILED_LOG
    qDebug() << "Module" << inputFileName << " loaded successfully";
#endif

    int readBytes;
    int writeBytes;
    const int PCM_SIZE = 65536;
    const int MP3_SIZE = 65536;

    QByteArray mod_buffer;
    mod_buffer.resize(PCM_SIZE * 2);

    QByteArray mp3_buffer;
    mp3_buffer.resize(MP3_SIZE);

    do {
        lame_t lame = lame_init();
        if(lame == NULL) {
            qDebug() << "Failed to initialise lame";
            break;
        }
#ifdef DETAILED_LOG
        qDebug() << "Lame initialised successfully";
#endif
        const int frequency = config.frequency();
        const bool isStereo = config.stereo();
        const int sampleBitSize = config.sampleSize();
        const int numBytesPerSample = sampleBitSize >> 3;
        const int numChannels = isStereo ? 2 : 0;
#ifdef DETAILED_LOG
        qDebug() << "Frequency:"
                 << frequency
                 << ", stereo:"
                 << isStereo
                 << ", channels:"
                 << numChannels
                 << ", bytes per sample:"
                 << numBytesPerSample;
#endif
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
#ifdef DETAILED_LOG
        qDebug() << "Starting encoding, input buffer="
                 << mod_buffer.size()
                 << ", output buffer="
                 << mp3_buffer.size();
#endif
        do {
            readBytes = ModPlug_Read(module,
                                     mod_buffer.data(),
                                     mod_buffer.size());
#ifdef DETAILED_LOG
            qDebug() << "Read" << readBytes << "bytes";
#endif
            if (readBytes == 0) {
#ifdef DETAILED_LOG
                qDebug() << "Flushing encoder";
#endif
                writeBytes = lame_encode_flush(lame,
                                               reinterpret_cast<unsigned char*>(mp3_buffer.data()),
                                               mp3_buffer.size());
            } else {
                const int numSamplesPerChannel = readBytes / (numBytesPerSample * numChannels);
                writeBytes = lame_encode_buffer_interleaved(lame,
                                                            reinterpret_cast<short int*>(mod_buffer.data()),
                                                            numSamplesPerChannel,
                                                            reinterpret_cast<unsigned char*>(mp3_buffer.data()),
                                                            mp3_buffer.size());
            }

            const int bytesWritten = outputFile.write(mp3_buffer.data(),
                                                writeBytes);
            if(writeBytes != bytesWritten) {
                qDebug() << "Failed to write" << writeBytes << "bytes, written:" << bytesWritten;
                break;
            }
            if(!outputFile.flush()) {
                qDebug() << "Failed to flush";
                break;
            }
#ifdef DETAILED_LOG
            qDebug() << "Wrote" << bytesWritten << "bytes";
#endif
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
