close all
mats = {'pesa.mtx','steam1.mtx','steam2.mtx','685_bus.mtx','nos3.mtx','coater1.mtx','ex7.mtx', ...
 'ex33.mtx','orani678.mtx','cavity16.mtx','crystm01.mtx','rajat01.mtx', ...
 'gyro_m.mtx', 'ford2.mtx', 'cage3.mtx', 'cage4.mtx','cage5.mtx','cage6.mtx',...
 'cage7.mtx','cage8.mtx','cage9.mtx','cage10.mtx','cage12.mtx'};
fprintf('Matrix & Size & Nonzeros & Symmetric\\\\\\hline\n')
for i = 1 : size(mats,2)
  mat = strjoin(mats(i));
  A = mmread(mat);
  symm = 'false';
  if(issymmetric(A)) 
      symm = 'true';
  end
  fprintf('\\textit{%s} & $%i\\times %i$ & $%i$ & %s\\\\\\hline\n', mat ,size(A,1),size(A,2),nnz(A),symm)
end
