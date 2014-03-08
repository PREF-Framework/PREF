#ifndef PREFSDK_DISASSEMBLERLISTING_H
#define PREFSDK_DISASSEMBLERLISTING_H

#include "prefsdk/qlua.h"
#include "prefsdk/prefdebug.h"
#include "prefsdk/io/bytebuffer.h"
#include "referencetable.h"
#include "listingitems/listingitem.h"
#include "listingitems/instructionitem.h"
#include "listingitems/labelitem.h"
#include "processor/processordefinition.h"

namespace PrefSDK
{
    class DisassemblerListing : public QObject, public LuaCTable
    {
        Q_OBJECT

        private:
            class Listing
            {
                private:
                    enum ListingFlags { ContainsNothing = 0, ContainsInstruction = 1, ContainsLabel = 2 };
                    typedef QMap<lua_Integer, InstructionItem*> DisassembledAddressList;
                    typedef QList<ListingItem*> ListingList;
                    typedef QPair<int, ListingList> ListingPair;
                    typedef QMap<lua_Integer, ListingPair> ListingMap;

                public:
                    Listing(): _count(0) { }

                private:
                    ListingPair& getList(lua_Integer address)
                    {
                        if(!this->_listing.contains(address))
                            this->_listing[address] = ListingPair(Listing::ContainsNothing, ListingList());

                        return this->_listing[address];
                    }

                public:
                    void add(InstructionItem* ii)
                    {
                        ListingPair& lp = this->getList(ii->address());
                        lp.first |= Listing::ContainsInstruction;
                        lp.second.append(ii);

                        Instruction::Ptr instr = ii->instruction();
                        lua_Integer addr = instr->address();

                        for(int i = 0; i < instr->instructionSize(); i++)
                            this->_disaddrlist[addr + i] = ii; /* Mark those addresses as disassembled */

                        this->_count++;
                    }

                    void add(LabelItem* li)
                    {
                        ListingPair& lp = this->getList(li->address());

                        if((lp.first & Listing::ContainsLabel) == 0)
                        {
                            lp.first |= Listing::ContainsLabel;
                            lp.second.prepend(li);
                            this->_count++;
                        }
                    }

                    bool contains(lua_Integer address) { return this->_listing.contains(address); }
                    bool containsInstruction(lua_Integer address) { return this->_disaddrlist.contains(address); }

                    ListingItem* operator[](int idx)
                    {
                        int i = 0;
                        ListingMap::Iterator it;

                        for(it = this->_listing.begin(); it != this->_listing.end(); it++)
                        {
                            ListingList ll = it.value().second;

                            for(int j = 0; j < ll.count(); j++)
                            {
                                if(i == idx)
                                    return ll[j];

                                i++;
                            }
                        }

                        throw std::runtime_error("DisassemblerListing: Index Not Found");
                    }

                    bool isEmpty() { return this->_count == 0; }
                    uint count() { return this->_count; }

                private:
                    DisassembledAddressList _disaddrlist;
                    ListingMap _listing;
                    uint _count;
            };

        public:
            typedef std::shared_ptr<DisassemblerListing> Ptr;

        public:
            explicit DisassemblerListing(lua_State* l, ByteBuffer *bytebuffer, const ProcessorDefinition::Ptr& processordef, QObject* parent = 0);
            void addInstruction(const Instruction::Ptr &item);
            bool isDisassembled(lua_Integer address);
            ListingItem *item(int i);
            ReferenceTable *referenceTable();
            ByteBuffer* buffer();
            int itemCount();
            int maxInstructionSize();
            void printBacktrace();

        public:
            static DisassemblerListing::Ptr create(lua_State* l, ByteBuffer *bytebuffer, const ProcessorDefinition::Ptr& processordef);

        private slots:
            void onCodeReferenceAdded(lua_Integer address);

        private:
            Listing _listing;
            ProcessorDefinition::Ptr _processordef;
            ReferenceTable* _referencetable;
            ByteBuffer* _bytebuffer;
            int _maxinstructionsize;
    };
}

#endif // PREFSDK_DISASSEMBLERLISTING_H
