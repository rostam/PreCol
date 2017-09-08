close all;

MatrixName= 'can_96.mtx';
method ='bicgstab';
%method ='gmres';
el = 2;
% GMRES parameter
restart = 3;
tol = 1.0e-17;
maxit = 100;

DirName = 'figs';
FontSize = 14;
%B = load('co2_jac_first_iteration.mat');
%A = B.jac; %mmread(MatrixName);
A = mmread(MatrixName);
[req,pot,add,F,c] = precol2('D2RestrictedColumns','Nat','Nat','15','2','can_96.mtx',-1);
[req2,pot2,add2,F2,c2] = precol2('D2RestrictedColumnsNonReq','Nat','Nat','15','2','can_96.mtx',-1);
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

set(0,'defaulttextinterpreter','latex')
 figure,shg,semilogy(residual,'-r','LineWidth',2)
    hold on
    shg,semilogy(residual_block,'-b','LineWidth',2)
    shg,semilogy(residual_block_add,'-k','LineWidth',2)
    shg,semilogy(residual_block_add2,'-g','LineWidth',2)
    axis tight
    
    xlabel('Matrix-vector products',...
        'FontSize',FontSize)
    ylabel('Residual norm  $\| r_n \|_2$',...
        'FontSize',FontSize)
    h=legend('Without preconditioning',...
        ['ILU(',num2str(el),'), $R_i$'],...
        ['ILU(',num2str(el),'), $R_i\cup R_a$ (Algorithm 3.1)'],...
        ['ILU(',num2str(el),'), $R_i\cup R_a$ (Algorithm 3.5)']);
    set(h,'Location','Best',...
        'FontSize',FontSize);
    set(h,'Interpreter','Latex');
    %title(['Convergence History of ', ...
    %    upper(method), ' for ', MatrixName],...
    %s    'FontSize',FontSize)
    set(gca,'FontSize',FontSize)
    ylim(gca,[10^-14 10^2])
    %myprint('convergence.png',MatrixName,DirName,h);