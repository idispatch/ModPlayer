#ifndef SONGGENRE_HPP_
#define SONGGENRE_HPP_

#include "ItemGroupBase.hpp"

class SongGenre : public ItemGroupBase {
    Q_OBJECT
public:
    SongGenre(int id,
              QString const& name,
              int count,
              QObject *parent = 0);
    ~SongGenre();
Q_SIGNALS:
private:
    Q_DISABLE_COPY(SongGenre)
};

Q_DECLARE_METATYPE(SongGenre*);

#endif
