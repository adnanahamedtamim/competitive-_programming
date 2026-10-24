The Concept

You have a set of tasks or projects. Each project yields a profit (positive weight) or costs money (negative weight). However, there are dependencies: to complete project A, you must also complete project B. Your goal is to select a valid subset of projects to maximize the total profit.

The Model (Min-Cut)
Add a Super Source (S) and Super Sink (T).
For every project with a positive profit P, add an edge S → project with capacity P.
For every project with a negative profit (cost) C, add an edge project → T with capacity |C|.
For every dependency "A requires B", add a directed edge A → B with capacity ∞ (infinity).
The Answer

The maximum profit is:

Maximum Profit = (Sum of All Positive Profits) − MaxFlow(S, T)
