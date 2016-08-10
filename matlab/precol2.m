function [req,pot,add,F,num_col] = precol2(col,col_ord,ilu_ord,bls,el,MatrixName, alpha)
com = './precol.out';
pars=[' ',col,' ',col_ord,'_',ilu_ord];
pars = [pars,' Best 1.0 BlockDiagonal ',bls,' ',el,' ',num2str(alpha),' '];
%pars = ' PartialD2RestrictedColumns SLO_Nat Best 1.0 BlockDiagonal 10 2';
%[com,pars,MatrixName]
[status,cmdout]=system([com,pars,MatrixName]);
%cmdout
pot = mmread('matlab/pot.mtx');
pot = spones(pot);
add = mmread('matlab/add.mtx');
add = spones(add);
req = mmread('matlab/req.mtx');
req = spones(req);
F = mmread('matlab/F.mtx');
F = spones(F);
num_col = str2num(cmdout(findstr(cmdout,'Column Colors:_')+15:findstr(cmdout,'Column Colors:_')+15+1));
end