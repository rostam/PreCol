close all;
MatrixName= 'result.mtx';
method ='bicgstab';
%method ='gmres';
el = 2;
% GMRES parameter
restart = 3;
tol = 1.0e-17;
maxit = 100;
DirName = 'figs';
FontSize = 14;
mats = {'result.mtx'};
for i = 1 : 20
[req,pot,add,F,c] = precol2('D2RestrictedColumns','Nat','Nat',num2str(i),'2','result.mtx',-1);
[req2,pot2,add2,F2,c2] = precol2('D2RestrictedColumnsNonReq','Nat','Nat',num2str(i),'2','result.mtx',-1);
fprintf('$%i$ & $%i$ & $%i$ & $%i$ & $%i$ & $%i$ & $%i$\\\\\\hline\n', ...
      i,nnz(pot),nnz(add),c,nnz(pot2),nnz(add2),c2)
end