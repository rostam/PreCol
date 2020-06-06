function R = ComputeAdditionalRequired(P,F);

% Suppose that you computed a symbolic ILU(el) factorization
% of a matrix A that led to a filled matrix with nonzero pattern F.
% Suppose further that there is a set of nonzero elements P 
% where P is disjoint from F. Then, this function returns a set 
% of nonzero elements R that can be added to F without
% resulting in additional fill-in when ILU(el) is applied 
% to a matrix with pattern A + R.

% This heuristic is very conservative, meaning that the set R
% could be larger.

if sum(sum(P.*F))~=0
    error('P and F should be disjoint patterns.')
end
    
% Necessary condition fot the code below.
if sum(diag(P))~=0
    error('There is a nonzero on the diagonal of P');
end

R = zeros(size(F));

[I,J] = find(P);
for k = 1:length(I)
    % node k of the set P representing (i,j)
    if I(k) < J(k)
        % i < j: look for predecessors of i
        n_i = find(F(:,I(k))); %predecessors of node I(k);
        violation = sum(nonzeros(n_i)>I(k)) > 0;
    else
        % i > j: look for successors of j
        n_j = find(F(J(k),:)); %successors of node J(k)
        violation = sum(nonzeros(n_j)>J(k)) > 0;
    end
    if ~violation
        % add node (i,j)
        R(I(k),J(k)) = 1;
    end
    
end

%R=sparse(R);

end