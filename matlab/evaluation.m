close all
els = [];
Fs = [];pots=[];adds = [];
Fs1 = [];pots1=[];adds1 = [];
Fs2 = [];pots2=[];adds2=[];
for i=10 : 20 :  30
    i
    els = [els i];
    [req,pot,add,F] = precol2('LFO','Nat',int2str(i),'10','ex33.mtx');
    Fs= [Fs nnz(F)];
    pots=[pots nnz(pot)];
    adds = [adds nnz(add)];
    [req1,pot1,add1,F1] = precol2('LFO','Min',int2str(i),'10','ex33.mtx');
    Fs1= [Fs1 nnz(F1)];
    pots1=[pots1 nnz(pot1)];
    adds1 = [adds1 nnz(add1)];
    [req2,pot2,add2,F2] = precol2('LFO','Metis',int2str(i),'10','ex33.mtx');
    Fs2= [Fs2 nnz(F2)];
    pots2=[pots2 nnz(pot2)];
    adds2 = [adds2 nnz(add2)];
end
 figure,plot(els,adds,'-r*','LineWidth',2)
 hold on
 shg, plot(els, adds1, '-g*', 'LineWidth',2)
 shg, plot(els, adds2, '-b*', 'LineWidth',2)
 xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
 ylabel('The number of additionally required elements','FontWeight','bold','FontSize',18);
 set(gca,'yscale','log')
 h=legend('Natural Ordering',...
         ['Minimum Ordering'],...
         ['Metis Ordering']);
     set(h,'Location','Best',...
         'FontSize',22);
    
figure,plot(els, pots,'-r*','LineWidth',2)
hold on
shg, plot(els, pots1, '-g*', 'LineWidth',2)
shg, plot(els, pots2, '-b*', 'LineWidth',2)
xlabel('The block size parameter for sparsification','FontWeight','bold','FontSize',18);
ylabel('The number of additionally required elements','FontWeight','bold','FontSize',18);
set(gca,'yscale','log')
h=legend('Natural Ordering',...
        ['Minimum Ordering'],...
        ['Metis Ordering']);
    set(h,'Location','Best',...
        'FontSize',22);    

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
    