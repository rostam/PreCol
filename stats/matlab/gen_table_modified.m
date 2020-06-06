close all
els = [];
Fs  = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs2 = [];pots2=[];adds2=[];
cols = [];cols1=[];cols2=[];
alpha=9;
mats = {'steam1.mtx','steam2.mtx','nos3.mtx','ex7.mtx', ...
 'ex33.mtx','crystm01.mtx', 'coater1.mtx','pesa.mtx','gyro_m.mtx'};

for i = 1 : 1 : 8
  mat = strjoin(mats(i));
  [req,pot,add,F,c] = precol2('D2RestrictedColumnsNonReq','Nat','Nat','200','10',mat,-1);
  Fs= [Fs nnz(F)];
  pots=[pots nnz(pot)];
  adds = [adds nnz(add)];
  cols = [cols c];
  
  [req1,pot1,add1,F1,c1] = precol2('D2RestrictedColumnsNonReq','Nat','Min','200','10',mat,-1);
  Fs1= [Fs1 nnz(F1)];
  pots1=[pots1 nnz(pot1)];
  adds1 = [adds1 nnz(add1)];
  cols1 = [cols1 c1];
  
  fprintf('\\textit{%s} & $%i$ & $%i$ & $%i$ & $%i$ & $%i$ & $%i$\\\\\\hline\n', mat ...
      ,nnz(add),nnz(add1),nnz(pot), nnz(pot1),nnz(F),nnz(F1))
end
  

%pots
%adds
%cols
