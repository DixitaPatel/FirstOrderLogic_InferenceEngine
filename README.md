**Description**

Using first-order-logic to answer the query raised.

**input.txt format**

1. Each query will be a single literal of the form Predicate(Constant) or ~Predicate(Constant).
2. Variables are all single lowercase letters.
3. All predicates (such as Sibling) and constants (such as John) are case-sensitive alphabetical strings that begin with an uppercase letter.
4. Each predicate takes at least one argument. Predicates will take at most 100 arguments. A given predicate name will not appear with different number of arguments.
5. There will be at most 100 queries and 1000 sentences in the knowledge base.

**Format for output.txt:**

For each query, determine if that query can be inferred from the knowledge base or not, one query per line.
Each answer should be either TRUE if you can prove that the corresponding query sentence is true given the knowledge base, or FALSE if you cannot.

**Example 1:**

For this input.txt:
6

F(Joe)

H(John)

~H(Alice)

~H(John)

G(Joe)

G(Tom)

14

~F(x) | G(x)

~G(x) | H(x)

~H(x) | F(x)

~R(x) | H(x)

~A(x) | H(x)

~D(x,y) | ~H(y)

~B(x,y) | ~C(x,y) | A(x)

B(John,Alice)

B(John,Joe)

~D(x,y) | ~Q(y) | C(x,y)

D(John,Alice)

Q(Joe)

D(John,Joe)

R(Tom)

**output.txt**  should be:

FALSE

TRUE

TRUE

FALSE

FALSE

TRUE
