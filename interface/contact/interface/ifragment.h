#pragma once

namespace itasca
{
    class IThing; 
    class IContainer;
}


namespace itascaxd
{
    using namespace itasca;

  class IFragment {
  public:
      inline static const TType type_ = 0x51a871bc;
      virtual IThing *                  getIThing() = 0;
      virtual const IThing *            getIThing() const = 0;

     virtual DVect      barycentre()    const = 0;
     virtual double     volume()        const = 0;
     virtual uint       nbelem()        const = 0;
     virtual quint64   getID()         const = 0;
     virtual quint64        getFather()     const = 0;
     virtual QVector<quint64> getChildren() const = 0;
  };

}

