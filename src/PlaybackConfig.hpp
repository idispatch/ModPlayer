#ifndef PLAYBACKCONFIG_HPP_
#define PLAYBACKCONFIG_HPP_

#include <QDebug>
#include <QMetaType>
#include <QObject>
#include "InstanceCounter.hpp"

class PlaybackConfig : public QObject,
                       public InstanceCounter<PlaybackConfig> {
    Q_OBJECT

    Q_PROPERTY(bool stereo READ stereo WRITE setStereo NOTIFY stereoChanged FINAL)
    Q_PROPERTY(int frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged FINAL)
    Q_PROPERTY(int sampleSize READ sampleSize WRITE setSampleSize NOTIFY sampleSizeChanged FINAL)

    Q_PROPERTY(int resamplingMode READ resamplingMode WRITE setResamplingMode NOTIFY resamplingModeChanged FINAL)
    Q_PROPERTY(int stereoSeparation READ stereoSeparation WRITE setStereoSeparation NOTIFY stereoSeparationChanged FINAL)
    Q_PROPERTY(int maximumMixingChannels READ maximumMixingChannels WRITE setMaximumMixingChannels NOTIFY maximumMixingChannelsChanged FINAL)

    Q_PROPERTY(bool reverbEnabled READ reverbEnabled WRITE setReverbEnabled NOTIFY reverbEnabledChanged FINAL)
    Q_PROPERTY(int reverbLevel READ reverbLevel WRITE setReverbLevel NOTIFY reverbLevelChanged FINAL)
    Q_PROPERTY(int reverbDelay READ reverbDelay WRITE setReverbDelay NOTIFY reverbDelayChanged FINAL)

    Q_PROPERTY(bool bassEnabled READ bassEnabled WRITE setBassEnabled NOTIFY bassEnabledChanged FINAL)
    Q_PROPERTY(int bassLevel READ bassLevel WRITE setBassLevel NOTIFY bassLevelChanged FINAL)
    Q_PROPERTY(int bassCutOff READ bassCutOff WRITE setBassCutOff NOTIFY bassCutOffChanged FINAL)

    Q_PROPERTY(bool surroundEnabled READ surroundEnabled WRITE setSurroundEnabled NOTIFY surroundEnabledChanged FINAL)
    Q_PROPERTY(int surroundLevel READ surroundLevel WRITE setSurroundLevel NOTIFY surroundLevelChanged FINAL)
    Q_PROPERTY(int surroundDelay READ surroundDelay WRITE setSurroundDelay NOTIFY surroundDelayChanged FINAL)

    Q_PROPERTY(bool oversamplingEnabled READ oversamplingEnabled WRITE setOversamplingEnabled NOTIFY oversamplingEnabledChanged FINAL)
    Q_PROPERTY(bool noiseReductionEnabled READ noiseReductionEnabled WRITE setNoiseReductionEnabled NOTIFY noiseReductionEnabledChanged FINAL)
public:
    PlaybackConfig(QObject *parent);
    ~PlaybackConfig();

    bool isAudioReconfigurationRequired() const;
    void configureModPlug();

    bool stereo() const;
    void setStereo(bool);

    int frequency() const;
    void setFrequency(int);

    int sampleSize() const;
    void setSampleSize(int);

    int resamplingMode() const;
    void setResamplingMode(int);

    int stereoSeparation() const;
    void setStereoSeparation(int);

    int maximumMixingChannels() const;
    void setMaximumMixingChannels(int);

    bool reverbEnabled() const;
    void setReverbEnabled(bool);

    int reverbLevel() const;
    void setReverbLevel(int);

    int reverbDelay() const;
    void setReverbDelay(int);

    bool bassEnabled() const;
    void setBassEnabled(bool);

    int bassLevel() const;
    void setBassLevel(int);

    int bassCutOff() const;
    void setBassCutOff(int);

    bool surroundEnabled() const;
    void setSurroundEnabled(bool);

    int surroundLevel() const;
    void setSurroundLevel(int);

    int surroundDelay() const;
    void setSurroundDelay(int);

    bool oversamplingEnabled() const;
    void setOversamplingEnabled(bool);

    bool noiseReductionEnabled() const;
    void setNoiseReductionEnabled(bool);

    using InstanceCounter<PlaybackConfig>::getInstanceCount;
    using InstanceCounter<PlaybackConfig>::getMaxInstanceCount;
Q_SIGNALS:
    void stereoChanged();
    void frequencyChanged();
    void sampleSizeChanged();

    void resamplingModeChanged();
    void stereoSeparationChanged();
    void maximumMixingChannelsChanged();

    void reverbEnabledChanged();
    void reverbLevelChanged();
    void reverbDelayChanged();

    void bassEnabledChanged();
    void bassLevelChanged();
    void bassCutOffChanged();

    void surroundEnabledChanged();
    void surroundLevelChanged();
    void surroundDelayChanged();

    void oversamplingEnabledChanged();
    void noiseReductionEnabledChanged();
private:
    Q_DISABLE_COPY(PlaybackConfig)
    bool m_bStereo;
    int m_frequency;
    int m_sampleSize;

    int m_resamplingMode;
    int m_stereoSeparation;
    int m_maximumMixingChannels;

    bool m_reverbEnabled;
    int m_reverbLevel;
    int m_reverbDelay;

    bool m_bassEnabled;
    int m_bassLevel;
    int m_bassCutoff;

    bool m_surroundEnabled;
    int m_surroundLevel;
    int m_surroundDelay;

    bool m_oversamplingEnabled;
    bool m_noiseReductionEnabled;
};

Q_DECLARE_METATYPE(PlaybackConfig*);

QDebug operator << (QDebug dbg, PlaybackConfig const &);

#endif
