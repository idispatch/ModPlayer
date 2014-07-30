#ifndef RADIO_HPP_
#define RADIO_HPP_

#include <QObject>
#include <QMetaType>
#include <QDebug>
#include "ItemGroupBase.hpp"
#include "InstanceCounter.hpp"

class Radio : public ItemGroupBase,
              public InstanceCounter<Radio> {
    Q_OBJECT
    Q_PROPERTY(QString radioPlaylist READ radioPlaylist NOTIFY radioPlaylistChanged FINAL)
    Q_PROPERTY(QString country       READ country NOTIFY countryChanged FINAL)
    Q_PROPERTY(QString location      READ location NOTIFY locationChanged FINAL)
    Q_PROPERTY(QString style         READ style NOTIFY styleChanged FINAL)
    Q_PROPERTY(QString url           READ url NOTIFY urlChanged FINAL)
    Q_PROPERTY(QString flag          READ flag NOTIFY flagChanged FINAL)
    Q_PROPERTY(int     bitrate       READ bitrate NOTIFY bitrateChanged FINAL)
public:
    Radio(int id,
          QString const& name,
          QString const& radioPlaylist,
          QString const& country,
          QString const& location,
          QString const& style,
          QString const& url,
          QString const& flag,
          int bitrate,
          QObject *parent);
    ~Radio();

    QString const& radioPlaylist() const;
    QString const& country() const;
    QString const& location() const;
    QString const& style() const;
    QString const& url() const;
    QString const& flag() const;
    int bitrate() const;

    using InstanceCounter<Radio>::getInstanceCount;
    using InstanceCounter<Radio>::getMaxInstanceCount;
Q_SIGNALS:
    void radioPlaylistChanged();
    void countryChanged();
    void locationChanged();
    void styleChanged();
    void urlChanged();
    void flagChanged();
    void bitrateChanged();
private:
    Q_DISABLE_COPY(Radio)

    QString m_playlist;
    QString m_country;
    QString m_location;
    QString m_style;
    QString m_url;
    QString m_flag;
    int     m_bitrate;
};

Q_DECLARE_METATYPE(Radio*);
QDebug operator << (QDebug dbg, Radio const&p);

#endif
