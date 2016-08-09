e

% Check on inputs
if nargin~=2
    error('Not correct number of input arguments.');
end
if ~(ndims(A)==2)
    error('Not correct number of dimension of matrix A.');
end
[m,n] = size(A);
if m~=n
    error('Input A should be a square matrix.');
end
if ~isfloat(A)
    error('Not correct type of matrix A.');
end
% We are only interested in the nonzero pattern
A = spones(A);
if el<0
    error('The level el should be >= 0.');
end

F = sparse(zeros(n));
level = spalloc(n,n,n^2);
for i = 1:n
    % All entries of A are entries of F with level = 0.
    index_nnz = find( A(i,:)==1 );
    level(i,index_nnz) = 0;
    F(i,index_nnz) = 1;
    % Update phase
    for k = 1:i-1  % node (i,k) with k<i
        if F(i,k)==1
            for j = k + find( F(k,k+1:n)==1 ) % node (k,j) with k<j
                lev = level(i,k) + level(k,j) + 1;
                if(lev <= el)
                    if F(i,j)==0
                        F(i,j) = 1;
                        level(i,j) = lev;
                    else
                        level(i,j) = min(level(i,j),lev);
                    end;
                end
            end;
        end
    end
end
end

