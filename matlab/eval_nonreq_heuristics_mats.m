close all
els = [];
Fs  = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs2 = [];pots2=[];adds2=[];
cols = [];cols1=[];cols2=[];
alpha=-1;
names=cell(17,1);
names{1} = 'cage3';names{2} = 'cage4';names{3} = 'cage5';names{4} = 'cage7';
names{5} = 'cage8';names{6} = 'cage9';names{7} = 'cage10';names{8} = 'cage12';
names{9}='steam1';names{10}='steam2';names{11}='nos3';names{12}='ex7';
names{13}='ex33';names{14}='cavity16';names{15}='crystm01';
names{16}='rajat01';names{17}='gyro_m';

for i=1 : 1 : 17
    %if i < 9 || i==17 || i == 12
    if i ~= 5
        continue;
    end
    name = names{i}
    [req1,pot1,add1,F1,c1] = precol2('PartialD2RestrictedColumns', ...
        'Nat','Nat','10','2',[names{i},'.mtx'],alpha);   
    [req2,pot2,add2,F2,c2] = precol2('PartialD2RestrictedColumnsNonReq', ...
        'Nat','Nat','10','2',[names{i},'.mtx'],alpha);
    
    els = [els i];
    Fs1= [Fs1 nnz(F1)];
    pots1=[pots1 nnz(pot1)];
    adds1 = [adds1 nnz(add1)];
    cols1 = [cols1 c1];
    
    Fs2= [Fs2 nnz(F2)];
    pots2=[pots2 nnz(pot2)];
    adds2 = [adds2 nnz(add2)];
    cols2 = [cols2 c2];
end