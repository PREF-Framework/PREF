#ifndef FIELD_H
#define FIELD_H

#include <QtCore>
#include <QtWidgets>
#include "prefsdk/format/elements/fieldobject.h"
#include "prefsdk/format/elements/bitfield.h"

namespace PrefSDK
{
    class Field : public FieldObject
    {
        Q_OBJECT

        public:
            typedef QList<QString> StringList;
            typedef QHash<QString, BitField*> StringMap;

        public:
            explicit Field(lua_State* l, DataType::Type type, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FormatObject* parentobject, QObject *parent = 0);
            explicit Field(lua_State* l, lua_String tablename, DataType::Type type, lua_Integer offset, QString name, ByteBuffer* bytebuffer, LuaCTable* model, FormatObject* parentobject, QObject *parent = 0);
            BitField* bitField(int i) const;
            int bitFieldCount() const;
            int indexOf(BitField* bf) const;
            BitField* setBitField(lua_Integer bitstart, QString name);
            BitField *setBitField(lua_Integer bitstart, lua_Integer bitend, QString name);
            BitField* find(const QString& name);

        lua_private:
            LuaTable::Ptr setBitField(lua_Integer bitstart, lua_String name);
            LuaTable::Ptr setBitField(lua_Integer bitstart, lua_Integer bitend, lua_String name);
            LuaTable::Ptr find(lua_String bitfieldname);

        protected:
            virtual void metaIndex(lua_State* l);

        public: /* Overriden methods */
            virtual FormatObject::FormatObjectType objectType();

        private:
            StringList _stringlist;
            StringMap _stringmap;
    };
}

#endif // FIELD_H
