function [FillIn,NrCol,NrAdd,NrPot,L_block, ...
         U_block,L_block_add,U_block_add]=...
         sparsify(A,el,NumProc,maxit,tol,strategy)
%SPARSIFY general idea in this script is as follows.
% [FillIn,NrCol,NrAdd,NrPot] = SPARSIFY(A,EL,NUMPROC,MAXIT,TOL,STRATEGY)
% A: the input matrix
% el: ILU parameter 

%
% BlockSize = floor(N/NumProc)
% NumProc : number of diagonal blocks in BlockA
%

% tol : newton tolerance
% maxit : the maximum iteration number
% strategy: cons aggr

% FillIn: Block iLU fill-in
% NrCol : Number of Colors
% NrAdd: Number of Additionaly Required Elements
% NrPot: Number of Potentially Required Elements

% We consider a fixed sparsification of a matrix A into block
% diagonal form. Let BlockA denote the result of this
% sparsification. We then take BlockA as the required elements
% of partial Jacobian computation for the graph of A.
% We refer to BlockA as the initially required elements.
% We then compute those entries of the non-initially
% required elements of A that can be computed without
% increasing the number of colors in the corresponding
% partial Jacobian computation. These elements are candidates
% to be added to the initially required elements. From
% these so-called potentially required elements, we find a
% subset of elements called additionally required elements
% such that the following holds:
% A symbolic ILU(el) factorization applied to BlockA
% generates the same fill-in pattern as applied
% to BlockA augmented by the pattern of additionally
% required elements.


%clear all;
close all;

% If RunLinearSystemSolver = true, the linear systems will
% actually be solved. Otherwise the preconditioner is
% set up but the linear sytems are not solved numerically.
RunLinearSystemSolver=true;
% If PrintToFile = true, the files for the plots will
% generated and printed to the directory 'PNG/'.
% File names will be generated from the name of the matrix.
PrintToFile = true;
%PrintToFile = false;
DirName = 'PNG-Sparsification';
FontSize = 14;

method ='bicgstab';
%method ='gmres';

% GMRES parameter
restart = 5;
%tol = 1.0e-15;
% maxit = 40;

% ILU parameter
% el = 4;

MatrixNumber = 11;
% 5 funktioniert

% MatrixNumber = 4, el = 3, numproc=4,restart=4 gmres gut fuer uns
% NumProc: number of diagonal blocks in BlockA. 
% The first NumProc - 1 blocks are of the same size, 
% but the last one is the remaining part
% NumProc = 10;

disp('============== Begin new run ==============')
%[A,MatrixName] = ReadMatrix(MatrixNumber);
%B = load('Matrices/co2_jac_first_iteration.mat')
%A=B.jac;
MatrixName = 'carbon sequestration'
% Generate right-hand side such that the exact solution
% is given by ones(N,1)
b = sum(A,2);
N = size(A,1);

if ( find(A~=0)==find(A'~=0) )
    disp('Structurally symmetric!');
end

% Decompose the matrix A into blocks of the same size.
% (The last block may have a different size.)
BlockSize = floor(N/NumProc);
Pptr    = 1:BlockSize:N;
Pptr = Pptr - 1;
Pptr(1) = [];
Pptr(size(Pptr,2)) = N;
Pptr = Pptr';
Qptr = Pptr;

% Sparsify the matrix A
[BlockA,nnzBlocks] = ExtractBlockDiagonal(A,Qptr,Pptr);

% Take BlockA as initially required nonzero entries and
% color BlockA. Then, compute those nonzero entries of A that
% can be added to BlockA such that the number of colors
% remains the same. We call these nonzero elements the
% potentially required nonzeros. Notice that the function
% below computes the sum of the initially required and
% potentially required nonzeros.


% The following is used to replace the old implementation of
% coloring and determination of potentially required elements:
%
%addpath('PotentialRequiredNonzeros');
%InitRequired = spones(BlockA);
%[InitPlusPotenRequired, NrCol] = postprocessingAllElements(spones(A),InitRequired);

%addpath('./GraphColoring/');
InitRequired = spones(BlockA);
coloring = PartialD2ColoringRestricted(spones(A),InitRequired);
p_d2 = max(coloring);
% Alternatively choose star bicoloring coloring
%[p_Vr,p_Vc,coloring_Vr,coloring_Vc] = ...
% StarBicoloringSchemeCombinedVertexCoverColoringRestricted(spones(A),InitRequired);
%p_sb_Vr = p_Vr
%p_sb_Vc = p_Vc
%p_sb = p_Vr + p_Vc

%addpath('./PotentiallyRequiredElements');
coloring_d = cast(coloring,'double');
[RpotD2] = potentialRequiredNonzerosD2(spones(A),InitRequired,coloring_d);

% Alternatively choose star bicoloring coloring
%coloring_Vr_d = cast(coloring_Vr,'double');
%coloring_Vc_d = cast(coloring_Vc,'double');
%[RpotSB] = potentialRequiredNonzerosSB(spones(A),InitRequired,coloring_Vr_d,coloring_Vc_d);

NrCol =  p_d2;

PotentRequired = RpotD2;
InitPlusPotenRequired = PotentRequired + spones(BlockA);

%disp(['Nr of colors = ',num2str(NrCol)]);


%spy(PotentRequired);
% subplot (2,2,1), spy(A), title('A')
% subplot (2,2,2), spy(BlockA), ...
%     title(['BlockA, block size = ',num2str(BlockSize)])
% subplot (2,2,3), spy(InitPlusPotenRequired), ...
%     title('Initially and potentially required')
% subplot (2,2,4), spy(PotentRequired), ...
%     title('Potentially required')

% ---- Symbolic ILU(el)
disp('Symbolic factorization')
[F] = SILUR(BlockA,el);
FillIn = nnz(F)-nnz(BlockA);

% From the set of potentially required elements,
% find the subset of those nonzeros that will not
% lead to additional fill-in as compared to F.
%addpath('./AdditionallyRequiredElements');

% Choose between two strategies for computing the additionally required
% elements

if(strategy == 'cons') 
% This one is a conservative strategy implemented in matlab
AdditionalRequired = ...
    ComputeAdditionalRequired(PotentRequired,F);
else
% This one is an aggressive strategy implemented in CPP
AdditionalRequired = ...
    additionallyRequiredElements(PotentRequired,F);
end

NrAdd=sum(nnz(AdditionalRequired))
NrPot=sum(nnz(PotentRequired))

% Compute the set of required elements
R = InitRequired + AdditionalRequired;


% Check for consistency
[Fcheck] = SILUR(R,el);
if (F+AdditionalRequired~=Fcheck)
    disp('Something is wrong here.')
end


disp(['A:                sum = ',num2str(sum(nnz(A)))])
disp(['Initially    Req: sum = ',num2str(sum(nnz(InitRequired)))])
disp(['Fill-in edges         = ',num2str(FillIn)])
disp(['Potentially  Req: sum = ',num2str(sum(nnz(PotentRequired)))])
disp(['Additionally Req: sum = ',num2str(sum(nnz(AdditionalRequired)))])

% ---- Numeric ILU(el)
disp('block+add: Numeric factorization')
% Take block diagonal entries of PAP
[L_block_add,U_block_add] = ILUR(BlockA+AdditionalRequired.*A,F+AdditionalRequired);
% Check
cond=rcond(full(L_block_add*U_block_add));
disp(['block+add: reciprocal condition number = ',num2str(cond)])

disp('block: Numeric factorization')
% Take block diagonal entries of A
[L_block,U_block] = ILUR(BlockA,F);
% Check
cond=rcond(full(L_block*U_block));



% ---- Plot the Patterns (Abbreviations like 'g' are not allowed)
ColDefault='black';
ColInit='green';
ColPotent='red';
ColAdd='blue';
ColFill='magenta';

prow=4;
pcol=2;

%
%subplot(prow,pcol,1),shg, spy(A,ColDefault);title('A')
NonInitRequired = spones(A)-InitRequired;
%
% Plot A decomposed into initially required and remaining nonzeros
%subplot(prow,pcol,2),shg,...
%    two_spy(InitRequired,NonInitRequired,ColInit,ColDefault,FontSize,MatrixName),...
%    title('Decomposition of A')
%legend('Initially Required')
%
%subplot(prow,pcol,3),shg,...
%    spy(InitRequired,ColInit),...
%    title('Initially Required')
%legend('Initially Required')
%
%subplot(prow,pcol,4),shg,...
%    spy(NonInitRequired,ColDefault),...
%    title('Non-initially Required')
%legend('Non-Initially Required')
%
%subplot(prow,pcol,5),shg,...
%    two_spy(NonInitRequired-PotentRequired,PotentRequired,ColDefault,ColPotent,FontSize,MatrixName),...
%    title('Decomposition of Non-initially Required')
%legend('Potentially Required')
%
%subplot(prow,pcol,6),shg,...
%    spy(PotentRequired,ColPotent),...
%    title('Potentially Required')
%legend('Potentially Required')
%
%subplot(prow,pcol,7),shg,...
%    two_spy(PotentRequired-AdditionalRequired,AdditionalRequired,ColPotent,ColAdd,FontSize,MatrixName),...
%    title('Decomposition of Potentially Required')
%legend('Additionally Required')
%
%subplot(prow,pcol,8),shg,...
%    spy(AdditionalRequired,ColAdd),...
%    title('Additionally Required')

% Plot to files
%figure;
%PlotPattern(A,ColDefault,...
%    'Nonzeros','Nonzeros',...
%    FontSize,PrintToFile,'p1.png',MatrixName,DirName);
%TwoPlotPattern(InitRequired,NonInitRequired,ColInit,ColDefault,...
%    'Initially Required','Nonzeros',...
%    FontSize,PrintToFile,'p2.png',MatrixName,DirName);
%PlotPattern(InitRequired,ColInit,...
%    'Initially Required','Initially Required',...
%    FontSize,PrintToFile,'p3.png',MatrixName,DirName);
%if FillIn>0
%    TwoPlotPattern(F-InitRequired,InitRequired,ColFill,ColInit,...
%        'Fill-in','Initially Required',...
%        FontSize,PrintToFile,'p0.png',MatrixName,DirName);
%end
%PlotPattern(NonInitRequired,ColDefault,...
%    'Non-initially Required','Non-initially Required',...
%    FontSize,PrintToFile,'p4.png',MatrixName,DirName);
%TwoPlotPattern(PotentRequired,NonInitRequired-PotentRequired,ColPotent,ColDefault,...
%    'Potentially Required','Non-initially Required',...
%    FontSize,PrintToFile,'p5.png',MatrixName,DirName);
%PlotPattern(PotentRequired,ColPotent,...
%    'Potentially Required','Potentially Required',...
%    FontSize,PrintToFile,'p6.png',MatrixName,DirName);
%TwoPlotPattern(AdditionalRequired,PotentRequired-AdditionalRequired,ColAdd,ColPotent,...
%    'Additionally Required','Potentially Required',...
%    FontSize,PrintToFile,'p7.png',MatrixName,DirName);
%PlotPattern(AdditionalRequired,ColAdd,...
%    'Additionally Required','Additionally Required',...
%    FontSize,PrintToFile,'p8.png',MatrixName,DirName);




% ---- Solve linear system
%if RunLinearSystemSolver
    %disp('Solve without PC')
    %[x,residual] = ...
    %    SolveLinearSystem(A,b,restart,tol,maxit,method);
    %res=residual;
    %disp('Solve with block + add PC')
    %[x_pc,residual_block_add] = ...
    %    SolveLinearSystem(A,b,restart,tol,maxit,method,L_block_add,U_block_add);
    %res_add=residual_block_add;
    %disp('Solve with block PC')
    %[x_orig,residual_block] = ...
    %    SolveLinearSystem(A,b,restart,tol,maxit,method,L_block,U_block);
    %res_block=residual_block;
    
    %figure('Visible','off'),shg,semilogy(residual,'-r*')
    %hold on
    %shg,semilogy(residual_block,'-bo')
    %shg,semilogy(residual_block_add,'-ks')
    
    
    %xlabel('Matrix Vector Product n',...
       % %FontSize',FontSize)
    %ylabel('Residual Norm  || r_n ||_2',...
       % 'FontSize',FontSize)
    %h=legend('Without PC',...
        %['ILU(',num2str(el),'), Init'],...
        %['ILU(',num2str(el),'), Init & Add']);
    %set(h,'Location','Best',...
        %'FontSize',FontSize);
    %title(['Convergence History of ', ...
        %upper(method), ' for ', MatrixName],...
        %'FontSize',FontSize)
    %set(gca,'FontSize',FontSize)
    %myprint('convergence.png',MatrixName,DirName);
    
    
%end

end


