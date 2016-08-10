close all
els = [];
Fs  = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs2 = [];pots2=[];adds2=[];
cols = [];cols1=[];cols2=[];
alpha=9;
for i=1 : 1 :  50
    i
    %[req,pot,add,F,c] = precol2('PartialD2RestrictedColumns','LFO','Nat',int2str(i),'2','nos3.mtx',1);
    %Fs= [Fs nnz(F)];
    %pots=[pots nnz(pot)];
    %adds = [adds nnz(add)];
    %cols = [cols c];
    [req1,pot1,add1,F1,c1] = precol2('PartialD2RestrictedColumns','SLO','Nat',int2str(i),'2','ex33.mtx',1);
    for al = 0 : 10 
    [req2,pot2,add2,F2,c2] = precol2('PartialD2RestrictedColumnsNonReq','SLO','Nat',int2str(i),'2','ex33.mtx',al);
    if(c2 == c1)
      els = [els i];
      Fs1= [Fs1 nnz(F1)];
      pots1=[pots1 nnz(pot1)];
      adds1 = [adds1 nnz(add1)];
      cols1 = [cols1 c1];
    
      Fs2= [Fs2 nnz(F2)];
      pots2=[pots2 nnz(pot2)];
      adds2 = [adds2 nnz(add2)];
      cols2 = [cols2 c2];
      break;
    end
    end 
end

%figure,plot(els,adds,'-r*','LineWidth',2)
figure, hold on
shg, plot(els, adds1, '-g*', 'LineWidth',2)
shg, plot(els, adds2, '-b*', 'LineWidth',2)
xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
ylabel('The number of additionally required elements','FontWeight','bold','FontSize',18);
h=legend('Greedy Coloring (Min Orderings)',...
        ['New Coloring']);
   set(h,'Location','Best',...
       'FontSize',22);

 
 %figure,plot(els,cols,'-r*','LineWidth',2)
 figure,hold on
 shg, plot(els, cols1, '-g*', 'LineWidth',2)
 shg, plot(els, cols2, '-b*', 'LineWidth',2)
 xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
 ylabel('The number of colors','FontWeight','bold','FontSize',18);
 %set(gca,'yscale','log')
 h2=legend('Greedy Coloring',...
         ['New Coloring']);
     set(h2,'Location','Best',...
         'FontSize',22);    


% figure,plot(els, pots,'-r*','LineWidth',2)
% hold on
% shg, plot(els, pots1, '-g*', 'LineWidth',2)
% shg, plot(els, pots2, '-b*', 'LineWidth',2)
% xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
% ylabel('The number of additionally required elements','FontWeight','bold','FontSize',18);
% set(gca,'yscale','log')
% h=legend('Natural Ordering',...
%         ['Minimum Ordering'],...
%         ['Metis Ordering']);
%     set(h,'Location','Best',...
%         'FontSize',22);    

%figure,plot(els,Fs,'-r*','LineWidth',2)
%hold on
%shg, plot(els, Fs1, '-g*', 'LineWidth',2)
%shg, plot(els, Fs2, '-b*', 'LineWidth',2)
%xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
%ylabel('The number of fillins','FontWeight','bold','FontSize',18);
%set(gca,'yscale','log')
%h=legend('Natural Ordering',...
%        ['Minimum Ordering'],...
%        ['Metis Ordering']);
%    set(h,'Location','Best',...
%        'FontSize',22);    
    