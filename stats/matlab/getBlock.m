function [BlockA,nnzBlocks] = getBlock(A,BlockSize)
N = size(A,1);
Pptr    = 1:BlockSize:N;
Pptr = Pptr - 1;
Pptr(1) = [];
Pptr(size(Pptr,2)) = N;
Pptr = Pptr';
Qptr = Pptr;
[BlockA,nnzBlocks] = ExtractBlockDiagonal(A,Qptr,Pptr);
end