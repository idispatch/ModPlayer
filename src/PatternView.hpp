#ifndef PATTERNVIEW_HPP_
#define PATTERNVIEW_HPP_

#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>
#include "Canvas.hpp"
#include "SongModule.hpp"
#include "InstanceCounter.hpp"
#include <vector>

namespace bb {
    namespace cascades {
        class Container;
        class ImageView;
    }
}

class PatternView : public bb::cascades::CustomControl,
                    public InstanceCounter<PatternView>
{
    Q_OBJECT
public:
    PatternView(bb::cascades::Container *parent = 0);

    Q_PROPERTY(QVariant song READ song WRITE setSong NOTIFY songChanged FINAL)

    QVariant song() const;
    void setSong(QVariant value);

    using InstanceCounter<PatternView>::getInstanceCount;
    using InstanceCounter<PatternView>::getMaxInstanceCount;
Q_SIGNALS:
    void songChanged();
private slots:
    void onSongLoadedChanged();
    void onChannelsChanged();
    void onCurrentRowChanged();
    void onCurrentPatternChanged();
private:
    void createPatternView();
    void updateCanvas();
private:
    QMutex m_mutex;
    bb::cascades::Container * m_rootContainer;
    SongModule * m_song;
    Canvas * m_canvas;
};

Q_DECLARE_METATYPE(PatternView*);

#endif /* PATTERNVIEW_HPP_ */
