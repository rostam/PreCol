close all;

MatrixName= 'result.mtx';
method ='bicgstab';
%method ='gmres';
el = 2;
% GMRES parameter
restart = 3;
tol = 1.0e-17;
maxit = 100;

DirName = 'figs';
FontSize = 14;
B = load('co2_jac_first_iteration.mat');
A = B.jac; %mmread(MatrixName);
[req,pot,add,add2,F] = precol4(MatrixName);
b = sum(A,2);

[L_block,U_block] = ILUR(req.*A,req+F);
[L_block_add,U_block_add] = ILUR(req.*A+add.*A,req+F+add);
[L_block_add2,U_block_add2] = ILUR(req.*A+add2.*A,req+F+add2);
disp('Solve without PC')
[x,residual] = SolveLinearSystem(A,b,restart,tol,maxit,method);
    
disp('Solve with block + add PC')
[x_pc,residual_block_add] = SolveLinearSystem(A,b,restart,tol,maxit,method,L_block_add,U_block_add);
    
disp('Solve with block + add PC')
[x_pc2,residual_block_add2] = SolveLinearSystem(A,b,restart,tol,maxit,method,L_block_add2,U_block_add2);
    

disp('Solve with block PC')
[x_orig,residual_block] = SolveLinearSystem(A,b,restart,tol,maxit,method,L_block,U_block);

 figure,shg,semilogy(residual,'-r','LineWidth',2)
    hold on
    shg,semilogy(residual_block,'-b','LineWidth',2)
    shg,semilogy(residual_block_add,'-k','LineWidth',2)
    shg,semilogy(residual_block_add2,'-g','LineWidth',2)
    axis tight
    
    xlabel('Matrix Vector Product n',...
        'FontSize',FontSize)
    ylabel('Residual Norm  || r_n ||_2',...
        'FontSize',FontSize)
    h=legend('Without Preconditioning',...
        ['ILU(',num2str(el),'), Init'],...
        ['ILU(',num2str(el),'), Init & Add (Greedy Coloring)'],...
        ['ILU(',num2str(el),'), Init & Add (New Coloring)']);
    set(h,'Location','Best',...
        'FontSize',FontSize);
    %title(['Convergence History of ', ...
    %    upper(method), ' for ', MatrixName],...
    %s    'FontSize',FontSize)
    set(gca,'FontSize',FontSize)
    %myprint('convergence.png',MatrixName,DirName,h);