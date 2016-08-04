function [req,pot,add,F] = precol(MatrixName)
com = './precol.out';
pars = ' PartialD2RestrictedColumns SLO_Nat Best 1.0 BlockDiagonal 10 2';
system([com,pars,MatrixName]);
pot = mmread('pot.mtx');
pot = spones(pot);
add = mmread('add_mat.mtx');
add = spones(add);
req = mmread('req.mtx');
req = spones(req);
F = mmread('F.mtx');
F = spones(F);
end