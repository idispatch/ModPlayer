# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        LIBS += -lFlurry

        LIBS +=  $$quote(-L$$BASEDIR/libs/armle-v7)

        PRE_TARGETDEPS +=  $$quote($$BASEDIR/libs/armle-v7)

        SOURCES +=  $$quote($$BASEDIR/src/Analytics.cpp) \
                 $$quote($$BASEDIR/src/Artist.cpp) \
                 $$quote($$BASEDIR/src/Cache.cpp) \
                 $$quote($$BASEDIR/src/Canvas.cpp) \
                 $$quote($$BASEDIR/src/Catalog.cpp) \
                 $$quote($$BASEDIR/src/Downloader.cpp) \
                 $$quote($$BASEDIR/src/FileUtils.cpp) \
                 $$quote($$BASEDIR/src/ItemGroupBase.cpp) \
                 $$quote($$BASEDIR/src/LCDDigits.cpp) \
                 $$quote($$BASEDIR/src/LCDDisplay.cpp) \
                 $$quote($$BASEDIR/src/ModPlayback.cpp) \
                 $$quote($$BASEDIR/src/PatternView.cpp) \
                 $$quote($$BASEDIR/src/PlaybackConfig.cpp) \
                 $$quote($$BASEDIR/src/Player.cpp) \
                 $$quote($$BASEDIR/src/SongBasicInfo.cpp) \
                 $$quote($$BASEDIR/src/SongExtendedInfo.cpp) \
                 $$quote($$BASEDIR/src/SongFormat.cpp) \
                 $$quote($$BASEDIR/src/SongGenre.cpp) \
                 $$quote($$BASEDIR/src/SongModule.cpp) \
                 $$quote($$BASEDIR/src/TouchHandler.cpp) \
                 $$quote($$BASEDIR/src/Unpacker.cpp) \
                 $$quote($$BASEDIR/src/VUMeter.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/lamemp3/VbrTag.c) \
                 $$quote($$BASEDIR/src/lamemp3/bitstream.c) \
                 $$quote($$BASEDIR/src/lamemp3/encoder.c) \
                 $$quote($$BASEDIR/src/lamemp3/fft.c) \
                 $$quote($$BASEDIR/src/lamemp3/gain_analysis.c) \
                 $$quote($$BASEDIR/src/lamemp3/id3tag.c) \
                 $$quote($$BASEDIR/src/lamemp3/lame.c) \
                 $$quote($$BASEDIR/src/lamemp3/mpglib_interface.c) \
                 $$quote($$BASEDIR/src/lamemp3/newmdct.c) \
                 $$quote($$BASEDIR/src/lamemp3/presets.c) \
                 $$quote($$BASEDIR/src/lamemp3/psymodel.c) \
                 $$quote($$BASEDIR/src/lamemp3/quantize.c) \
                 $$quote($$BASEDIR/src/lamemp3/quantize_pvt.c) \
                 $$quote($$BASEDIR/src/lamemp3/reservoir.c) \
                 $$quote($$BASEDIR/src/lamemp3/set_get.c) \
                 $$quote($$BASEDIR/src/lamemp3/tables.c) \
                 $$quote($$BASEDIR/src/lamemp3/takehiro.c) \
                 $$quote($$BASEDIR/src/lamemp3/util.c) \
                 $$quote($$BASEDIR/src/lamemp3/vbrquantize.c) \
                 $$quote($$BASEDIR/src/lamemp3/vector/xmm_quantize_sub.c) \
                 $$quote($$BASEDIR/src/lamemp3/version.c) \
                 $$quote($$BASEDIR/src/lcd_font_6x8.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/modplug/fastmix.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_669.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_abc.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_amf.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_ams.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_dbm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_dmf.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_dsm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_far.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_it.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_j2b.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mdl.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_med.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mid.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mod.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mt2.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mtm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_okt.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_pat.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_psm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_ptm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_s3m.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_stm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_ult.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_umx.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_wav.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_xm.cpp) \
                 $$quote($$BASEDIR/src/modplug/mmcmp.cpp) \
                 $$quote($$BASEDIR/src/modplug/modplug.cpp) \
                 $$quote($$BASEDIR/src/modplug/snd_dsp.cpp) \
                 $$quote($$BASEDIR/src/modplug/snd_flt.cpp) \
                 $$quote($$BASEDIR/src/modplug/snd_fx.cpp) \
                 $$quote($$BASEDIR/src/modplug/sndfile.cpp) \
                 $$quote($$BASEDIR/src/modplug/sndmix.cpp) \
                 $$quote($$BASEDIR/src/zip/JlCompress.cpp) \
                 $$quote($$BASEDIR/src/zip/qioapi.cpp) \
                 $$quote($$BASEDIR/src/zip/quaadler32.cpp) \
                 $$quote($$BASEDIR/src/zip/quacrc32.cpp) \
                 $$quote($$BASEDIR/src/zip/quagzipfile.cpp) \
                 $$quote($$BASEDIR/src/zip/quaziodevice.cpp) \
                 $$quote($$BASEDIR/src/zip/quazip.cpp) \
                 $$quote($$BASEDIR/src/zip/quazipdir.cpp) \
                 $$quote($$BASEDIR/src/zip/quazipfile.cpp) \
                 $$quote($$BASEDIR/src/zip/quazipfileinfo.cpp) \
                 $$quote($$BASEDIR/src/zip/quazipnewinfo.cpp) \
                 $$quote($$BASEDIR/src/zip/unzip.c) \
                 $$quote($$BASEDIR/src/zip/zip.c)

        HEADERS +=  $$quote($$BASEDIR/src/Analytics.hpp) \
                 $$quote($$BASEDIR/src/Artist.hpp) \
                 $$quote($$BASEDIR/src/Cache.hpp) \
                 $$quote($$BASEDIR/src/Canvas.hpp) \
                 $$quote($$BASEDIR/src/Catalog.hpp) \
                 $$quote($$BASEDIR/src/Downloader.hpp) \
                 $$quote($$BASEDIR/src/FileUtils.hpp) \
                 $$quote($$BASEDIR/src/Flurry/Flurry.h) \
                 $$quote($$BASEDIR/src/InstanceCounter.hpp) \
                 $$quote($$BASEDIR/src/ItemGroupBase.hpp) \
                 $$quote($$BASEDIR/src/LCDDigits.hpp) \
                 $$quote($$BASEDIR/src/LCDDisplay.hpp) \
                 $$quote($$BASEDIR/src/ModPlayback.hpp) \
                 $$quote($$BASEDIR/src/PatternView.hpp) \
                 $$quote($$BASEDIR/src/PlaybackConfig.hpp) \
                 $$quote($$BASEDIR/src/Player.hpp) \
                 $$quote($$BASEDIR/src/SongBasicInfo.hpp) \
                 $$quote($$BASEDIR/src/SongExtendedInfo.hpp) \
                 $$quote($$BASEDIR/src/SongFormat.hpp) \
                 $$quote($$BASEDIR/src/SongGenre.hpp) \
                 $$quote($$BASEDIR/src/SongModule.hpp) \
                 $$quote($$BASEDIR/src/TouchHandler.hpp) \
                 $$quote($$BASEDIR/src/Unpacker.hpp) \
                 $$quote($$BASEDIR/src/VUMeter.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/lamemp3/VbrTag.h) \
                 $$quote($$BASEDIR/src/lamemp3/bitstream.h) \
                 $$quote($$BASEDIR/src/lamemp3/encoder.h) \
                 $$quote($$BASEDIR/src/lamemp3/fft.h) \
                 $$quote($$BASEDIR/src/lamemp3/gain_analysis.h) \
                 $$quote($$BASEDIR/src/lamemp3/id3tag.h) \
                 $$quote($$BASEDIR/src/lamemp3/l3side.h) \
                 $$quote($$BASEDIR/src/lamemp3/lame-analysis.h) \
                 $$quote($$BASEDIR/src/lamemp3/lame.h) \
                 $$quote($$BASEDIR/src/lamemp3/lame_global_flags.h) \
                 $$quote($$BASEDIR/src/lamemp3/lameerror.h) \
                 $$quote($$BASEDIR/src/lamemp3/machine.h) \
                 $$quote($$BASEDIR/src/lamemp3/newmdct.h) \
                 $$quote($$BASEDIR/src/lamemp3/psymodel.h) \
                 $$quote($$BASEDIR/src/lamemp3/quantize.h) \
                 $$quote($$BASEDIR/src/lamemp3/quantize_pvt.h) \
                 $$quote($$BASEDIR/src/lamemp3/reservoir.h) \
                 $$quote($$BASEDIR/src/lamemp3/set_get.h) \
                 $$quote($$BASEDIR/src/lamemp3/tables.h) \
                 $$quote($$BASEDIR/src/lamemp3/util.h) \
                 $$quote($$BASEDIR/src/lamemp3/vbrquantize.h) \
                 $$quote($$BASEDIR/src/lamemp3/vector/lame_intrin.h) \
                 $$quote($$BASEDIR/src/lamemp3/version.h) \
                 $$quote($$BASEDIR/src/lcd_fonts.hpp) \
                 $$quote($$BASEDIR/src/modplug/config.h) \
                 $$quote($$BASEDIR/src/modplug/it_defs.h) \
                 $$quote($$BASEDIR/src/modplug/load_pat.h) \
                 $$quote($$BASEDIR/src/modplug/modplug.h) \
                 $$quote($$BASEDIR/src/modplug/sndfile.h) \
                 $$quote($$BASEDIR/src/modplug/stdafx.h) \
                 $$quote($$BASEDIR/src/modplug/tables.h) \
                 $$quote($$BASEDIR/src/zip/JlCompress.h) \
                 $$quote($$BASEDIR/src/zip/crypt.h) \
                 $$quote($$BASEDIR/src/zip/ioapi.h) \
                 $$quote($$BASEDIR/src/zip/quaadler32.h) \
                 $$quote($$BASEDIR/src/zip/quachecksum32.h) \
                 $$quote($$BASEDIR/src/zip/quacrc32.h) \
                 $$quote($$BASEDIR/src/zip/quagzipfile.h) \
                 $$quote($$BASEDIR/src/zip/quaziodevice.h) \
                 $$quote($$BASEDIR/src/zip/quazip.h) \
                 $$quote($$BASEDIR/src/zip/quazip_global.h) \
                 $$quote($$BASEDIR/src/zip/quazipdir.h) \
                 $$quote($$BASEDIR/src/zip/quazipfile.h) \
                 $$quote($$BASEDIR/src/zip/quazipfileinfo.h) \
                 $$quote($$BASEDIR/src/zip/quazipnewinfo.h) \
                 $$quote($$BASEDIR/src/zip/unzip.h) \
                 $$quote($$BASEDIR/src/zip/zip.h)
    }

    CONFIG(release, debug|release) {
        LIBS += -lFlurry

        LIBS +=  $$quote(-L$$BASEDIR/libs/armle-v7)

        PRE_TARGETDEPS +=  $$quote($$BASEDIR/libs/armle-v7)

        SOURCES +=  $$quote($$BASEDIR/src/Analytics.cpp) \
                 $$quote($$BASEDIR/src/Artist.cpp) \
                 $$quote($$BASEDIR/src/Cache.cpp) \
                 $$quote($$BASEDIR/src/Canvas.cpp) \
                 $$quote($$BASEDIR/src/Catalog.cpp) \
                 $$quote($$BASEDIR/src/Downloader.cpp) \
                 $$quote($$BASEDIR/src/FileUtils.cpp) \
                 $$quote($$BASEDIR/src/ItemGroupBase.cpp) \
                 $$quote($$BASEDIR/src/LCDDigits.cpp) \
                 $$quote($$BASEDIR/src/LCDDisplay.cpp) \
                 $$quote($$BASEDIR/src/ModPlayback.cpp) \
                 $$quote($$BASEDIR/src/PatternView.cpp) \
                 $$quote($$BASEDIR/src/PlaybackConfig.cpp) \
                 $$quote($$BASEDIR/src/Player.cpp) \
                 $$quote($$BASEDIR/src/SongBasicInfo.cpp) \
                 $$quote($$BASEDIR/src/SongExtendedInfo.cpp) \
                 $$quote($$BASEDIR/src/SongFormat.cpp) \
                 $$quote($$BASEDIR/src/SongGenre.cpp) \
                 $$quote($$BASEDIR/src/SongModule.cpp) \
                 $$quote($$BASEDIR/src/TouchHandler.cpp) \
                 $$quote($$BASEDIR/src/Unpacker.cpp) \
                 $$quote($$BASEDIR/src/VUMeter.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/lamemp3/VbrTag.c) \
                 $$quote($$BASEDIR/src/lamemp3/bitstream.c) \
                 $$quote($$BASEDIR/src/lamemp3/encoder.c) \
                 $$quote($$BASEDIR/src/lamemp3/fft.c) \
                 $$quote($$BASEDIR/src/lamemp3/gain_analysis.c) \
                 $$quote($$BASEDIR/src/lamemp3/id3tag.c) \
                 $$quote($$BASEDIR/src/lamemp3/lame.c) \
                 $$quote($$BASEDIR/src/lamemp3/mpglib_interface.c) \
                 $$quote($$BASEDIR/src/lamemp3/newmdct.c) \
                 $$quote($$BASEDIR/src/lamemp3/presets.c) \
                 $$quote($$BASEDIR/src/lamemp3/psymodel.c) \
                 $$quote($$BASEDIR/src/lamemp3/quantize.c) \
                 $$quote($$BASEDIR/src/lamemp3/quantize_pvt.c) \
                 $$quote($$BASEDIR/src/lamemp3/reservoir.c) \
                 $$quote($$BASEDIR/src/lamemp3/set_get.c) \
                 $$quote($$BASEDIR/src/lamemp3/tables.c) \
                 $$quote($$BASEDIR/src/lamemp3/takehiro.c) \
                 $$quote($$BASEDIR/src/lamemp3/util.c) \
                 $$quote($$BASEDIR/src/lamemp3/vbrquantize.c) \
                 $$quote($$BASEDIR/src/lamemp3/vector/xmm_quantize_sub.c) \
                 $$quote($$BASEDIR/src/lamemp3/version.c) \
                 $$quote($$BASEDIR/src/lcd_font_6x8.cpp) \
                 $$quote($$BASEDIR/src/main.cpp) \
                 $$quote($$BASEDIR/src/modplug/fastmix.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_669.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_abc.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_amf.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_ams.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_dbm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_dmf.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_dsm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_far.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_it.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_j2b.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mdl.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_med.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mid.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mod.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mt2.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_mtm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_okt.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_pat.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_psm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_ptm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_s3m.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_stm.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_ult.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_umx.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_wav.cpp) \
                 $$quote($$BASEDIR/src/modplug/load_xm.cpp) \
                 $$quote($$BASEDIR/src/modplug/mmcmp.cpp) \
                 $$quote($$BASEDIR/src/modplug/modplug.cpp) \
                 $$quote($$BASEDIR/src/modplug/snd_dsp.cpp) \
                 $$quote($$BASEDIR/src/modplug/snd_flt.cpp) \
                 $$quote($$BASEDIR/src/modplug/snd_fx.cpp) \
                 $$quote($$BASEDIR/src/modplug/sndfile.cpp) \
                 $$quote($$BASEDIR/src/modplug/sndmix.cpp) \
                 $$quote($$BASEDIR/src/zip/JlCompress.cpp) \
                 $$quote($$BASEDIR/src/zip/qioapi.cpp) \
                 $$quote($$BASEDIR/src/zip/quaadler32.cpp) \
                 $$quote($$BASEDIR/src/zip/quacrc32.cpp) \
                 $$quote($$BASEDIR/src/zip/quagzipfile.cpp) \
                 $$quote($$BASEDIR/src/zip/quaziodevice.cpp) \
                 $$quote($$BASEDIR/src/zip/quazip.cpp) \
                 $$quote($$BASEDIR/src/zip/quazipdir.cpp) \
                 $$quote($$BASEDIR/src/zip/quazipfile.cpp) \
                 $$quote($$BASEDIR/src/zip/quazipfileinfo.cpp) \
                 $$quote($$BASEDIR/src/zip/quazipnewinfo.cpp) \
                 $$quote($$BASEDIR/src/zip/unzip.c) \
                 $$quote($$BASEDIR/src/zip/zip.c)

        HEADERS +=  $$quote($$BASEDIR/src/Analytics.hpp) \
                 $$quote($$BASEDIR/src/Artist.hpp) \
                 $$quote($$BASEDIR/src/Cache.hpp) \
                 $$quote($$BASEDIR/src/Canvas.hpp) \
                 $$quote($$BASEDIR/src/Catalog.hpp) \
                 $$quote($$BASEDIR/src/Downloader.hpp) \
                 $$quote($$BASEDIR/src/FileUtils.hpp) \
                 $$quote($$BASEDIR/src/Flurry/Flurry.h) \
                 $$quote($$BASEDIR/src/InstanceCounter.hpp) \
                 $$quote($$BASEDIR/src/ItemGroupBase.hpp) \
                 $$quote($$BASEDIR/src/LCDDigits.hpp) \
                 $$quote($$BASEDIR/src/LCDDisplay.hpp) \
                 $$quote($$BASEDIR/src/ModPlayback.hpp) \
                 $$quote($$BASEDIR/src/PatternView.hpp) \
                 $$quote($$BASEDIR/src/PlaybackConfig.hpp) \
                 $$quote($$BASEDIR/src/Player.hpp) \
                 $$quote($$BASEDIR/src/SongBasicInfo.hpp) \
                 $$quote($$BASEDIR/src/SongExtendedInfo.hpp) \
                 $$quote($$BASEDIR/src/SongFormat.hpp) \
                 $$quote($$BASEDIR/src/SongGenre.hpp) \
                 $$quote($$BASEDIR/src/SongModule.hpp) \
                 $$quote($$BASEDIR/src/TouchHandler.hpp) \
                 $$quote($$BASEDIR/src/Unpacker.hpp) \
                 $$quote($$BASEDIR/src/VUMeter.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/lamemp3/VbrTag.h) \
                 $$quote($$BASEDIR/src/lamemp3/bitstream.h) \
                 $$quote($$BASEDIR/src/lamemp3/encoder.h) \
                 $$quote($$BASEDIR/src/lamemp3/fft.h) \
                 $$quote($$BASEDIR/src/lamemp3/gain_analysis.h) \
                 $$quote($$BASEDIR/src/lamemp3/id3tag.h) \
                 $$quote($$BASEDIR/src/lamemp3/l3side.h) \
                 $$quote($$BASEDIR/src/lamemp3/lame-analysis.h) \
                 $$quote($$BASEDIR/src/lamemp3/lame.h) \
                 $$quote($$BASEDIR/src/lamemp3/lame_global_flags.h) \
                 $$quote($$BASEDIR/src/lamemp3/lameerror.h) \
                 $$quote($$BASEDIR/src/lamemp3/machine.h) \
                 $$quote($$BASEDIR/src/lamemp3/newmdct.h) \
                 $$quote($$BASEDIR/src/lamemp3/psymodel.h) \
                 $$quote($$BASEDIR/src/lamemp3/quantize.h) \
                 $$quote($$BASEDIR/src/lamemp3/quantize_pvt.h) \
                 $$quote($$BASEDIR/src/lamemp3/reservoir.h) \
                 $$quote($$BASEDIR/src/lamemp3/set_get.h) \
                 $$quote($$BASEDIR/src/lamemp3/tables.h) \
                 $$quote($$BASEDIR/src/lamemp3/util.h) \
                 $$quote($$BASEDIR/src/lamemp3/vbrquantize.h) \
                 $$quote($$BASEDIR/src/lamemp3/vector/lame_intrin.h) \
                 $$quote($$BASEDIR/src/lamemp3/version.h) \
                 $$quote($$BASEDIR/src/lcd_fonts.hpp) \
                 $$quote($$BASEDIR/src/modplug/config.h) \
                 $$quote($$BASEDIR/src/modplug/it_defs.h) \
                 $$quote($$BASEDIR/src/modplug/load_pat.h) \
                 $$quote($$BASEDIR/src/modplug/modplug.h) \
                 $$quote($$BASEDIR/src/modplug/sndfile.h) \
                 $$quote($$BASEDIR/src/modplug/stdafx.h) \
                 $$quote($$BASEDIR/src/modplug/tables.h) \
                 $$quote($$BASEDIR/src/zip/JlCompress.h) \
                 $$quote($$BASEDIR/src/zip/crypt.h) \
                 $$quote($$BASEDIR/src/zip/ioapi.h) \
                 $$quote($$BASEDIR/src/zip/quaadler32.h) \
                 $$quote($$BASEDIR/src/zip/quachecksum32.h) \
                 $$quote($$BASEDIR/src/zip/quacrc32.h) \
                 $$quote($$BASEDIR/src/zip/quagzipfile.h) \
                 $$quote($$BASEDIR/src/zip/quaziodevice.h) \
                 $$quote($$BASEDIR/src/zip/quazip.h) \
                 $$quote($$BASEDIR/src/zip/quazip_global.h) \
                 $$quote($$BASEDIR/src/zip/quazipdir.h) \
                 $$quote($$BASEDIR/src/zip/quazipfile.h) \
                 $$quote($$BASEDIR/src/zip/quazipfileinfo.h) \
                 $$quote($$BASEDIR/src/zip/quazipnewinfo.h) \
                 $$quote($$BASEDIR/src/zip/unzip.h) \
                 $$quote($$BASEDIR/src/zip/zip.h)
    }
}

INCLUDEPATH +=  $$quote($$BASEDIR/src/Flurry) \
         $$quote($$BASEDIR/src/zip) \
         $$quote($$BASEDIR/src/lamemp3) \
         $$quote($$BASEDIR/src/modplug) \
         $$quote($$BASEDIR/src/lamemp3/vector) \
         $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}_ru.ts) \
         $$quote($${TARGET}.ts)
