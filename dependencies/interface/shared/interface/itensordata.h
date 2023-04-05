#pragma once

namespace itasca {
    class ITensorData {
    public:
        enum class Quantity { XX=0    ,YY     ,ZZ      ,XY        ,XZ  ,
                              YZ      ,Min    ,Int     ,Max       ,Vol ,
                              MaxShear,TotMeas,VonMises,Octahedral,Norm,
                              Mean };
    };
}
// EoF
