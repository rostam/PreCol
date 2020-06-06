function [req,pot,add,F,num_col] = precol2(c,co,io,bl,el,m,a)
% function  [req,pot,add,F,num_col] = precol5(c,co,io,bl,el,m,a)
%      c coloring algorithm, co coloring order, io ilu ordering
%      bl block size, el ilu level parameter, m matrix, a alpha

com = '/home/rostam/kara/precol/cmake-build-debug/precol.out';
pars=[' ',c,' ',co,'_',io];
pars = [pars,' Best 1.5 BlockDiagonal ',bl,' ',el,' ',num2str(a),' '];
[~,cmdout]=system([com,pars,m]);
[com,pars,m]
cmdout

pot = mmread('pot.mtx');
pot = spones(pot);
add = mmread('add.mtx');
add = spones(add);
req = mmread('req.mtx');
req = spones(req);
F = mmread('F.mtx');
F = spones(F);
num_col = str2num(cmdout(findstr(cmdout,'Column Colors:_')+15:findstr(cmdout,'Column Colors:_')+15+1));
end