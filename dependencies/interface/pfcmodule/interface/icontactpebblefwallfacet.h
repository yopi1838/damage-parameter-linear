#pragma once
// icontactpebblefwallfacet.h

namespace itasca {
    class IThing;
}

namespace itascaxd {
    class IContact;
    class IContactMechanical;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;

    class IContactPebbleFWallFacet {
    public:
        inline static const TType type_ = 0x4e78ec7d;

        virtual const IThing *            getIThing() const=0;
        virtual IThing *                  getIThing()=0;
        virtual const IContact *          getIContact() const=0;
        virtual IContact *                getIContact()=0;
        virtual const IContactMechanical *getIContactMechanical() const=0;
        virtual IContactMechanical *      getIContactMechanical()=0;

    };

} // namespace pfc
// EoF
