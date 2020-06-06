% close all
% els = [];
% Fs  = [];pots=[];adds = [];cols=[];
% Fs1 = [];pots1=[];adds1 = [];cols1=[];
% Fs2 = [];pots2=[];adds2=[];cols2=[];
% Fs3 = [];pots3=[];adds3=[];cols3=[]
% alpha=9;
% mats = {'steam1.mtx','steam2.mtx','nos3.mtx','ex7.mtx', ...
%  'ex33.mtx','crystm01.mtx', 'coater1.mtx','pesa.mtx'};
% 
% for i = 1 : 1: 70
%   i
%   els = [els i];
%   [req,pot,add,F,c] = precol2('D2RestrictedColumnsNonReqBalanced','LFO','Nat',num2str(i),'2','nos3.mtx',0);
%   [req1,pot1,add1,F1,c1] = precol2('D2RestrictedColumnsNonReqBalanced','LFO','Nat',num2str(i),'2','nos3.mtx',2);
%   [req2,pot2,add2,F2,c2] = precol2('D2RestrictedColumnsNonReqBalanced','LFO','Nat',num2str(i),'2','nos3.mtx',6);
%   [req3,pot3,add3,F3,c3] = precol2('D2RestrictedColumnsNonReqBalanced','LFO','Nat',num2str(i),'2','nos3.mtx',10);
%   Fs= [Fs nnz(F)];
%   pots=[pots nnz(pot)];
%   adds = [adds nnz(add)];
%   cols = [cols c];
%   
%   Fs1= [Fs1 nnz(F1)];
%   pots1=[pots1 nnz(pot1)];
%   adds1 = [adds1 nnz(add1)];
%   cols1 = [cols1 c1];
%   
%   Fs2= [Fs2 nnz(F2)];
%   pots2=[pots2 nnz(pot2)];
%   adds2 = [adds2 nnz(add2)];
%   cols2 = [cols2 c2];
%   
%   Fs3= [Fs3 nnz(F3)];
%   pots3=[pots3 nnz(pot3)];
%   adds3 = [adds3 nnz(add3)];
%   cols3 = [cols3 c3];
% end
% 
% figure, hold on
% shg, plot(els, adds, '-g*', 'LineWidth',2)
% shg, plot(els, adds1, '-b*', 'LineWidth',2)
% shg, plot(els, adds2,'-r*','LineWidth',2)
% shg, plot(els, adds3,'-k*','LineWidth',2)
% xlabel('The block size','FontWeight','bold','FontSize',18);
% ylabel('The number of additionally required elements','FontWeight','bold','FontSize',18);
% h=legend('Algorithm 3.5 with \alpha=0',...
%          'Algorithm 3.5 with \alpha=2',...
%          'Algorithm 3.5 with \alpha=6',...
%          'Algorithm 3.5 with \alpha=10');
%    set(h,'Location','Best',...
%        'FontSize',22);


% figure, hold on
% shg, plot(els, cols, '-g*', 'LineWidth',2)
% shg, plot(els, cols3,'-k*','LineWidth',2)
% xlabel('The block size','FontWeight','bold','FontSize',18);
% ylabel('The number of colors','FontWeight','bold','FontSize',18);
% h=legend('Algorithm 3.5 with \alpha=0',...
%          'Algorithm 3.5 with \alpha=10');
%    set(h,'Location','Best',...
%        'FontSize',22);

% 


figure, hold on
shg, plot(els, cols, '-g*', 'LineWidth',2)
shg, plot(els, cols1, '-b*', 'LineWidth',2)
shg, plot(els, cols2,'-r*','LineWidth',2)
shg, plot(els, cols3,'-k*','LineWidth',2)
xlabel('The block size','FontWeight','bold','FontSize',18);
ylabel('The number of colors','FontWeight','bold','FontSize',18);
h=legend('Algorithm 3.5 with \alpha=0',...
         'Algorithm 3.5 with \alpha=2',...
         'Algorithm 3.5 with \alpha=6',...
         'Algorithm 3.5 with \alpha=10');
   set(h,'Location','Best',...
       'FontSize',22);

