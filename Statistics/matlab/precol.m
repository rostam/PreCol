function [req,pot,add,add2,F] = precol(MatrixName)
com = './precol.out';
pars = ' PartialD2RestrictedColumns Nat_Nat Best 1.0 BlockDiagonal 10 2';
system([com,pars,MatrixName]);
pot = mmread('pot.mtx');
pot = spones(pot);
add = mmread('add.mtx');
add = spones(add);
req = mmread('req.mtx');
req = spones(req);
F = mmread('F.mtx');
F = spones(F);

pars = ' PartialD2RestrictedColumnsNonReq Nat_Nat Best 1.0 BlockDiagonal 10 2';
system([com,pars,MatrixName]);
add2 = mmread('add.mtx');
add2 = spones(add);
end