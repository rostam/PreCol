close all
els = [];
Fs  = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs2 = [];pots2=[];adds2=[];
cols = [];cols1=[];cols2=[];
alpha=9;
mats = {'steam1.mtx','steam2.mtx','nos3.mtx','ex7.mtx', ...
 'ex33.mtx','crystm01.mtx', 'coater1.mtx','pesa.mtx'};

for i = 1 : 70
  els = [els i];
  [req,pot,add,F,c] = precol2('D2RestrictedColumns','Nat','Nat',num2str(i),'2','ex33.mtx',-1);
  [req1,pot1,add1,F1,c1] = precol2('D2RestrictedColumnsNonReq','Nat','Nat',num2str(i),'2','ex33.mtx',-1);
  Fs= [Fs nnz(F)];
  pots=[pots nnz(pot)];
  adds = [adds nnz(add)];
  cols = [cols c];
  
  Fs1= [Fs1 nnz(F)];
  pots1=[pots1 nnz(pot1)];
  adds1 = [adds1 nnz(add1)];
  cols1 = [cols1 c1];
  
end
% 
% figure, hold on
% shg, plot(els, pots, '-g*', 'LineWidth',2)
% shg, plot(els, pots1, '-b*', 'LineWidth',2)
% xlabel('The block size','FontWeight','bold','FontSize',18);
% ylabel('The number of potentially required elements','FontWeight','bold','FontSize',18);
% h=legend('Algorithm 3.1',...
%          'Algorithm 3.2');
%    set(h,'Location','Best',...
%         'FontSize',22);
   
% figure, hold on
% shg, plot(els, adds, '-g*', 'LineWidth',2)
% shg, plot(els, adds1, '-b*', 'LineWidth',2)
% xlabel('The block size','FontWeight','bold','FontSize',18);
% ylabel('The number of additionally required elements','FontWeight','bold','FontSize',18);
% h=legend('Algorithm 3.1',...
%          'Algorithm 3.2');
%    set(h,'Location','Best',...
%        'FontSize',22);
% 
% figure, hold on
% shg, plot(els, cols, '-g*', 'LineWidth',2)
% shg, plot(els, cols1, '-b*', 'LineWidth',2)
% xlabel('The block size','FontWeight','bold','FontSize',18);
% ylabel('The number of colors','FontWeight','bold','FontSize',18);
% h=legend('Algorithm 3.1',...
%          'Algorithm 3.2');
%    set(h,'Location','Best',...
%        'FontSize',22);

