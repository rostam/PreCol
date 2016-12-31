close all
els = [];
Fs  = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs2 = [];pots2=[];adds2=[];
cols = [];cols1=[];cols2=[];
alpha=9;
mats = {'steam1.mtx','steam2.mtx','nos3.mtx','ex33.mtx','crystm01.mtx'};
for i = 1 : size(mats,2)
  mat = strjoin(mats(i));
  [req,pot,add,F,c] = precol2('PartialD2RestrictedColumns','LFO','Nat','10','2',mat,-1);
  [req1,pot1,add1,F1,c1] = precol2('PartialD2RestrictedColumnsNonReq','LFO','Nat','10','2',mat,-1);
  Fs= [Fs nnz(F)];
  pots=[pots nnz(pot)];
  adds = [adds nnz(add)];
  cols = [cols c];
  
  Fs1= [Fs1 nnz(F)];
  pots1=[pots1 nnz(pot)];
  adds1 = [adds1 nnz(add)];
  cols1 = [cols1 c];
  
  fprintf('\\textit{%s} & $%i$ & %i$ & $%i$ & $%i$ \\\\\\hline\n', mat ...
      ,nnz(pot),nnz(pot1),nnz(add),nnz(add1))
end
%pots
%adds
%cols
