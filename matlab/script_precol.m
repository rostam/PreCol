close all;

MatrixName= 'nos3.mtx';
method ='bicgstab';
%method ='gmres';
el = 2;
% GMRES parameter
restart = 3;
tol = 1.0e-12;
maxit = 150;

DirName = 'figs';
FontSize = 14;

A = mmread(MatrixName);
[req,pot,add,F] = precol(MatrixName);
b = sum(A,2);

[L_block,U_block] = ILUR(req.*A,req+F);
[L_block_add,U_block_add] = ILUR(req.*A+add.*A,req+F+add);

disp('Solve without PC')
[x,residual] = SolveLinearSystem(A,b,restart,tol,maxit,method);
    
disp('Solve with block + add PC')
[x_pc,residual_block_add] = SolveLinearSystem(A,b,restart,tol,maxit,method,L_block_add,U_block_add);
    
disp('Solve with block PC')
[x_orig,residual_block] = SolveLinearSystem(A,b,restart,tol,maxit,method,L_block,U_block);

 figure,shg,semilogy(residual,'-r','LineWidth',2)
    hold on
    shg,semilogy(residual_block,'-b','LineWidth',2)
    shg,semilogy(residual_block_add,'-k','LineWidth',2)
    
    
    xlabel('Matrix Vector Product n',...
        'FontSize',FontSize)
    ylabel('Residual Norm  || r_n ||_2',...
        'FontSize',FontSize)
    h=legend('Without PC',...
        ['ILU(',num2str(el),'), Init'],...
        ['ILU(',num2str(el),'), Init & Add']);
    set(h,'Location','Best',...
        'FontSize',FontSize);
    title(['Convergence History of ', ...
        upper(method), ' for ', MatrixName],...
        'FontSize',FontSize)
    set(gca,'FontSize',FontSize)
    myprint('convergence.png',MatrixName,DirName,h);