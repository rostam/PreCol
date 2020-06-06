clear all
close all

A = mmread('rajat11.mtx');
%A = A(1:100,1:100);


figure
subplot(2,2,1)
spy(A,'b');

subplot(2,2,2)
[a,b] = getBlock(A,10);
spy(a,'r');

subplot(2,2,3)
F = SILUR(A,2);
spy(F,'b');

subplot(2,2,4)
f = SILUR(a,2);
spy(f,'r');

