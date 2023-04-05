#pragma once
// ifragmentlogic.h
// interface to access fragment logic

namespace itasca
{
  class IThing; 
  class IContainer;
}


namespace itascaxd
{
  using namespace itasca;


  class IFragmentLogic
  {
  public:
    inline static const TType type_ = 0x512b7041; 

    virtual DVect   getBarycentre(uint clindex) const =0;
    virtual int     getNbElements(uint clindex) const =0;
    virtual int     getNbFragment(double state) const =0;
    
  };
} // namespace dfn
// EoF