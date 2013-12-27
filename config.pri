# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        LIBS += -lFlurry

        LIBS +=  $$quote(-L$$BASEDIR/libs/armle-v7)

        PRE_TARGETDEPS +=  $$quote($$BASEDIR/libs/armle-v7)

        SOURCES +=  $$quote($$BASEDIR/src/Analytics.cpp) \
                 $$quote($$BASEDIR/src/ApplicationVersion.cpp) \
                 $$quote($$BASEDIR/src/Artist.cpp) \
                 $$quote($$BASEDIR/src/Cache.cpp) \
                 $$quote($$BASEDIR/src/Canvas.cpp) \
                 $$quote($$BASEDIR/src/Catalog.cpp) \
                 $$quote($$BASEDIR/src/Downloader.cpp) \
                 $$quote($$BASEDIR/src/FileUtils.cpp) \
                 $$quote($$BASEDIR/src/Importer.cpp) \
                 $$quote($$BASEDIR/src/ItemGroupBase.cpp) \
                 $$quote($$BASEDIR/src/LCDDigits.cpp) \
                 $$quote($$BASEDIR/src/LCDDisplay.cpp) \
                 $$quote($$BASEDIR/src/ModPlayback.cpp) \
                 $$quote($$BASEDIR/src/Mp3Export.cpp) \
                 $$quote($$BASEDIR/src/NamedPlaylist.cpp) \
                 $$quote($$BASEDIR/src/PatternView.cpp) \
                 $$quote($$BASEDIR/src/PlaybackConfig.cpp) \
                 $$quote($$BASEDIR/src/Player.cpp) \
                 $$quote($$BASEDIR/src/Playlist.cpp) \
                 $$quote($$BASEDIR/src/SongBasicInfo.cpp) \
                 $$quote($$BASEDIR/src/SongExtendedInfo.cpp) \
                 $$quote($$BASEDIR/src/SongFormat.cpp) \
                 $$quote($$BASEDIR/src/SongGenre.cpp) \
                 $$quote($$BASEDIR/src/SongModule.cpp) \
                 $$quote($$BASEDIR/src/SuspendPlayback.cpp) \
                 $$quote($$BASEDIR/src/TouchHandler.cpp) \
                 $$quote($$BASEDIR/src/Unpacker.cpp) \
                 $$quote($$BASEDIR/src/VUMeter.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/lcd_font_6x8.cpp) \
                 $$quote($$BASEDIR/src/libid3tag/id3_compat.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_crc.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_debug.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_field.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_file.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_frame.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_frametype.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_genre.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_latin1.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_parse.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_render.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_tag.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_ucs4.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_utf16.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_utf8.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_util.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_version.c) \
                 $$quote($$BASEDIR/src/libmad/mad_bit.c) \
                 $$quote($$BASEDIR/src/libmad/mad_decoder.c) \
                 $$quote($$BASEDIR/src/libmad/mad_fixed.c) \
                 $$quote($$BASEDIR/src/libmad/mad_frame.c) \
                 $$quote($$BASEDIR/src/libmad/mad_huffman.c) \
                 $$quote($$BASEDIR/src/libmad/mad_layer12.c) \
                 $$quote($$BASEDIR/src/libmad/mad_layer3.c) \
                 $$quote($$BASEDIR/src/libmad/mad_minimad.c) \
                 $$quote($$BASEDIR/src/libmad/mad_stream.c) \
                 $$quote($$BASEDIR/src/libmad/mad_synth.c) \
                 $$quote($$BASEDIR/src/libmad/mad_timer.c) \
                 $$quote($$BASEDIR/src/libmad/mad_version.c) \
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
                 $$quote($$BASEDIR/src/mp3lame/VbrTag.c) \
                 $$quote($$BASEDIR/src/mp3lame/bitstream.c) \
                 $$quote($$BASEDIR/src/mp3lame/encoder.c) \
                 $$quote($$BASEDIR/src/mp3lame/fft.c) \
                 $$quote($$BASEDIR/src/mp3lame/gain_analysis.c) \
                 $$quote($$BASEDIR/src/mp3lame/id3tag.c) \
                 $$quote($$BASEDIR/src/mp3lame/lame.c) \
                 $$quote($$BASEDIR/src/mp3lame/mpglib_interface.c) \
                 $$quote($$BASEDIR/src/mp3lame/newmdct.c) \
                 $$quote($$BASEDIR/src/mp3lame/presets.c) \
                 $$quote($$BASEDIR/src/mp3lame/psymodel.c) \
                 $$quote($$BASEDIR/src/mp3lame/quantize.c) \
                 $$quote($$BASEDIR/src/mp3lame/quantize_pvt.c) \
                 $$quote($$BASEDIR/src/mp3lame/reservoir.c) \
                 $$quote($$BASEDIR/src/mp3lame/set_get.c) \
                 $$quote($$BASEDIR/src/mp3lame/tables.c) \
                 $$quote($$BASEDIR/src/mp3lame/takehiro.c) \
                 $$quote($$BASEDIR/src/mp3lame/util.c) \
                 $$quote($$BASEDIR/src/mp3lame/vbrquantize.c) \
                 $$quote($$BASEDIR/src/mp3lame/vector/xmm_quantize_sub.c) \
                 $$quote($$BASEDIR/src/mp3lame/version.c) \
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
                 $$quote($$BASEDIR/src/Importer.hpp) \
                 $$quote($$BASEDIR/src/InstanceCounter.hpp) \
                 $$quote($$BASEDIR/src/ItemGroupBase.hpp) \
                 $$quote($$BASEDIR/src/LCDDigits.hpp) \
                 $$quote($$BASEDIR/src/LCDDisplay.hpp) \
                 $$quote($$BASEDIR/src/ModPlayback.hpp) \
                 $$quote($$BASEDIR/src/Mp3Export.hpp) \
                 $$quote($$BASEDIR/src/NamedPlaylist.hpp) \
                 $$quote($$BASEDIR/src/PatternView.hpp) \
                 $$quote($$BASEDIR/src/PlaybackConfig.hpp) \
                 $$quote($$BASEDIR/src/Player.hpp) \
                 $$quote($$BASEDIR/src/Playlist.hpp) \
                 $$quote($$BASEDIR/src/SongBasicInfo.hpp) \
                 $$quote($$BASEDIR/src/SongExtendedInfo.hpp) \
                 $$quote($$BASEDIR/src/SongFormat.hpp) \
                 $$quote($$BASEDIR/src/SongGenre.hpp) \
                 $$quote($$BASEDIR/src/SongModule.hpp) \
                 $$quote($$BASEDIR/src/SuspendPlayback.hpp) \
                 $$quote($$BASEDIR/src/TouchHandler.hpp) \
                 $$quote($$BASEDIR/src/Unpacker.hpp) \
                 $$quote($$BASEDIR/src/VUMeter.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/lcd_fonts.hpp) \
                 $$quote($$BASEDIR/src/libid3tag/id3_compat.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_crc.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_debug.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_field.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_file.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_frame.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_frametype.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_genre.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_global.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_id3tag.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_latin1.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_parse.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_render.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_tag.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_ucs4.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_utf16.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_utf8.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_util.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_version.h) \
                 $$quote($$BASEDIR/src/libid3tag/msvc++/config.h) \
                 $$quote($$BASEDIR/src/libmad/mad.h) \
                 $$quote($$BASEDIR/src/libmad/mad_bit.h) \
                 $$quote($$BASEDIR/src/libmad/mad_decoder.h) \
                 $$quote($$BASEDIR/src/libmad/mad_fixed.h) \
                 $$quote($$BASEDIR/src/libmad/mad_frame.h) \
                 $$quote($$BASEDIR/src/libmad/mad_global.h) \
                 $$quote($$BASEDIR/src/libmad/mad_huffman.h) \
                 $$quote($$BASEDIR/src/libmad/mad_layer12.h) \
                 $$quote($$BASEDIR/src/libmad/mad_layer3.h) \
                 $$quote($$BASEDIR/src/libmad/mad_stream.h) \
                 $$quote($$BASEDIR/src/libmad/mad_synth.h) \
                 $$quote($$BASEDIR/src/libmad/mad_timer.h) \
                 $$quote($$BASEDIR/src/libmad/mad_version.h) \
                 $$quote($$BASEDIR/src/libmad/msvc++/config.h) \
                 $$quote($$BASEDIR/src/libmad/msvc++/mad.h) \
                 $$quote($$BASEDIR/src/modplug/config.h) \
                 $$quote($$BASEDIR/src/modplug/it_defs.h) \
                 $$quote($$BASEDIR/src/modplug/load_pat.h) \
                 $$quote($$BASEDIR/src/modplug/modplug.h) \
                 $$quote($$BASEDIR/src/modplug/sndfile.h) \
                 $$quote($$BASEDIR/src/modplug/stdafx.h) \
                 $$quote($$BASEDIR/src/modplug/tables.h) \
                 $$quote($$BASEDIR/src/mp3lame/VbrTag.h) \
                 $$quote($$BASEDIR/src/mp3lame/bitstream.h) \
                 $$quote($$BASEDIR/src/mp3lame/encoder.h) \
                 $$quote($$BASEDIR/src/mp3lame/fft.h) \
                 $$quote($$BASEDIR/src/mp3lame/gain_analysis.h) \
                 $$quote($$BASEDIR/src/mp3lame/id3tag.h) \
                 $$quote($$BASEDIR/src/mp3lame/l3side.h) \
                 $$quote($$BASEDIR/src/mp3lame/lame-analysis.h) \
                 $$quote($$BASEDIR/src/mp3lame/lame.h) \
                 $$quote($$BASEDIR/src/mp3lame/lame_global_flags.h) \
                 $$quote($$BASEDIR/src/mp3lame/lameerror.h) \
                 $$quote($$BASEDIR/src/mp3lame/machine.h) \
                 $$quote($$BASEDIR/src/mp3lame/newmdct.h) \
                 $$quote($$BASEDIR/src/mp3lame/psymodel.h) \
                 $$quote($$BASEDIR/src/mp3lame/quantize.h) \
                 $$quote($$BASEDIR/src/mp3lame/quantize_pvt.h) \
                 $$quote($$BASEDIR/src/mp3lame/reservoir.h) \
                 $$quote($$BASEDIR/src/mp3lame/set_get.h) \
                 $$quote($$BASEDIR/src/mp3lame/tables.h) \
                 $$quote($$BASEDIR/src/mp3lame/util.h) \
                 $$quote($$BASEDIR/src/mp3lame/vbrquantize.h) \
                 $$quote($$BASEDIR/src/mp3lame/vector/lame_intrin.h) \
                 $$quote($$BASEDIR/src/mp3lame/version.h) \
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
                 $$quote($$BASEDIR/src/ApplicationVersion.cpp) \
                 $$quote($$BASEDIR/src/Artist.cpp) \
                 $$quote($$BASEDIR/src/Cache.cpp) \
                 $$quote($$BASEDIR/src/Canvas.cpp) \
                 $$quote($$BASEDIR/src/Catalog.cpp) \
                 $$quote($$BASEDIR/src/Downloader.cpp) \
                 $$quote($$BASEDIR/src/FileUtils.cpp) \
                 $$quote($$BASEDIR/src/Importer.cpp) \
                 $$quote($$BASEDIR/src/ItemGroupBase.cpp) \
                 $$quote($$BASEDIR/src/LCDDigits.cpp) \
                 $$quote($$BASEDIR/src/LCDDisplay.cpp) \
                 $$quote($$BASEDIR/src/ModPlayback.cpp) \
                 $$quote($$BASEDIR/src/Mp3Export.cpp) \
                 $$quote($$BASEDIR/src/NamedPlaylist.cpp) \
                 $$quote($$BASEDIR/src/PatternView.cpp) \
                 $$quote($$BASEDIR/src/PlaybackConfig.cpp) \
                 $$quote($$BASEDIR/src/Player.cpp) \
                 $$quote($$BASEDIR/src/Playlist.cpp) \
                 $$quote($$BASEDIR/src/SongBasicInfo.cpp) \
                 $$quote($$BASEDIR/src/SongExtendedInfo.cpp) \
                 $$quote($$BASEDIR/src/SongFormat.cpp) \
                 $$quote($$BASEDIR/src/SongGenre.cpp) \
                 $$quote($$BASEDIR/src/SongModule.cpp) \
                 $$quote($$BASEDIR/src/SuspendPlayback.cpp) \
                 $$quote($$BASEDIR/src/TouchHandler.cpp) \
                 $$quote($$BASEDIR/src/Unpacker.cpp) \
                 $$quote($$BASEDIR/src/VUMeter.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/lcd_font_6x8.cpp) \
                 $$quote($$BASEDIR/src/libid3tag/id3_compat.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_crc.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_debug.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_field.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_file.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_frame.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_frametype.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_genre.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_latin1.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_parse.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_render.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_tag.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_ucs4.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_utf16.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_utf8.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_util.c) \
                 $$quote($$BASEDIR/src/libid3tag/id3_version.c) \
                 $$quote($$BASEDIR/src/libmad/mad_bit.c) \
                 $$quote($$BASEDIR/src/libmad/mad_decoder.c) \
                 $$quote($$BASEDIR/src/libmad/mad_fixed.c) \
                 $$quote($$BASEDIR/src/libmad/mad_frame.c) \
                 $$quote($$BASEDIR/src/libmad/mad_huffman.c) \
                 $$quote($$BASEDIR/src/libmad/mad_layer12.c) \
                 $$quote($$BASEDIR/src/libmad/mad_layer3.c) \
                 $$quote($$BASEDIR/src/libmad/mad_minimad.c) \
                 $$quote($$BASEDIR/src/libmad/mad_stream.c) \
                 $$quote($$BASEDIR/src/libmad/mad_synth.c) \
                 $$quote($$BASEDIR/src/libmad/mad_timer.c) \
                 $$quote($$BASEDIR/src/libmad/mad_version.c) \
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
                 $$quote($$BASEDIR/src/mp3lame/VbrTag.c) \
                 $$quote($$BASEDIR/src/mp3lame/bitstream.c) \
                 $$quote($$BASEDIR/src/mp3lame/encoder.c) \
                 $$quote($$BASEDIR/src/mp3lame/fft.c) \
                 $$quote($$BASEDIR/src/mp3lame/gain_analysis.c) \
                 $$quote($$BASEDIR/src/mp3lame/id3tag.c) \
                 $$quote($$BASEDIR/src/mp3lame/lame.c) \
                 $$quote($$BASEDIR/src/mp3lame/mpglib_interface.c) \
                 $$quote($$BASEDIR/src/mp3lame/newmdct.c) \
                 $$quote($$BASEDIR/src/mp3lame/presets.c) \
                 $$quote($$BASEDIR/src/mp3lame/psymodel.c) \
                 $$quote($$BASEDIR/src/mp3lame/quantize.c) \
                 $$quote($$BASEDIR/src/mp3lame/quantize_pvt.c) \
                 $$quote($$BASEDIR/src/mp3lame/reservoir.c) \
                 $$quote($$BASEDIR/src/mp3lame/set_get.c) \
                 $$quote($$BASEDIR/src/mp3lame/tables.c) \
                 $$quote($$BASEDIR/src/mp3lame/takehiro.c) \
                 $$quote($$BASEDIR/src/mp3lame/util.c) \
                 $$quote($$BASEDIR/src/mp3lame/vbrquantize.c) \
                 $$quote($$BASEDIR/src/mp3lame/vector/xmm_quantize_sub.c) \
                 $$quote($$BASEDIR/src/mp3lame/version.c) \
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
                 $$quote($$BASEDIR/src/Importer.hpp) \
                 $$quote($$BASEDIR/src/InstanceCounter.hpp) \
                 $$quote($$BASEDIR/src/ItemGroupBase.hpp) \
                 $$quote($$BASEDIR/src/LCDDigits.hpp) \
                 $$quote($$BASEDIR/src/LCDDisplay.hpp) \
                 $$quote($$BASEDIR/src/ModPlayback.hpp) \
                 $$quote($$BASEDIR/src/Mp3Export.hpp) \
                 $$quote($$BASEDIR/src/NamedPlaylist.hpp) \
                 $$quote($$BASEDIR/src/PatternView.hpp) \
                 $$quote($$BASEDIR/src/PlaybackConfig.hpp) \
                 $$quote($$BASEDIR/src/Player.hpp) \
                 $$quote($$BASEDIR/src/Playlist.hpp) \
                 $$quote($$BASEDIR/src/SongBasicInfo.hpp) \
                 $$quote($$BASEDIR/src/SongExtendedInfo.hpp) \
                 $$quote($$BASEDIR/src/SongFormat.hpp) \
                 $$quote($$BASEDIR/src/SongGenre.hpp) \
                 $$quote($$BASEDIR/src/SongModule.hpp) \
                 $$quote($$BASEDIR/src/SuspendPlayback.hpp) \
                 $$quote($$BASEDIR/src/TouchHandler.hpp) \
                 $$quote($$BASEDIR/src/Unpacker.hpp) \
                 $$quote($$BASEDIR/src/VUMeter.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp) \
                 $$quote($$BASEDIR/src/lcd_fonts.hpp) \
                 $$quote($$BASEDIR/src/libid3tag/id3_compat.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_crc.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_debug.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_field.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_file.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_frame.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_frametype.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_genre.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_global.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_id3tag.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_latin1.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_parse.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_render.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_tag.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_ucs4.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_utf16.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_utf8.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_util.h) \
                 $$quote($$BASEDIR/src/libid3tag/id3_version.h) \
                 $$quote($$BASEDIR/src/libid3tag/msvc++/config.h) \
                 $$quote($$BASEDIR/src/libmad/mad.h) \
                 $$quote($$BASEDIR/src/libmad/mad_bit.h) \
                 $$quote($$BASEDIR/src/libmad/mad_decoder.h) \
                 $$quote($$BASEDIR/src/libmad/mad_fixed.h) \
                 $$quote($$BASEDIR/src/libmad/mad_frame.h) \
                 $$quote($$BASEDIR/src/libmad/mad_global.h) \
                 $$quote($$BASEDIR/src/libmad/mad_huffman.h) \
                 $$quote($$BASEDIR/src/libmad/mad_layer12.h) \
                 $$quote($$BASEDIR/src/libmad/mad_layer3.h) \
                 $$quote($$BASEDIR/src/libmad/mad_stream.h) \
                 $$quote($$BASEDIR/src/libmad/mad_synth.h) \
                 $$quote($$BASEDIR/src/libmad/mad_timer.h) \
                 $$quote($$BASEDIR/src/libmad/mad_version.h) \
                 $$quote($$BASEDIR/src/libmad/msvc++/config.h) \
                 $$quote($$BASEDIR/src/libmad/msvc++/mad.h) \
                 $$quote($$BASEDIR/src/modplug/config.h) \
                 $$quote($$BASEDIR/src/modplug/it_defs.h) \
                 $$quote($$BASEDIR/src/modplug/load_pat.h) \
                 $$quote($$BASEDIR/src/modplug/modplug.h) \
                 $$quote($$BASEDIR/src/modplug/sndfile.h) \
                 $$quote($$BASEDIR/src/modplug/stdafx.h) \
                 $$quote($$BASEDIR/src/modplug/tables.h) \
                 $$quote($$BASEDIR/src/mp3lame/VbrTag.h) \
                 $$quote($$BASEDIR/src/mp3lame/bitstream.h) \
                 $$quote($$BASEDIR/src/mp3lame/encoder.h) \
                 $$quote($$BASEDIR/src/mp3lame/fft.h) \
                 $$quote($$BASEDIR/src/mp3lame/gain_analysis.h) \
                 $$quote($$BASEDIR/src/mp3lame/id3tag.h) \
                 $$quote($$BASEDIR/src/mp3lame/l3side.h) \
                 $$quote($$BASEDIR/src/mp3lame/lame-analysis.h) \
                 $$quote($$BASEDIR/src/mp3lame/lame.h) \
                 $$quote($$BASEDIR/src/mp3lame/lame_global_flags.h) \
                 $$quote($$BASEDIR/src/mp3lame/lameerror.h) \
                 $$quote($$BASEDIR/src/mp3lame/machine.h) \
                 $$quote($$BASEDIR/src/mp3lame/newmdct.h) \
                 $$quote($$BASEDIR/src/mp3lame/psymodel.h) \
                 $$quote($$BASEDIR/src/mp3lame/quantize.h) \
                 $$quote($$BASEDIR/src/mp3lame/quantize_pvt.h) \
                 $$quote($$BASEDIR/src/mp3lame/reservoir.h) \
                 $$quote($$BASEDIR/src/mp3lame/set_get.h) \
                 $$quote($$BASEDIR/src/mp3lame/tables.h) \
                 $$quote($$BASEDIR/src/mp3lame/util.h) \
                 $$quote($$BASEDIR/src/mp3lame/vbrquantize.h) \
                 $$quote($$BASEDIR/src/mp3lame/vector/lame_intrin.h) \
                 $$quote($$BASEDIR/src/mp3lame/version.h) \
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

INCLUDEPATH +=  $$quote($$BASEDIR/src/libid3tag) \
         $$quote($$BASEDIR/src/Flurry) \
         $$quote($$BASEDIR/src/zip) \
         $$quote($$BASEDIR/src/mp3lame/vector) \
         $$quote($$BASEDIR/src/mp3lame) \
         $$quote($$BASEDIR/src/libmad/msvc++) \
         $$quote($$BASEDIR/src/libmad) \
         $$quote($$BASEDIR/src/modplug) \
         $$quote($$BASEDIR/src/libid3tag/msvc++) \
         $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../src/Flurry/*.c) \
             $$quote($$BASEDIR/../src/Flurry/*.c++) \
             $$quote($$BASEDIR/../src/Flurry/*.cc) \
             $$quote($$BASEDIR/../src/Flurry/*.cpp) \
             $$quote($$BASEDIR/../src/Flurry/*.cxx) \
             $$quote($$BASEDIR/../src/libid3tag/*.c) \
             $$quote($$BASEDIR/../src/libid3tag/*.c++) \
             $$quote($$BASEDIR/../src/libid3tag/*.cc) \
             $$quote($$BASEDIR/../src/libid3tag/*.cpp) \
             $$quote($$BASEDIR/../src/libid3tag/*.cxx) \
             $$quote($$BASEDIR/../src/libid3tag/msvc++/*.c) \
             $$quote($$BASEDIR/../src/libid3tag/msvc++/*.c++) \
             $$quote($$BASEDIR/../src/libid3tag/msvc++/*.cc) \
             $$quote($$BASEDIR/../src/libid3tag/msvc++/*.cpp) \
             $$quote($$BASEDIR/../src/libid3tag/msvc++/*.cxx) \
             $$quote($$BASEDIR/../src/libmad/*.c) \
             $$quote($$BASEDIR/../src/libmad/*.c++) \
             $$quote($$BASEDIR/../src/libmad/*.cc) \
             $$quote($$BASEDIR/../src/libmad/*.cpp) \
             $$quote($$BASEDIR/../src/libmad/*.cxx) \
             $$quote($$BASEDIR/../src/libmad/msvc++/*.c) \
             $$quote($$BASEDIR/../src/libmad/msvc++/*.c++) \
             $$quote($$BASEDIR/../src/libmad/msvc++/*.cc) \
             $$quote($$BASEDIR/../src/libmad/msvc++/*.cpp) \
             $$quote($$BASEDIR/../src/libmad/msvc++/*.cxx) \
             $$quote($$BASEDIR/../src/modplug/*.c) \
             $$quote($$BASEDIR/../src/modplug/*.c++) \
             $$quote($$BASEDIR/../src/modplug/*.cc) \
             $$quote($$BASEDIR/../src/modplug/*.cpp) \
             $$quote($$BASEDIR/../src/modplug/*.cxx) \
             $$quote($$BASEDIR/../src/mp3lame/*.c) \
             $$quote($$BASEDIR/../src/mp3lame/*.c++) \
             $$quote($$BASEDIR/../src/mp3lame/*.cc) \
             $$quote($$BASEDIR/../src/mp3lame/*.cpp) \
             $$quote($$BASEDIR/../src/mp3lame/*.cxx) \
             $$quote($$BASEDIR/../src/mp3lame/vector/*.c) \
             $$quote($$BASEDIR/../src/mp3lame/vector/*.c++) \
             $$quote($$BASEDIR/../src/mp3lame/vector/*.cc) \
             $$quote($$BASEDIR/../src/mp3lame/vector/*.cpp) \
             $$quote($$BASEDIR/../src/mp3lame/vector/*.cxx) \
             $$quote($$BASEDIR/../src/zip/*.c) \
             $$quote($$BASEDIR/../src/zip/*.c++) \
             $$quote($$BASEDIR/../src/zip/*.cc) \
             $$quote($$BASEDIR/../src/zip/*.cpp) \
             $$quote($$BASEDIR/../src/zip/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs) \
             $$quote($$BASEDIR/../assets/images/*.qml) \
             $$quote($$BASEDIR/../assets/images/*.js) \
             $$quote($$BASEDIR/../assets/images/*.qs) \
             $$quote($$BASEDIR/../assets/images/actions/*.qml) \
             $$quote($$BASEDIR/../assets/images/actions/*.js) \
             $$quote($$BASEDIR/../assets/images/actions/*.qs) \
             $$quote($$BASEDIR/../assets/images/backgrounds/*.qml) \
             $$quote($$BASEDIR/../assets/images/backgrounds/*.js) \
             $$quote($$BASEDIR/../assets/images/backgrounds/*.qs) \
             $$quote($$BASEDIR/../assets/images/badges/*.qml) \
             $$quote($$BASEDIR/../assets/images/badges/*.js) \
             $$quote($$BASEDIR/../assets/images/badges/*.qs) \
             $$quote($$BASEDIR/../assets/images/cover/*.qml) \
             $$quote($$BASEDIR/../assets/images/cover/*.js) \
             $$quote($$BASEDIR/../assets/images/cover/*.qs) \
             $$quote($$BASEDIR/../assets/images/formats/*.qml) \
             $$quote($$BASEDIR/../assets/images/formats/*.js) \
             $$quote($$BASEDIR/../assets/images/formats/*.qs) \
             $$quote($$BASEDIR/../assets/images/lcd/*.qml) \
             $$quote($$BASEDIR/../assets/images/lcd/*.js) \
             $$quote($$BASEDIR/../assets/images/lcd/*.qs) \
             $$quote($$BASEDIR/../assets/images/led/*.qml) \
             $$quote($$BASEDIR/../assets/images/led/*.js) \
             $$quote($$BASEDIR/../assets/images/led/*.qs) \
             $$quote($$BASEDIR/../assets/images/vu/*.qml) \
             $$quote($$BASEDIR/../assets/images/vu/*.js) \
             $$quote($$BASEDIR/../assets/images/vu/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}_ru.ts) \
         $$quote($${TARGET}.ts)
