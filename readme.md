# Red-Black Tree

## Intro

- Respect to Rudolf Bayer, Leo J. Guibas and Robert Sedgewick for their contributions!

- This is a clear and minimal implementation of Red-Black Tree.


## Red-Black Tree Properties

- Every node is either red or black.
- The root and leaves(NIL's) are black.
- If a node is red, then its parent is black.
- All simple paths from any node X to a descendant leaf have the same number of black nodes = black-height(X).

## Time Complexity

| Opereation | Average | Worst Case |
| ---- | ---- | ---- |
| Space | O(N) | O(N) |
| Search | O(logN) | O(logN) |
| Insert | O(logN) | O(logN) |
| Delete | O(logN) | O(logN) |

## Reference

- https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
- Lec10, MIT6.046J
