
function [S, NnzBlocks] = ExtractBlockDiagonal(A,RowBlockPtr,ColBlockPtr)
%EXTRACTBLOCKDIAGONAL extracts blocks from the diagonal of A
%  A the given matrix
%  RowBlockPtr(i) points to the last row of block i
%  ColBlockPtr(i) points to the last column of block i
%  S is the block diagonal of A  
%  NnzBlocks a sequence of number of nonzero elements in each block

S = A(1:RowBlockPtr(1),1:ColBlockPtr(1));
NnzBlocks=nnz(S);
for i = 2:size(RowBlockPtr)
    new_block = A(RowBlockPtr(i-1)+1:RowBlockPtr(i),ColBlockPtr(i-1)+1:ColBlockPtr(i));
    S = blkdiag(S,new_block);
    NnzBlocks = [NnzBlocks nnz(new_block)];
end

end

