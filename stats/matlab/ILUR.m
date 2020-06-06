function [L,U] = ILUR(A,F)
%ILUR  Computes an incomplete LU decomposition
%   I_ncomplete
%   LU decompostion
%   Row-oriented in classical form
%
% Decompose a square matrix A into factors L and U of the same size such
% that A = L*U + R where L and U are lower and upper triangular matrices
% and F = L + U -I is the filled matrix.

% 08/29/10 by M. Buecker, RWTH Aachen University

% Check on inputs
if nargin ~= 2
    error('Not correct number of input arguments.');
end
if ~isfloat(A)
    error('Not correct type of matrix A.');
end
%if ~islogical(F)
%    error('Not correct type of matrix F.');
%end
if ~(ndims(A)==2)
    error('Not correct number of dimension of matrix A.');
end
[m,n] = size(A);
if m~=n
    error('Input A should be a square matrix.')
end
if ~(ndims(F)==2)
    error('Not correct number of dimension of matrix F.');
end
[mF,nF] = size(F);
if mF~=nF
    error('Input F should be a square matrix.');
end
if ~((m==mF) && (n==nF))
    error('Input A and F should be of the same size.');
end



% The implementations assumes that the nonzero pattern of A is a subset of
% the nonzero pattern of F.
if min(min(spones(F)-spones(A)))<0;
    error('The Nonzero pattern of A is not a subset of the nonzero pattern of F.')
end

for i = 2:n % could also run from 1:n
    for  k = 1:i-1
        if F(i,k) ~=0
            A(i,k) = A(i,k)/A(k,k);
            for j = k+1:n
                if (F(i,j)~= 0) && (F(k,j)~=0)
                    A(i,j) = A(i,j) - A(i,k)*A(k,j);
                end
            end
        end
    end
end

% Extract lower and upper parts of A into separate varaiables.
L = tril(A);
L = spdiags(ones(n,1),0,L);
U = triu(A);
end
