close all
els = [];
Fs  = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs11 = [];pots11=[];adds11 = [];
Fs2 = [];pots2=[];adds2=[];
Fs22 = [];pots22=[];adds22=[];
cols = [];cols1=[];cols2=[];cols11=[];cols22=[];
alpha=-1;
for i=0 : 1 :  25
    i
    [req,pot,add,F,c] = precol2('PartialD2RestrictedColumns','LFO','Metis','100',int2str(i),'ex33.mtx',1);
    [req1,pot1,add1,F1,c1] = precol2('PartialD2RestrictedColumns','LFO','Nat','100',int2str(i),'ex33.mtx',1);
    [req2,pot2,add2,F2,c2] = precol2('PartialD2RestrictedColumns','LFO','Min','100',int2str(i),'ex33.mtx',1);
    els = [els i];
     
    Fs= [Fs nnz(F)];
    pots=[pots nnz(pot)];
    adds = [adds nnz(add)];
    cols = [cols c];
  
    Fs1= [Fs1 nnz(F1)];
    pots1=[pots1 nnz(pot1)];
    adds1 = [adds1 nnz(add1)];
    cols1 = [cols1 c1];
    
    Fs2= [Fs2 nnz(F2)];
    pots2=[pots2 nnz(pot2)];
    adds2 = [adds2 nnz(add2)];
    cols2 = [cols2 c2];
end

figure,plot(els,Fs1,'-r*','LineWidth',2)
hold on
shg, plot(els, Fs2, '-g*', 'LineWidth',2)
shg, plot(els, Fs, '-b*', 'LineWidth',2)
xlabel('The level parameter of ILU factorization','FontWeight','bold','FontSize',18);
ylabel('The number of fillins','FontWeight','bold','FontSize',18);
h=legend('Natural Ordering',...
         'Minimum Ordering',...
         'Metis Ordering');
set(h,'Location','Best','FontSize',22);   

% figure,plot(els,adds1,'-r*','LineWidth',2)
% hold on
% shg, plot(els, adds2, '-g*', 'LineWidth',2)
% shg, plot(els, adds, '-b*', 'LineWidth',2)
% xlabel('The level parameter of ILU factorization','FontWeight','bold','FontSize',18);
% ylabel('The number of fillins','FontWeight','bold','FontSize',18);
% h=legend('Natural Ordering',...
%          'Minimum Ordering',...
%          'Metis Ordering');
% set(h,'Location','Best','FontSize',22);   