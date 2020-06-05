//
// Created by rostam on 05.08.16.
//

#ifndef PRECOL_INDSET_H
#define PRECOL_INDSET_H
class IndSet {
protected:
    int mode;
    vector<unsigned int>& V_r;
    vector<unsigned int>& V_c;
    Graph& G_b;
    double ratio;
public:
    IndSet(Graph& g, vector<unsigned int>& Vr,vector<unsigned int>& Vc, int m, double r) :
            G_b(g), V_r(Vr), V_c(Vc), mode(m), ratio(r) {}
    IndSet(Graph& g, vector<unsigned int>& Vr,vector<unsigned int>& Vc, double r) :
            G_b(g), V_r(Vr), V_c(Vc), ratio(r) {}
    IndSet(Graph& g, vector<unsigned int>& Vr,vector<unsigned int>& Vc, int m) :
            G_b(g), V_r(Vr), V_c(Vc), mode(m) {}

    virtual vector<int> compute()=0;
};
#endif //PRECOL_INDSET_H
