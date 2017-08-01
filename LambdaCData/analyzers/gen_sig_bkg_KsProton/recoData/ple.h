#ifndef PLE_H_
#define PLE_H_

namespace ple_definition {

//struct definition
struct ple
{
    double mass;
    Float_t p4[4];  //4-momentum vector
    Float_t pt;
    Float_t eta;
    Float_t charge;
};

}	//namespace ple_definition


#endif