close all;

MatrixName= 'nos3.mtx';
method ='bicgstab';
%method ='gmres';
el = 2;
% GMRES parameter
restart = 3;
tol = 1.0e-13;
maxit = 150;

DirName = 'figs';
FontSize = 14;

A = mmread(MatrixName);
[req,pot,add,F] = precol(MatrixName);
add2 = mmread('add_mat2.mtx');
add2 = spones(add2);
F2 = mmread('F2.mtx');
F2 = spones(F2);
b = sum(A,2);

[L_block,U_block] = ILUR(req.*A,req+F);
[L_block_add,U_block_add] = ILUR(req.*A+add.*A,req+F+add);
[L_block_add2,U_block_add2] = ILUR(req.*A+add2.*A,req+F+add2);

disp('Solve without PC')
[x,residual] = SolveLinearSystem(A,b,restart,tol,maxit,method);
    
disp('Solve with block + add PC')
[x_pc,residual_block_add] = SolveLinearSystem(A,b,restart,tol,maxit,method,L_block_add,U_block_add);

    
disp('Solve with block + add PC2')
[x_pc2,residual_block_add2] = SolveLinearSystem(A,b,restart,tol,maxit,method,L_block_add2,U_block_add2);
    
disp('Solve with block PC')
[x_orig,residual_block] = SolveLinearSystem(A,b,restart,tol,maxit,method,L_block,U_block);

 figure,shg,semilogy(residual,'-r*')
    hold on
    shg,semilogy(residual_block,'-bo')
    shg,semilogy(residual_block_add,'-ks')
    shg,semilogy(residual_block_add2,'-gs')
   
    xlabel('Matrix Vector Product n',...
        'FontSize',FontSize)
    ylabel('Residual Norm  || r_n ||_2',...
        'FontSize',FontSize)
    h=legend('Without PC',...
        ['ILU(',num2str(el),'), Init'],...
        ['ILU(',num2str(el),'), Init & Add'],...
        ['ILU Init & Add2']);
    set(h,'Location','Best',...
        'FontSize',FontSize);
    title(['Convergence History of ', ...
        upper(method), ' for ', MatrixName],...
        'FontSize',FontSize)
    set(gca,'FontSize',FontSize)
    myprint('convergence.png',MatrixName,DirName);