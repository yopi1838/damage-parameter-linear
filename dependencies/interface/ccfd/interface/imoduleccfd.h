#pragma once
/** \brief Interface for the PFC ccfd module.
    * \file imoduleccfd.h
    *
    * \defgroup pfcccfd PFC ccfd module interface specification
*/

// imoduleccfd.h
// For CCFD Module
#include "base/src/farray.h"

namespace itasca {
    class IContainer;
}

/** \brief ccfd namespace */
namespace ccfd {
    using namespace itasca;
    class ICcfdElement;
    /// \addtogroup pfcccfd
    /// @{

    /// \brief Interface for the PFC ccfd module.
    class IModuleCCFD {
    public:
        virtual const TType          getElementFluidType()                     const =0;
        virtual const ICcfdElement * findElementFluidWithID(quint64 id)       const =0;
        virtual ICcfdElement *       findElementFluidWithID(quint64 id)             =0;

        virtual int                  getNumberOfElements()                     const =0;
        virtual int                  getElementNode(int element, int node)     const =0;
        virtual int                  getElementFaces()                         const =0; // number of faces in an element
        virtual int                  getElementNodes()                         const =0; 
        virtual const ICcfdElement * getICcfdElement(int i)                    const =0;
        virtual const char *         getCcfdElementType()                      const =0;
        virtual int                  getElementNodeGlobalAddress(int e, int n) const =0;
        virtual const IContainer    *getIContainerElement() const = 0;
        virtual IContainer *         getIContainerElement() = 0;
        virtual uint                 getElementCyclingNotice() const = 0;
        virtual uint                 getElementGeometryNotice() const = 0;
        virtual uint                 getElementExtraNotice() const = 0;
        virtual uint                 getElementGroupNotice() const = 0;
        virtual uint                 getCCFDCleanNotice() const = 0;
        virtual void                 createMesh(const QList<double> &nlist, const QList<qint64> &elist)=0;

        // Populates sets of property names based on their type including the relevant notices
        virtual void  populateElementAttributeSets(QStringList *scalarAtts,FArray<uint> *scalarNotices,QStringList *stringAtts,FArray<uint> *stringNotices,QStringList *vectorAtts,FArray<uint> *vectorNotices)const=0;

        //elements in a box
        virtual void getElementsInBoxList(const DExtent &d,FArray<ICcfdElement *> *ret,bool intersect=true)=0;
        virtual void getElementsInBoxList(const DExtent &d,FArray<const ICcfdElement *> *ret,bool intersect=true)=0;
        virtual const ICcfdElement *            findElementClosestTo(const DVect &v,const double &radius=limits<double>::max()) const=0;
        virtual ICcfdElement *                  findElementClosestTo(const DVect &v,const double &radius=limits<double>::max())=0;
    /// @}


    };

} // namespace ccfd
// EoF
