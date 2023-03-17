#pragma once
// icontactballball.h

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

    class IContactBallBall {
    public:
        inline static const TType type_ = 0x4c8f3141;

        virtual const IThing *            getIThing() const=0;
        virtual IThing *                  getIThing()=0;
        virtual const IContact *          getIContact() const=0;
        virtual IContact *                getIContact()=0;
        virtual const IContactMechanical *getIContactMechanical() const=0;
        virtual IContactMechanical *      getIContactMechanical()=0;

    };
} // namespace pfc
// EoF
