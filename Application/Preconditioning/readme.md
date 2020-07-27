# PreCol - Application in Preconditioning

![PreCol](doc/images/image.png)
 
 1. [Partial Coloring and Preconditioning](#PreCol)
 2. [Coloring Algorithms](#colalg)
 &nbsp;
 
 Given a large sparse nonsingular $n\times n$ Jacobian matrix $J$,
 we are considering the solution to the following system of linear equations,
 ```
 J x = b,
 ```
 in which $x$ and $b$ are $n\times 1$ vectors.
 Iterative solvers are considered to be among the effective solution techniques~\cite{ilu2003}.
 %These solvers are matrix-free which makes AD as a suitable method of differentiation.
 
 Iterative techniques are typically used in combination with
 the preconditioning techniques~\cite{precond1,ilu2003}.
 Rather than solving the previous system,
 we can solve the preconditioned system
```
 M^{-1} J x= M^{-1} b,
```
 where the $n \times n$ matrix $M$ serves as a preconditioner that approximates
 the coefficient matrix,

```
 M approximates J
 ```
 Some preconditioning techniques like ILU preconditioning can generate a preconditioner
 which has nonzero at some places in which the Jacobian matrix $J$ has zero elements.
 These nonzero elements are called fill-in.