function [req,pot,add,F] = precol2(col_ord,ilu_ord,bls,el,MatrixName)
com = './precol.out';
pars=[' PartialD2RestrictedColumns ',col_ord,'_',ilu_ord];
pars = [pars,' Best 1.0 BlockDiagonal ',bls,' ',el,' '];
%pars = ' PartialD2RestrictedColumns SLO_Nat Best 1.0 BlockDiagonal 10 2';
[status,cmdout]=system([com,pars,MatrixName]);
pot = mmread('matlab/pot.mtx');
pot = spones(pot);
add = mmread('matlab/add_mat.mtx');
add = spones(add);
req = mmread('matlab/req.mtx');
req = spones(req);
F = mmread('matlab/F.mtx');
F = spones(F);
end