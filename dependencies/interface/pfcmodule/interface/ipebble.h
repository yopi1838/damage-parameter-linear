#pragma once
// ipebble.h

#include "base/src/matrix.h"
#include "module/interface/ibody.h"
#include "utility/src/thing.h"
#include "utility/src/locallink.h"

namespace itasca {
    class IGroup;
}
namespace itascaxd {
    class IPiece;
    class IPieceMechanical;
    class IPieceMechanicalData;
}

namespace pfc {
    using namespace itasca;
    using namespace itascaxd;
    class IClump;
    class IClumpTemplate;
    class IPebble: public ThingID,
                   public LocalLink<IPebble> 
    {
    public:
        inline static const TType type_ = 0x4e6627a1;
        IPebble(quint64 id) : ThingID(id) { }
        virtual ~IPebble() { };
        virtual IThing *                    getIThing() { return this; }
        virtual const IThing *              getIThing() const { return this; }
        virtual IPiece *                    getIPiece()=0;
        virtual const IPiece *              getIPiece() const=0;
        virtual IPieceMechanical *          getIPieceMechanical()=0;
        virtual const IPieceMechanical *    getIPieceMechanical() const=0;
        virtual IPieceMechanicalData *      getIPieceMechanicalData()=0;
        virtual const IPieceMechanicalData *getIPieceMechanicalData() const=0;
        virtual IClump *                    getIClump()=0;
        virtual const IClump *              getIClump() const=0;
        virtual void                        setIClump(IClump *)=0;
        virtual IPebble *                   getNext()=0;
        virtual const IPebble *             getNext() const=0;
        virtual IPebble *                   getPrev()=0;
        virtual void                        setNext(IPebble *)=0;
        virtual void                        setPrev(IPebble *)=0;
        virtual const IPebble *             getPrev() const=0;
        virtual bool                        baseIsIn(const DVect &d,const double &tol=1e-7) const { d; tol; throw Exception("Not in IPiece baseIsIn"); }
        virtual bool                        baseBoxIsIn(const DExtent &d,const double &tol=1e-7) const { d; tol;  throw Exception("Not in IPiece baseBoxIsIn"); }
        virtual bool                        baseIntersects(const DExtent &d,const double &tol=1e-7) const { d; tol; throw Exception("Not in IPiece baseIntersects"); }
        virtual IBody::InOutMode            baseCheckInOutSphere(const DVect &,const double &) const=0;
        virtual void                        baseScale(const double &d)=0;
        virtual double                      baseRadius() const=0;
        virtual void                        baseSetOnRestore(bool b)=0;
        virtual void                        baseSetMapTolConst(const double &d)=0;
        virtual double                      baseTolerance() const=0;
        virtual bool                        checkExtent(bool localLock=false)=0;
        virtual bool                        regularPebble() const=0;

        virtual void                        copy(const IPebble *)=0;

        virtual DVect            getPosition() const=0;
        virtual double           getRadius() const=0;
        virtual DVect            getVelocity() const=0;
        virtual DAVect           getAngVelocity() const=0;
        virtual DExtent          getDExtent() const=0;
        virtual DExtent          getCellExtent() const=0;
        virtual DExtent          getToleranceExtent() const=0;
        virtual bool             getIsClumpTemplate() const=0;
        virtual bool             getIsClumpTemplateAndReferredTo() const=0;
        virtual void             spinTranslate(const DMatrix<dim,dim> &rm,const DVect &trans,bool lock=false)=0;
        /// Return the IClumpTemplate pointer
        virtual IClumpTemplate * getMyClumpTemplate() =0;
        /// Return the const IClumpTemplate pointer
        virtual const IClumpTemplate * getMyClumpTemplate() const =0;

        virtual void setPosition(const DVect &p)=0;
        virtual void setRadius(const double &d,bool lock=false)=0;
        virtual void setRadiusNoLock(const double &d)=0;

        virtual int getFragmentIndex() const = 0;

        // for plotting
        virtual void getFacetGeometry(FArray<DVect> *vert,double &) const=0;


    };
} // namespace pfc
// EoF

