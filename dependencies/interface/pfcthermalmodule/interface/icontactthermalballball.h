#pragma once
// icontactthermalballball.h

namespace itasca {
    class IThing;
};

namespace itascaxd {
    class IContact; 
    class IContactThermal; 
};

namespace pfcthermal {
    using namespace itasca;
    using namespace itascaxd;

    class IContactThermalBallBall {
    public:
        inline static const TType type_ = 0x4f5effdb;

        virtual IThing *                     getIThing()                =0;
        virtual const IThing *               getIThing()   const        =0;
        virtual IContact *                   getIContact()              =0;
        virtual const IContact *             getIContact() const        =0;
        virtual IContactThermal *            getIContactThermal()       =0;
        virtual const IContactThermal *      getIContactThermal() const =0;
    };
} // namespace pfcthermal
// EoF

