close all
els = [];
Fs  = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs11 = [];pots11=[];adds11 = [];
Fs2 = [];pots2=[];adds2=[];
Fs22 = [];pots22=[];adds22=[];
cols = [];cols1=[];cols2=[];cols11=[];cols22=[];
alpha=-1;
for i=1 : 1 :  70
    i
%    [req,pot,add,F,c] = precol2('PartialD2RestrictedColumns','LFO','Nat',int2str(i),'2','ex33.mtx',1);
%     Fs= [Fs nnz(F)];
%     pots=[pots nnz(pot)];
%     adds = [adds nnz(add)];
%     cols = [cols c];
    [req1,pot1,add1,F1,c1] = precol2('PartialD2RestrictedColumns','SLO','Nat',int2str(i),'2','ex33.mtx',1);
    %for al = 0 : 15 
    [req2,pot2,add2,F2,c2] = precol2('PartialD2RestrictedColumnsNonReq','LFO','Nat',int2str(i),'2','ex33.mtx',alpha);
    
    [req11,pot11,add11,F11,c11] = precol3('PartialD2RestrictedColumns','SLO','Nat',int2str(i),'2','ex33.mtx',1);
    %for al = 0 : 15 
    [req22,pot22,add22,F22,c22] = precol3('PartialD2RestrictedColumnsNonReq','LFO','Nat',int2str(i),'2','ex33.mtx',alpha);
    
    
    %if(abs(c2 - c1) <= 3)
      els = [els i];
      Fs1= [Fs1 nnz(F1)];
      pots1=[pots1 nnz(pot1)];
      adds1 = [adds1 nnz(add1)];
      cols1 = [cols1 c1];
    
      Fs2= [Fs2 nnz(F2)];
      pots2=[pots2 nnz(pot2)];
      adds2 = [adds2 nnz(add2)];
      cols2 = [cols2 c2];
      
      Fs11= [Fs11 nnz(F11)];
      pots11=[pots11 nnz(pot11)];
      adds11 = [adds11 nnz(add11)];
      cols11 = [cols11 c11];
    
      Fs22= [Fs22 nnz(F22)];
      pots22=[pots22 nnz(pot22)];
      adds22 = [adds22 nnz(add22)];
      cols22 = [cols22 c22];
%      break;
    %end
%    end 
end

figure,plot(els,adds2,'-r*','LineWidth',2)
hold on
shg, plot(els, adds22, '-g*', 'LineWidth',2)
xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
ylabel('The number of additionally required elements','FontWeight','bold','FontSize',18);
h=legend('Greedy Coloring',...
         'Independent Set Coloring');
set(h,'Location','Best',...
        'FontSize',22);
% 
%  
%  %figure,plot(els,cols,'-r*','LineWidth',2)
%  figure,hold on
%  shg, plot(els, cols1, '-g*', 'LineWidth',2)
%  shg, plot(els, cols2, '-b*', 'LineWidth',2)
%  xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
%  ylabel('The number of colors','FontWeight','bold','FontSize',18);
%  %set(gca,'yscale','log')
%  h2=legend('Greedy Coloring',...
%          ['New Coloring with \alpha = ', num2str(alpha+1)]);
%      set(h2,'Location','Best',...
%          'FontSize',22);    
% 
% 
% % figure,plot(els, pots,'-r*','LineWidth',2)
% % hold on
% % shg, plot(els, pots1, '-g*', 'LineWidth',2)
% % shg, plot(els, pots2, '-b*', 'LineWidth',2)
% % xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
% % ylabel('The number of additionally required elements','FontWeight','bold','FontSize',18);
% % set(gca,'yscale','log')
% % h=legend('Natural Ordering',...
% %         ['Minimum Ordering'],...
% %         ['Metis Ordering']);
% %     set(h,'Location','Best',...
% %         'FontSize',22);    
% 
% %figure,plot(els,Fs,'-r*','LineWidth',2)
% %hold on
% %shg, plot(els, Fs1, '-g*', 'LineWidth',2)
% %shg, plot(els, Fs2, '-b*', 'LineWidth',2)
% %xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
% %ylabel('The number of fillins','FontWeight','bold','FontSize',18);
% %set(gca,'yscale','log')
% %h=legend('Natural Ordering',...
% %        ['Minimum Ordering'],...
% %        ['Metis Ordering']);
% %    set(h,'Location','Best',...
% %        'FontSize',22);    
    