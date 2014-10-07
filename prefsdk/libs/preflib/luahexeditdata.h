#ifndef PREFSDK_LUAHEXEDITDATA_H
#define PREFSDK_LUAHEXEDITDATA_H

#include <QObject>
#include <lua.hpp>
#include "prefsdk/prefexception.h"
#include "prefsdk/type/datatype.h"
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include "qhexedit/qhexeditdatawriter.h"
#include "qhexedit/qhexeditdatadevice.h"
#include "prefsdk/libs/qt/qtlua.h"

namespace PrefSDK
{
    /*
     * Simplified QHexEditData's interface for Lua
     */
    class LuaHexEditData : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(lua_Integer length READ length)

        public:
            explicit LuaHexEditData(QHexEditData* hexeditdata, QObject *parent = 0);
            QHexEditData* hexEditData() const;
            lua_Integer length() const;

        public slots:
            void copyTo(QObject *luahexeditdata, lua_Integer start, lua_Integer end);
            lua_Integer indexOf(const QString& s, lua_Integer startpos = 0);
            lua_Integer readType(lua_Integer pos, lua_Integer datatype);
            void writeType(lua_Integer pos, lua_Integer datatype, lua_Integer value);
            QString readLine(lua_Integer pos);
            QString readString(lua_Integer pos, lua_Integer maxlen = -1);
            void writeString(lua_Integer pos, const QString& s);

        private:
            Q_INVOKABLE int metaIndex(lua_State* l, lua_Integer key);

        private:
            QHexEditData* _hexeditdata;
            QHexEditDataReader* _reader;
            QHexEditDataWriter* _writer;
    };
}

#endif // PREFSDK_LUAHEXEDITDATA_H