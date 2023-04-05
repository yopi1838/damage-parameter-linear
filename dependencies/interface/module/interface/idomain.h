#pragma once
// 

/**
    * \file idomain.h
    * \brief IDomain class - define the model boundaries.
    */


#include "dim.h"

namespace itasca {
    class IThing;
}

namespace itascaxd {
    /// \brief IDomain class - define the model boundaries.
    /** 
    * \ingroup module
    */
    using namespace itasca;
    class IDomain {
    public:
        /// The type identification number for this class, for use in convert_cast() and convert_getcast().
        inline static const TType type_ = 0x4c815b87;

        /// Returns a pointer to its QObject 
        virtual const QObject *getObject() const=0;
        /// Returns a const IThing interface pointer associated with this domain.
        virtual const IThing *getIThing() const=0;
        /// Returns a IThing interface pointer associated with this domain.
        virtual IThing *      getIThing()=0;

        /// Returns the domain extent.
        virtual DExtent     getDomainExtent() const=0;
        /// Returns the domain extent lower bound.
        virtual DVect       getDomainLowerBound() const=0;
        /// Returns the domain extent upper bound
        virtual DVect       getDomainUpperBound() const=0;
        /// Returns the domain strain rate   
        virtual DMatrix<DIM,DIM> getStrainRate() const =0;
        /// Get the domain remap tolerance   
        virtual double      getRemapTolerance() const=0;
        /// Set the domain extent lower bound.
        virtual void        setDomainLowerBound(const DVect &)=0;
        /// Set the domain extent upper bound   
        virtual void        setDomainUpperBound(const DVect &)=0;
        /// Set the domain strain rate   
        virtual void        setStrainRate(const DMatrix<DIM,DIM> &)=0;
        /// Set the domain remap tolerance   
        virtual void        setRemapTolerance(const double &)=0;

        /// Returns the domain condition.
        virtual int         getCondition(quint32 dof) const=0;
        virtual QString     getCondition(const QString &dir) const=0;
        /// Returns the domain condition name. Possible values are: Stop, Reflect, Destroy and Periodic.
        virtual QStringList getConditionNames() const=0;
        /// Returns whether or not there is a periodic condition on any boundary.
        virtual bool        hasPeriodic() const=0;

        /// Sets the domain extent.
        virtual void setDomainExtent(const DExtent &e,bool=false)=0;
        /// Sets the domain condition.
        virtual void setCondition(quint32 dof,int condition)=0;
        virtual void setCondition(const QString &dir,const QString &cond)=0;

        /// Returns if the domain is in mpi mode
        virtual bool getMPIDomain() const=0;
        /// Set if the domain is in mpi mode
        virtual void setMPIDomain(bool)=0;

        // Signals
        /// Signal emitted when the domain extent has changed
        virtual void domainExtentChanged(const DExtent &e,bool quiet=true)=0;
        /// Signal emitted when the domain condition has changed
        virtual void conditionChanged(quint32 dof,int cond)=0;

    #ifdef TWOD
        virtual double getDepth() const=0;
        //virtual void   setDepth(const double &depth)=0;
        //virtual void   depthChanged(const double &d)=0; // SIGNAL
    #endif 
    };
} // namespace itascaxd
// EoF
