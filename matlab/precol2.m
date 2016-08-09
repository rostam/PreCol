function [req,pot,add,F] = precol2(col_ord,ilu_ord,bls,el,MatrixName)
com = './precol.out';
pars=[' PartialD2RestrictedColumns ',col_ord,'_',ilu_ord];
pars = [pars,' Best 1.0 BlockDiagonal ',bls,' ',el,' '];
%pars = ' PartialD2RestrictedColumns SLO_Nat Best 1.0 BlockDiagonal 10 2';
[com,pars,MatrixName]
[status,cmdout]=system([com,pars,MatrixName]);
cmdout
pot = mmread('matlab/pot.mtx');
pot = spones(pot);
nnz(pot)
add = mmread('matlab/add.mtx');
add = spones(add);
nnz(add)
req = mmread('matlab/req.mtx');
req = spones(req);
F = mmread('matlab/F.mtx');
F = spones(F);
end