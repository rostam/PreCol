close all

els = [];
Fs = [];
adds = [];
for i=0 : 16
  [req,pot,add,F] = precol2('LFO','Nat','10',int2str(i),'nos3.mtx');
  els = [els i];
  Fs= [Fs nnz(F)];
  adds = [adds nnz(add)];
end
yyaxis left
plot(els,Fs,'-r*','LineWidth',2)
ylim([0 85])
xlabel('The level parameter for ILU','FontWeight','bold','FontSize',18);
ylabel('The number of fillins','FontWeight','bold','FontSize',18);
yyaxis right
plot(els,adds,'-b*','LineWidth',2)
ylim([380 395])
xlabel('The level parameter for ILU','FontWeight','bold','FontSize',18);
ylabel('The number of additionally required elements','FontWeight','bold','FontSize',18);
%export_fig('figs/el_fillins.jpg','-painters','-m2.5','-transparent');