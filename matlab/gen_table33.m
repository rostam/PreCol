close all
els = [];
Fs  = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs2 = [];pots2=[];adds2=[];
cols = [];cols1=[];cols2=[];
alpha=9;
mats = {'steam1.mtx','steam2.mtx','nos3.mtx','ex7.mtx', ...
 'ex33.mtx','crystm01.mtx', 'coater1.mtx','pesa.mtx'};
for i = 1 : 1 : 8
  mat = strjoin(mats(i));
  [req,pot,add,F,c] = precol2('D2RestrictedColumnsNonReqBalanced','SLO','Nat','10','2',mat,0);
  Fs= [Fs nnz(F)];
  pots=[pots nnz(pot)];
  adds = [adds nnz(add)];
  cols = [cols c];
  
  [req1,pot1,add1,F1,c1] = precol2('D2RestrictedColumnsNonReqBalanced','SLO','Nat','10','2',mat,6);
  Fs1= [Fs1 nnz(F1)];
  pots1=[pots1 nnz(pot1)];
  adds1 = [adds1 nnz(add1)];
  cols1 = [cols1 c1];
  
  [req2,pot2,add2,F2,c2] = precol2('D2RestrictedColumnsNonReqBalanced','SLO','Nat','10','2',mat,10);
  Fs2= [Fs2 nnz(F2)];
  pots2=[pots2 nnz(pot2)];
  adds2 = [adds2 nnz(add2)];
  cols2 = [cols2 c2];
  
  fprintf('\\textit{%s} & $%i$ & $%i$ & $%i$ & $%i$ & $%i$ & $%i$ \\\\\\hline\n', mat ...
      ,nnz(add),nnz(add1),nnz(add2),c,c1,c2)
end
  

%pots
%adds
%cols
