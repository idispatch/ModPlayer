#include "PlaybackConfig.hpp"

PlaybackConfig::PlaybackConfig(QObject *parent)
    : QObject(parent),
      m_bStereo(true),
      m_frequency(44100),
      m_sampleSize(16),
      m_resamplingMode(2),
      m_stereoSeparation(128),
      m_maximumMixingChannels(128),
      m_reverbEnabled(true),
      m_reverbLevel(50),
      m_reverbDelay(128),
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

PlaybackConfig& PlaybackConfig::operator = (PlaybackConfig const& other) {
    if(this != &other) {
        setStereo(other.stereo());
        setFrequency(other.frequency());
        setSampleSize(other.sampleSize());

        setResamplingMode(other.resamplingMode());
        setStereoSeparation(other.stereoSeparation());
        setMaximumMixingChannels(other.maximumMixingChannels());

        setReverbEnabled(other.reverbEnabled());
        setReverbDelay(other.reverbDelay());
        setReverbLevel(other.reverbLevel());

        setBassEnabled(other.bassEnabled());
        setBassLevel(other.bassLevel());
        setBassCutOff(other.bassCutOff());

        setSurroundEnabled(other.surroundEnabled());
        setSurroundDelay(other.surroundDelay());
        setSurroundLevel(other.surroundLevel());

        setOversamplingEnabled(other.oversamplingEnabled());
        setNoiseReductionEnabled(other.noiseReductionEnabled());
    }
    return *this;
}

bool PlaybackConfig::operator == (PlaybackConfig const& other) {
    return stereo() == other.stereo() &&
           sampleSize() == other.sampleSize() &&
           frequency() == other.frequency() &&

           resamplingMode() == other.resamplingMode() &&
           stereoSeparation() == other.stereoSeparation() &&
           maximumMixingChannels() == other.maximumMixingChannels() &&

           reverbEnabled() == other.reverbEnabled() &&
           reverbDelay() == other.reverbDelay() &&
           reverbLevel() == other.reverbLevel() &&

           bassEnabled() == other.bassEnabled() &&
           bassLevel() == other.bassLevel() &&
           bassCutOff() == other.bassCutOff() &&

           surroundEnabled() == other.surroundEnabled() &&
           surroundDelay() == other.surroundDelay() &&
           surroundLevel() == other.surroundLevel() &&

           oversamplingEnabled() == other.oversamplingEnabled() &&
           noiseReductionEnabled() == other.noiseReductionEnabled();
}
bool PlaybackConfig::operator != (PlaybackConfig const& other) {
    return !(*this == other);
}

bool PlaybackConfig::stereo() const {
    return m_bStereo;
}

void PlaybackConfig::setStereo(bool value) {
    if(m_bStereo != value) {
        m_bStereo = value;
        emit stereoChanged();
    }
}

int PlaybackConfig::frequency() const {
    return m_frequency;
}

void PlaybackConfig::setFrequency(int value) {
    if(m_frequency != value) {
        m_frequency = value;
        emit frequencyChanged();
    }
}

int PlaybackConfig::sampleSize() const {
    return m_sampleSize;
}

void PlaybackConfig::setSampleSize(int value) {
    if(m_sampleSize != value) {
        m_sampleSize = value;
        emit sampleSizeChanged();
    }
}

int PlaybackConfig::resamplingMode() const {
    return m_resamplingMode;
}

void PlaybackConfig::setResamplingMode(int value) {
    if(m_resamplingMode != value) {
        m_resamplingMode = value;
        emit resamplingModeChanged();
    }
}

int PlaybackConfig::stereoSeparation() const {
    return m_stereoSeparation;
}

void PlaybackConfig::setStereoSeparation(int value) {
    if(m_stereoSeparation != value) {
        m_stereoSeparation = value;
        emit stereoSeparationChanged();
    }
}

int PlaybackConfig::maximumMixingChannels() const {
    return m_maximumMixingChannels;
}

void PlaybackConfig::setMaximumMixingChannels(int value) {
    if(m_maximumMixingChannels != value) {
        m_maximumMixingChannels = value;
        emit maximumMixingChannelsChanged();
    }
}

bool PlaybackConfig::reverbEnabled() const {
    return m_reverbEnabled;
}

void PlaybackConfig::setReverbEnabled(bool value) {
    if(m_reverbEnabled != value) {
        m_reverbEnabled = value;
        emit reverbEnabledChanged();
    }
}

int PlaybackConfig::reverbLevel() const {
    return m_reverbLevel;
}

void PlaybackConfig::setReverbLevel(int value) {
    if(m_reverbLevel != value) {
        m_reverbLevel = value;
        emit reverbEnabledChanged();
    }
}

int PlaybackConfig::reverbDelay() const {
    return m_reverbDelay;
}

void PlaybackConfig::setReverbDelay(int value) {
    if(m_reverbDelay != value) {
        m_reverbDelay = value;
        emit reverbDelayChanged();
    }
}

bool PlaybackConfig::bassEnabled() const {
    return m_bassEnabled;
}

void PlaybackConfig::setBassEnabled(bool value) {
    if(m_bassEnabled != value) {
        m_bassEnabled = value;
        emit bassEnabledChanged();
    }
}

int PlaybackConfig::bassLevel() const {
    return m_bassLevel;
}

void PlaybackConfig::setBassLevel(int value) {
    if(m_bassLevel != value) {
        m_bassLevel = value;
        emit bassLevelChanged();
    }
}

int PlaybackConfig::bassCutOff() const {
    return m_bassCutoff;
}

void PlaybackConfig::setBassCutOff(int value) {
    if(m_bassCutoff != value) {
        m_bassCutoff = value;
        emit bassCutOffChanged();
    }
}

bool PlaybackConfig::surroundEnabled() const {
    return m_surroundEnabled;
}
void PlaybackConfig::setSurroundEnabled(bool value) {
    if(m_surroundEnabled != value) {
        m_surroundEnabled = value;
        emit surroundEnabledChanged();
    }
}

int PlaybackConfig::surroundLevel() const {
    return m_surroundLevel;
}

void PlaybackConfig::setSurroundLevel(int value) {
    if(m_surroundLevel != value) {
        m_surroundLevel = value;
        emit surroundLevelChanged();
    }
}

int PlaybackConfig::surroundDelay() const {
    return m_surroundDelay;
}

void PlaybackConfig::setSurroundDelay(int value) {
    if(m_surroundDelay != value) {
        m_surroundDelay = value;
        emit surroundDelayChanged();
    }
}

bool PlaybackConfig::oversamplingEnabled() const {
    return m_oversamplingEnabled;
}

void PlaybackConfig::setOversamplingEnabled(bool value) {
    if(m_oversamplingEnabled != value) {
        m_oversamplingEnabled = value;
        emit oversamplingEnabledChanged();
    }
}

bool PlaybackConfig::noiseReductionEnabled() const {
    return m_noiseReductionEnabled;
}

void PlaybackConfig::setNoiseReductionEnabled(bool value) {
    if(m_noiseReductionEnabled != value) {
        m_noiseReductionEnabled = value;
        emit noiseReductionEnabledChanged();
    }
}

QDebug operator << (QDebug dbg, PlaybackConfig const &) {
    return dbg.space();
}
