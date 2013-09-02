#include "PlaybackConfig.hpp"
#include "modplug.h"

template<>
int InstanceCounter<PlaybackConfig>::s_count;
template<>
int InstanceCounter<PlaybackConfig>::s_maxCount;

PlaybackConfig::PlaybackConfig(QObject *parent)
    : QObject(parent),
      m_bStereo(true),
      m_frequency(44100),
      m_sampleSize(16),
      m_resamplingMode(3),
      m_stereoSeparation(128),
      m_maximumMixingChannels(128),
      m_reverbEnabled(true),
      m_reverbLevel(20),
      m_reverbDelay(50),
      m_bassEnabled(true),
      m_bassLevel(50),
      m_bassCutoff(50),
      m_surroundEnabled(true),
      m_surroundLevel(50),
      m_surroundDelay(20),
      m_oversamplingEnabled(true),
      m_noiseReductionEnabled(true) {
}

PlaybackConfig::~PlaybackConfig() {
}

bool PlaybackConfig::isAudioReconfigurationRequired() const {
    // All options will take effect immediately, except for:
    //   * channels
    //   * bits-per-sample
    //   * sampling rate
    //   * loop count
    // Those options which don't take effect immediately will take effect
    // the next time you load a song
    ModPlug_Settings settings;
    ModPlug_GetSettings(&settings);
    return settings.mChannels != (stereo() ? 2 : 1) ||
           settings.mBits != sampleSize() ||
           settings.mFrequency != frequency();
}

void PlaybackConfig::configureModPlug() {
    ModPlug_Settings settings;
    memset(&settings, 0, sizeof(settings));

    settings.mFlags = 0;
    if(oversamplingEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_OVERSAMPLING;
    }
    if(noiseReductionEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_NOISE_REDUCTION;
    }
    if(reverbEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_REVERB;
    }
    if(bassEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_MEGABASS;
    }
    if(surroundEnabled()) {
        settings.mFlags |= MODPLUG_ENABLE_SURROUND;
    }

    settings.mChannels = stereo() ? 2 : 1;
    settings.mBits = sampleSize();
    settings.mFrequency = frequency();
    settings.mResamplingMode = resamplingMode();

    settings.mStereoSeparation = stereoSeparation();
    settings.mMaxMixChannels = maximumMixingChannels();

    settings.mReverbDepth = reverbLevel();
    settings.mReverbDelay = reverbDelay();

    settings.mBassAmount = bassLevel();
    settings.mBassRange = bassCutOff();

    settings.mSurroundDepth = surroundLevel();
    settings.mSurroundDelay = surroundDelay();

    settings.mLoopCount = 0;

    ModPlug_SetSettings(&settings);
}

bool PlaybackConfig::stereo() const {
    return m_bStereo;
}

void PlaybackConfig::setStereo(bool value) {
    if(m_bStereo != value) {
        m_bStereo = value;
        // Not applying immediately
        emit stereoChanged();
    }
}

int PlaybackConfig::frequency() const {
    return m_frequency;
}

void PlaybackConfig::setFrequency(int value) {
    if(!(value == 11025 || value == 22050 || value == 44100))
        return;
    if(m_frequency != value) {
        m_frequency = value;
        // Not applying immediately
        emit frequencyChanged();
    }
}

int PlaybackConfig::sampleSize() const {
    return m_sampleSize;
}

void PlaybackConfig::setSampleSize(int value) {
    if(!(value == 8 || value == 16 || value == 32))
        return;
    if(m_sampleSize != value) {
        m_sampleSize = value;
        // Not applying immediately
        emit sampleSizeChanged();
    }
}

int PlaybackConfig::resamplingMode() const {
    return m_resamplingMode;
}

void PlaybackConfig::setResamplingMode(int value) {
    if(value < 0 || value > 3)
        return;
    if(m_resamplingMode != value) {
        m_resamplingMode = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        settings.mResamplingMode = resamplingMode();
        ModPlug_SetSettings(&settings);

        emit resamplingModeChanged();
    }
}

int PlaybackConfig::stereoSeparation() const {
    return m_stereoSeparation;
}

void PlaybackConfig::setStereoSeparation(int value) {
    if(value < 1) value = 1;
    if(value > 256) value = 256;
    if(m_stereoSeparation != value) {
        m_stereoSeparation = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        settings.mStereoSeparation = stereoSeparation();
        ModPlug_SetSettings(&settings);

        emit stereoSeparationChanged();
    }
}

int PlaybackConfig::maximumMixingChannels() const {
    return m_maximumMixingChannels;
}

void PlaybackConfig::setMaximumMixingChannels(int value) {
    if(value < 32) value = 32;
    if(value > 256) value = 256;
    if(m_maximumMixingChannels != value) {
        m_maximumMixingChannels = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        settings.mMaxMixChannels = maximumMixingChannels();
        ModPlug_SetSettings(&settings);

        emit maximumMixingChannelsChanged();
    }
}

bool PlaybackConfig::reverbEnabled() const {
    return m_reverbEnabled;
}

void PlaybackConfig::setReverbEnabled(bool value) {
    if(m_reverbEnabled != value) {
        m_reverbEnabled = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        if(reverbEnabled()) {
            settings.mFlags |= MODPLUG_ENABLE_REVERB;
        } else {
            settings.mFlags &= ~MODPLUG_ENABLE_REVERB;
        }
        ModPlug_SetSettings(&settings);

        emit reverbEnabledChanged();
    }
}

int PlaybackConfig::reverbLevel() const {
    return m_reverbLevel;
}

void PlaybackConfig::setReverbLevel(int value) {
    if(value < 0) value = 0;
    if(value > 100) value = 100;
    if(m_reverbLevel != value) {
        m_reverbLevel = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        settings.mReverbDepth = reverbLevel();
        ModPlug_SetSettings(&settings);

        emit reverbEnabledChanged();
    }
}

int PlaybackConfig::reverbDelay() const {
    return m_reverbDelay;
}

void PlaybackConfig::setReverbDelay(int value) {
    if(value < 0) value = 0;
    if(value > 400) value = 400;
    if(m_reverbDelay != value) {
        m_reverbDelay = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        settings.mReverbDelay = reverbDelay();
        ModPlug_SetSettings(&settings);

        emit reverbDelayChanged();
    }
}

bool PlaybackConfig::bassEnabled() const {
    return m_bassEnabled;
}

void PlaybackConfig::setBassEnabled(bool value) {
    if(m_bassEnabled != value) {
        m_bassEnabled = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        if(bassEnabled()) {
            settings.mFlags |= MODPLUG_ENABLE_MEGABASS;
        } else {
            settings.mFlags &= ~MODPLUG_ENABLE_MEGABASS;
        }
        ModPlug_SetSettings(&settings);

        emit bassEnabledChanged();
    }
}

int PlaybackConfig::bassLevel() const {
    return m_bassLevel;
}

void PlaybackConfig::setBassLevel(int value) {
    if(value < 0) value = 0;
    if(value > 100) value = 100;
    if(m_bassLevel != value) {
        m_bassLevel = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        settings.mBassAmount = bassLevel();
        ModPlug_SetSettings(&settings);

        emit bassLevelChanged();
    }
}

int PlaybackConfig::bassCutOff() const {
    return m_bassCutoff;
}

void PlaybackConfig::setBassCutOff(int value) {
    if(value < 10) value = 10;
    if(value > 100) value = 100;
    if(m_bassCutoff != value) {
        m_bassCutoff = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        settings.mBassRange = bassCutOff();
        ModPlug_SetSettings(&settings);

        emit bassCutOffChanged();
    }
}

bool PlaybackConfig::surroundEnabled() const {
    return m_surroundEnabled;
}
void PlaybackConfig::setSurroundEnabled(bool value) {
    if(m_surroundEnabled != value) {
        m_surroundEnabled = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        if(bassEnabled()) {
            settings.mFlags |= MODPLUG_ENABLE_SURROUND;
        } else {
            settings.mFlags &= ~MODPLUG_ENABLE_SURROUND;
        }
        ModPlug_SetSettings(&settings);

        emit surroundEnabledChanged();
    }
}

int PlaybackConfig::surroundLevel() const {
    return m_surroundLevel;
}

void PlaybackConfig::setSurroundLevel(int value) {
    if(value < 0) value = 0;
    if(value > 100) value = 100;
    if(m_surroundLevel != value) {
        m_surroundLevel = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        settings.mSurroundDepth = surroundLevel();
        ModPlug_SetSettings(&settings);

        emit surroundLevelChanged();
    }
}

int PlaybackConfig::surroundDelay() const {
    return m_surroundDelay;
}

void PlaybackConfig::setSurroundDelay(int value) {
    if(value < 5) value = 5;
    if(value > 100) value = 100;
    if(m_surroundDelay != value) {
        m_surroundDelay = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        settings.mSurroundDelay = surroundDelay();
        ModPlug_SetSettings(&settings);

        emit surroundDelayChanged();
    }
}

bool PlaybackConfig::oversamplingEnabled() const {
    return m_oversamplingEnabled;
}

void PlaybackConfig::setOversamplingEnabled(bool value) {
    if(m_oversamplingEnabled != value) {
        m_oversamplingEnabled = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        if(bassEnabled()) {
            settings.mFlags |= MODPLUG_ENABLE_OVERSAMPLING;
        } else {
            settings.mFlags &= ~MODPLUG_ENABLE_OVERSAMPLING;
        }
        ModPlug_SetSettings(&settings);

        emit oversamplingEnabledChanged();
    }
}

bool PlaybackConfig::noiseReductionEnabled() const {
    return m_noiseReductionEnabled;
}

void PlaybackConfig::setNoiseReductionEnabled(bool value) {
    if(m_noiseReductionEnabled != value) {
        m_noiseReductionEnabled = value;

        ModPlug_Settings settings;
        ModPlug_GetSettings(&settings);
        if(bassEnabled()) {
            settings.mFlags |= MODPLUG_ENABLE_NOISE_REDUCTION;
        } else {
            settings.mFlags &= ~MODPLUG_ENABLE_NOISE_REDUCTION;
        }
        ModPlug_SetSettings(&settings);

        emit noiseReductionEnabledChanged();
    }
}

QDebug operator << (QDebug dbg, PlaybackConfig const & config) {
    dbg.nospace() << "(PlaybackConfig: "
            << "stereo=" << config.stereo()
            << ", sampleSize=" << config.sampleSize()
            << ", frequency=" << config.frequency()
            << ", resampling=" << config.resamplingMode()
            << ", stereoSeparation=" << config.stereoSeparation()
            << ", maximumChannels=" << config.maximumMixingChannels()
            << ", oversampling=" << config.oversamplingEnabled()
            << ", noiseReduction=" << config.noiseReductionEnabled()
            << ", reverbEnabled=" << config.reverbEnabled()
            << ", reverbLevel=" << config.reverbLevel()
            << ", reverbDelay=" << config.reverbDelay()
            << ", bassEnabled=" << config.bassEnabled()
            << ", bassLevel=" << config.bassLevel()
            << ", bassCutOff=" << config.bassCutOff()
            << ", surroundEnabled=" << config.surroundEnabled()
            << ", surroundLevel=" << config.surroundLevel()
            << ", surroundDelay=" << config.surroundDelay()
            << ")";
    return dbg.space();
}
