#pragma once
// ifragmentmodule.h
#include "base/src/basedef.h"
#include "base/src/farray.h"

namespace itasca {
    class IContainer;
}

namespace itascaxd {
    class IContact;
    class IFragment;
    class IBody;
    class Range;
    using namespace itasca;

    class IFragmentModule {
    public:
        virtual int                 getNbFragments(double ) const =0;
        virtual void                setIgnoreBond(bool) = 0;
        virtual bool                getIgnoreBond() = 0;
        virtual void                assignContact(IContact *) = 0;
        virtual void                removeContact(IContact * co) = 0;
        virtual void                registerBodyContainer(itasca::IContainer*) = 0;       
        virtual void                registerContactContainer(TType, itasca::IContainer* , Range& r,bool fast=false) = 0;      
        virtual int                 getFragmentIndex(IBody*, double ) = 0;
        virtual const IContainer*   getIContainerFragment() const = 0;
        virtual IContainer*         getIContainerFragment() = 0;
        virtual void                getFragmentAtState( double state, FArray<IFragment *>* list ) = 0 ;
        virtual bool                checkAddContact( IContact * co ) = 0 ;
        virtual QMap<double,quint64> getHistory( IBody* bd ) = 0 ;
        virtual DVect               getFragmentPosition( IFragment * cl, double state ) = 0;
        virtual uint                getFragmentNbelem( IFragment * cl, double state ) = 0;
        virtual double              getFragmentVolume( IFragment * cl, double state ) = 0;
        virtual QVector<const IBody*> verticesOfFragment( IFragment * cl ) = 0;
        virtual QVector<const IBody*> constructListVertices( IFragment * cl, double state ) = 0;
        virtual QVector<double>       getActiveStates() = 0;
        virtual QVector<QPair<quint64,double> > getFragmentTimes() const = 0;
        virtual double              fragmentTimeFromNumber(int i) const = 0;
        virtual int                 fragmentCycleFromNumber(int i) const = 0;
        virtual int                 fragmentNumberFromTime(const double &d,bool closest=false) const = 0;

    };

} // namespace itascaxd
// EoF
