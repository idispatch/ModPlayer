#include "lamemp3/lame.h"
#include "modplug/modplug.h"
#include "Mp3Export.hpp"
#include <QFile>
#include <QDebug>
#include <QByteArray>

bool Mp3Export::convert(PlaybackConfig &config,
                        QString const& inputFileName,
                        QString const& outputFileName) {
    qDebug() << "Starting converting" << inputFileName << "to" << outputFileName;

    bool result = false;
    QFile fileIn(inputFileName);
    QFile fileOut(outputFileName);
    QByteArray data = fileIn.readAll();

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
        const int numChannels = isStereo ? 2 : 0;

        if(-1 == lame_set_in_samplerate(lame, frequency)) {
            qDebug() << "Failed to set input sample rate";
            break;
        }

        if(-1 == lame_set_out_samplerate(lame, 44100)) {
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

        do {
            readBytes = ModPlug_Read(module, mod_buffer, sizeof(mod_buffer));
            if (readBytes == 0) {
                writeBytes = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
            } else {
                writeBytes = lame_encode_buffer_interleaved(lame,
                                                            reinterpret_cast<short int*>(mod_buffer),
                                                            readBytes,
                                                            mp3_buffer,
                                                            MP3_SIZE);
            }
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
