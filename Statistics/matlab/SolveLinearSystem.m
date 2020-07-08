function [x,ResidualHistory] = SolveLinearSystem(A,b,restart,tol,maxit,method,L,U)
%SOLVELINEARSYSTEM computes the solution of Ax=b using an iterative method
%specified by the string METHOD.


if nargin == 6
    % without PC
    switch lower(method)
        case 'gmres'
            [x,flag,~,~,ResidualHistory] = gmres(A,b,restart,tol,maxit);
        case 'bicgstab'
            [x,flag,~,~,ResidualHistory] = bicgstab(A,b,tol,maxit);
        otherwise
            disp('Wrong method specified')
    end
elseif nargin == 8
    % with PC
    switch lower(method)
        case 'gmres'
            [x,flag,~,~,ResidualHistory] = ...
                gmres(A,b,restart,tol,maxit,L,U);
        case 'bicgstab'
            [x,flag,~,~,ResidualHistory] = ...
                bicgstab(A,b,tol,maxit,L,U);
        otherwise
            disp('Wrong method specified')
    end
else
    error('Wrong number of arguments.')
end

switch flag
    case 0
        disp([method,' converged to the desired tolerance'])
    case 1
        disp([method,' iterated maxit times but did not converge'])
    case 2
        disp([method,': Preconditioner M was ill-conditioned.'])
    case 3
        disp([method,' stagnated. (Two consecutive iterates were the same.)'])
    case 4
        if lower(method)=='bicgstab'
            disp([method,': One of the scalar quantities calculated during BICGSTAB became too small or too large to continue computing.'])
        else
            error('This flag is only for BiCGSTAB.')
        end
    otherwise
        disp(['This should never happen. Unusual flag returned from ',method])
end

%disp('The exact solution x* is known.')
disp(['The error is given by ||x-x*|| = ',...
    num2str( norm(x-ones(length(x),1)))])

end

