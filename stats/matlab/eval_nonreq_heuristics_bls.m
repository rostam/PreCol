close all
els = [];
Fs  = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs2 = [];pots2=[];adds2=[];
cols = [];cols1=[];cols2=[];
alpha=9;
for i=1 : 1 :  100
    i
    [req1,pot1,add1,F1,c1] = precol2('PartialD2RestrictedColumnsNonReq', ...
        'Nat','Nat',int2str(i),'2','crystm01.mtx',-1);   
    [req2,pot2,add2,F2,c2] = precol2('StarBicoloringSchemeCombinedVertexCoverColoringRestrictedNonReq', ...
        'Nat','Nat',int2str(i),'2','crystm01.mtx',-1);
    
    els = [els i];
    Fs1= [Fs1 nnz(F1)];
    pots1=[pots1 nnz(pot1)];
    adds1 = [adds1 nnz(add1)];
    cols1 = [cols1 c1];
    
    Fs2= [Fs2 nnz(F2)];
    pots2=[pots2 nnz(pot2)];
    adds2 = [adds2 nnz(add2)];
    cols2 = [cols2 c2];
end